#include "Sysinfo.h"

String SysinfoClass::getWiFiMac()
{
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    return String(baseMacChr);
}

void SysinfoClass::page_0()
{
    chipRev = ESP.getChipRevision();
    cpuSpeed = ESP.getCpuFreqMHz();
    flashSpeed = ESP.getFlashChipSpeed();
    ramFree = esp_get_free_heap_size();
    WiFiMAC = getWiFiMac();

    M5.Lcd.drawString(F("CPU FREQ:"), 10, 40, 2);
    M5.Lcd.drawNumber(cpuSpeed, 120, 40, 2);

    M5.Lcd.drawString(F("CPU CORES:"), 10, 60, 2);
    M5.Lcd.drawNumber(2, 120, 60, 2);

    M5.Lcd.drawString(F("CHIP REV:"), 10, 80, 2);
    M5.Lcd.drawNumber(chipRev, 120, 80, 2);

    M5.Lcd.drawString(F("FLASH SPEED:"), 10, 100, 2);
    M5.Lcd.drawNumber(flashSpeed, 120, 100, 2);

    M5.Lcd.drawString(F("FREE RAM:"), 10, 120, 2);
    M5.Lcd.drawNumber(ramFree, 120, 120, 2);

    M5.Lcd.drawString(F("WIFI STA MAC:"), 10, 140, 2);
    M5.Lcd.drawString(WiFiMAC, 120, 140, 2);

    M5.Lcd.drawString(F("IP ADDRESS:"), 10, 160, 2);
    M5.Lcd.drawString((WiFi.localIP().toString()), 120, 160, 2);

    M5.Lcd.drawString(F("WIFI MODE:"), 10, 180, 2);
    M5.Lcd.drawString(wifi_m_mode[WiFi.getMode()], 120, 180, 2);
}

void SysinfoClass::page_1()
{
    flashSize = ESP.getFlashChipSize();
    type = SD.cardType();

    M5.Lcd.drawString(F("MIN FREE HEAP:"), 10, 40, 2);
    M5.Lcd.drawNumber(esp_get_minimum_free_heap_size(), 120, 40, 2);

    M5.Lcd.drawString(F("FLASH SIZE:"), 10, 60, 2);
    M5.Lcd.drawNumber(flashSize, 120, 60, 2);

    M5.Lcd.drawString(F("SPIFFS SIZE:"), 10, 80, 2);
    M5.Lcd.drawNumber(SPIFFS.totalBytes(), 120, 80, 2);

    M5.Lcd.drawString(F("SPIFFS USED:"), 10, 100, 2);
    M5.Lcd.drawNumber(SPIFFS.usedBytes(), 120, 100, 2);

    M5.Lcd.drawString(F("SD CARD TYPE:"), 10, 120, 2);
    M5.Lcd.drawString(SD_Type[type], 120, 120, 2);

    if (type != 0)
    {
        sdSize = SD.cardSize();
        sdUsed = SD.usedBytes();
        M5.Lcd.drawString(F("SD CARD SIZE:"), 10, 140, 2);
        M5.Lcd.drawString(String(sdSize), 120, 140, 2);

        M5.Lcd.drawString(F("SD BYTES USED:"), 10, 160, 2);
        M5.Lcd.drawString(String(sdUsed), 120, 160, 2);
    }
}

void SysinfoClass::drawpage(int page)
{
    switch (page)
    {
    case 0:
        page_0();
        break;
    case 1:
        page_1();
        break;
    }
}

void SysinfoClass::Run()
{
    while (!M5.BtnB.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            if (page < PAGEMAX)
            {
                page++;
            }
            else
            {
                page = 0;
            }
            done = false;
        }

        if (M5.BtnA.wasPressed())
        {
            if (page > 0)
            {
                page--;
            }
            else
            {
                page = PAGEMAX;
            }
            done = false;
        }

        if (!done)
        {
            MyMenu.windowClr();
            drawpage(page);
            done = true;
        }
        M5.update();
    }
    done = false;
}

SysinfoClass::SysinfoClass()
{
    M5.update();
    MyMenu.drawAppMenu(F("M5 SYSTEM INFO"), F("<"), F("ESC"), F(">"));
}

SysinfoClass::~SysinfoClass()
{
    MyMenu.show();
}