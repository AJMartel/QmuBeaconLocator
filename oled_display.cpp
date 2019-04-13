#include "oled_display.h"
#include "Arduino.h"
#include "beacons.h"

OledDisplay::OledDisplay(SSD1306 *display) {
    _display = display;
}

void OledDisplay::init() {
    _display->init();
    _display->flipScreenVertically();
    _display->setFont(ArialMT_Plain_10);
}

void OledDisplay::loop() {
    page(pageSequence[_mainPageSequenceIndex]);
}

void OledDisplay::nextPage() {
    _mainPageSequenceIndex++;
    if (_mainPageSequenceIndex == OLED_DISPLAY_PAGE_COUNT) {
        _mainPageSequenceIndex = 0;
    }
}

void OledDisplay::page(uint8_t page) {

    static uint32_t lastUpdate = 0;

    //Do not allow for OLED to be updated too often
    if (lastUpdate > 0 && millis() - lastUpdate < 200 && _forceDisplay == false) {
        return;
    }

    _forceDisplay = false;

    switch (page) {
        
        case OLED_PAGE_LIST:
            renderPageList();
            break;
        case OLED_PAGE_BEACON:
            renderPageBeacon();
            break;
    }
    _page = page;

    lastUpdate = millis();
}

void OledDisplay::renderHeader(String title) {
    _display->setFont(ArialMT_Plain_10);
    _display->drawString(0, 0, title);

    _display->drawString(90, 0, String(gps.satellites.value()) + " sats");
}

void OledDisplay::renderPageList() {
    _display->clear();

    renderHeader("Beacons");

    if (beacons.count() > 0) {
        _display->setFont(ArialMT_Plain_10);
        _display->drawString(0, 16, "Beacon " + String(currentBeaconIndex + 1) + "/" + String(beacons.count()));


    } else {
        _display->setFont(ArialMT_Plain_16);

        _display->drawString(0, 32, "No beacons");
    }

    _display->display();
}

void OledDisplay::renderPageBeacon() {
    _display->clear();

    renderHeader("Locator");

    
    _display->display();
}