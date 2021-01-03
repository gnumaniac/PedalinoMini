/*
__________           .___      .__  .__                 _____  .__       .__     ___ ________________    ___
\______   \ ____   __| _/____  |  | |__| ____   ____   /     \ |__| ____ |__|   /  / \__    ___/     \   \  \
 |     ___// __ \ / __ |\__  \ |  | |  |/    \ /  _ \ /  \ /  \|  |/    \|  |  /  /    |    | /  \ /  \   \  \
 |    |   \  ___// /_/ | / __ \|  |_|  |   |  (  <_> )    Y    \  |   |  \  | (  (     |    |/    Y    \   )  )
 |____|    \___  >____ |(____  /____/__|___|  /\____/\____|__  /__|___|  /__|  \  \    |____|\____|__  /  /  /
               \/     \/     \/             \/               \/        \/       \__\                 \/  /__/
                                                                                   (c) 2018-2021 alf45star
                                                                       https://github.com/alf45tar/PedalinoMini
 */

#ifdef TTGO_T_DISPLAY
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>

TFT_eSPI    display  = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);

#endif

#include <WiFi.h>
#include "Images.h"
#include "Fonts.h"

bool          uiUpdate = true;


void display_clear()
{
  display.fillScreen(TFT_BLACK);
}

void display_progress_bar_title(String title)
{
  display.fillScreen(TFT_BLACK);
  display.setTextSize(1);
  display.setFreeFont(&FreeSans18pt7b);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextDatum(BC_DATUM);
  display.drawString(title, display.width() / 2, display.height() / 2);
}

void display_progress_bar_title2(String title1, String title2)
{
  display.fillScreen(TFT_BLACK);
  display.setTextSize(1);
  display.setFreeFont(&FreeSans18pt7b);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextDatum(BC_DATUM);
  display.drawString(title2, display.width() / 2, display.height() / 2);
  uint16_t h = display.fontHeight(1);
  display.setFreeFont(&FreeSans9pt7b);
  display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  display.setTextDatum(BC_DATUM);
  display.drawString(title1, display.width() / 2, display.height() / 2 - h);

}

void display_progress_bar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress)
{
  height += height % 2;

  uint16_t radius = height / 2;
  uint16_t doubleRadius = 2 * radius;
  uint16_t innerRadius = radius - 2;

  TFT_eSprite bar = TFT_eSprite(&display);

  bar.createSprite(width, height);

  bar.drawCircleHelper(radius,             radius, radius, 0b00001001, TFT_LIGHTGREY);
  bar.drawCircleHelper(width - 1 - radius, radius, radius, 0b00000110, TFT_LIGHTGREY);
  bar.drawFastHLine(radius - 1, 0,          width - doubleRadius + 1, TFT_LIGHTGREY);
  bar.drawFastHLine(radius - 1, height - 1, width - doubleRadius + 1, TFT_LIGHTGREY);

  uint16_t maxProgressWidth = (width - doubleRadius - 1) * progress / 100;

  bar.fillCircle(radius, radius, innerRadius, TFT_ORANGE);
  bar.fillRect(radius + 1, 2, maxProgressWidth, height - 4, TFT_ORANGE);
  bar.fillCircle(radius + maxProgressWidth, radius, innerRadius, TFT_ORANGE);

  bar.pushSprite(x, y);

  bar.deleteSprite();
}

void display_progress_bar_update(unsigned int progress, unsigned int total)
{
  display_progress_bar(0, display.height() / 2 + 8, display.width() - 1, display.height() / 8, 100*progress/total);
}

void display_progress_bar_2_update(unsigned int progress, unsigned int total)
{
  display_progress_bar(0, display.height() - display.height() / 8, display.width() - 1, display.height() / 8, 100*progress/total);
}

void display_progress_bar_2_label(unsigned int label, unsigned int x)
{
  const String l(label);

  display.setTextSize(1);
  display.setFreeFont(&FreeSans9pt7b);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  if (x <= display.textWidth(l) / 2) {
    display.setTextDatum(BL_DATUM);
    display.drawString(l, 0, display.height() - display.height() / 8);
  }
  else if (x >= (display.width() - display.textWidth(l) / 2)) {
    display.setTextDatum(BR_DATUM);
    display.drawString(l, display.width() + 1, display.height() - display.height() / 8);
  }
  else {
    display.setTextDatum(BC_DATUM);
    display.drawString(l, x, display.height() - display.height() / 8);
  }
  display.drawFastVLine(x, display.height() - display.height() / 8, x, display.height() - 1);
}

void topOverlay()
{
  TFT_eSprite top = TFT_eSprite(&display);

  top.setColorDepth(8);
  top.createSprite(display.width(), 24);
  top.fillRect(0, 0, display.width(), 24, TFT_BLACK);

  if ((millis() >= endMillis2) ||
      (millis() < endMillis2 && MTC.getMode() == MidiTimeCode::SynchroNone)) {
#ifdef WIFI
    if (wifiEnabled) {
      top.fillCircleHelper(1, 22, 22, 0b00000010, 0, TFT_DARKGREY);
      top.fillCircleHelper(1, 22, 18, 0b00000010, 0, TFT_BLACK);
      top.fillCircleHelper(1, 22, 16, 0b00000010, 0, TFT_DARKGREY);
      top.fillCircleHelper(1, 22, 12, 0b00000010, 0, TFT_BLACK);
      top.fillCircleHelper(1, 22, 10, 0b00000010, 0, TFT_DARKGREY);
      top.fillCircleHelper(1, 22,  6, 0b00000010, 0, TFT_BLACK);
      top.fillCircleHelper(1, 22,  4, 0b00000010, 0, TFT_DARKGREY);
      if      (wifiLevel < -80) {
        top.fillCircleHelper(1, 22,  4, 0b00000010, 0, TFT_RED);
      }
      else if (wifiLevel < -72) {
        top.fillCircleHelper(1, 22, 10, 0b00000010, 0, TFT_ORANGE);
        top.fillCircleHelper(1, 22,  6, 0b00000010, 0, TFT_BLACK);
        top.fillCircleHelper(1, 22,  4, 0b00000010, 0, TFT_ORANGE);
      }
      else if (wifiLevel < -65) {
        top.fillCircleHelper(1, 22, 16, 0b00000010, 0, TFT_YELLOW);
        top.fillCircleHelper(1, 22, 12, 0b00000010, 0, TFT_BLACK);
        top.fillCircleHelper(1, 22, 10, 0b00000010, 0, TFT_YELLOW);
        top.fillCircleHelper(1, 22,  6, 0b00000010, 0, TFT_BLACK);
        top.fillCircleHelper(1, 22,  4, 0b00000010, 0, TFT_YELLOW);
      }
      else {
        top.fillCircleHelper(1, 22, 22, 0b00000010, 0, TFT_DARKGREEN);
        top.fillCircleHelper(1, 22, 18, 0b00000010, 0, TFT_BLACK);
        top.fillCircleHelper(1, 22, 16, 0b00000010, 0, TFT_DARKGREEN);
        top.fillCircleHelper(1, 22, 12, 0b00000010, 0, TFT_BLACK);
        top.fillCircleHelper(1, 22, 10, 0b00000010, 0, TFT_DARKGREEN);
        top.fillCircleHelper(1, 22,  6, 0b00000010, 0, TFT_BLACK);
        top.fillCircleHelper(1, 22,  4, 0b00000010, 0, TFT_DARKGREEN);
      }
      /*
      top.fillTriangle(1, 22, 30, 22, 30, 2, TFT_DARKGREY);
      int level = constrain(wifiLevel, -90, -60);
      if      (level < -80) top.fillTriangle(1, 22, map(level, -90, -60, 1, 30), 22, map(level, -90, -60, 1, 30), map(level, -90, -60, 22, 1), TFT_RED);
      else if (level < -72) top.fillTriangle(1, 22, map(level, -90, -60, 1, 30), 22, map(level, -90, -60, 1, 30), map(level, -90, -60, 22, 1), TFT_ORANGE);
      else if (level < -65) top.fillTriangle(1, 22, map(level, -90, -60, 1, 30), 22, map(level, -90, -60, 1, 30), map(level, -90, -60, 22, 1), TFT_YELLOW);
      else                  top.fillTriangle(1, 22, map(level, -90, -60, 1, 30), 22, map(level, -90, -60, 1, 30), map(level, -90, -60, 22, 1), TFT_DARKGREEN);
      for (byte c = 1; c < 6; c++)
        top.drawFastVLine(1+c*6, 0, 24, TFT_BLACK);
      */
      /*
      top.setSwapBytes(true);
      if      (wifiLevel < -90) top.pushImage(1, 1, 32, 22, w0);
      else if (wifiLevel < -80) top.pushImage(1, 1, 32, 22, w25);
      else if (wifiLevel < -70) top.pushImage(1, 1, 32, 22, w50);
      else if (wifiLevel < -60) top.pushImage(1, 1, 32, 22, w75);
      else                   top.pushImage(1, 1, 32, 22, w100);
      */
    }
#endif

#ifdef BLE
    if (bleMidiConnected) {
      top.drawFastVLine(36, 1, 22, TFT_BLUE);
      top.drawLine(41,  6, 31, 16, TFT_BLUE);
      top.drawLine(31,  6, 41, 16, TFT_BLUE);
      top.drawLine(36,  1, 41,  6, TFT_BLUE);
      top.drawLine(36, 22, 41, 16, TFT_BLUE);
     }
    /*
    top.setSwapBytes(true);
    if (bleMidiConnected)
      if (wifiEnabled) top.pushImage(40, 1, 16, 22, bt);
      else top.pushImage(1, 1, 16, 22, bt);
    else if (wifiEnabled) top.fillRect(40, 1, 16, 22, TFT_BLACK);
      else top.fillRect(1, 1, 16, 22, TFT_BLACK);
    */
#endif

#ifdef BATTERY
    top.drawRoundRect(display.width() - 50, 1, 44, 20, 4, TFT_WHITE);
    top.fillRoundRect(display.width() - 6, 7, 4, 8, 2, TFT_WHITE);
    top.fillRoundRect(display.width() - 50 + 2, 1 + 2, map(constrain(batteryVoltage, 3000, 4000), 3000, 4000, 0, 40), 20 - 4, 4, TFT_DARKGREEN);
    if (batteryVoltage > 4200) {
      top.fillTriangle(display.width() - 26,  3, display.width() - 34, 13, display.width() - 28, 12, TFT_WHITE);
      top.fillTriangle(display.width() - 30, 18, display.width() - 22,  9, display.width() - 28, 12, TFT_WHITE);
    }
/*
    top.setSwapBytes(true);
    if      (batteryVoltage > 4200) top.pushImage(display.width() - 50, 1, 50, 22, bcharge);
    else if (batteryVoltage > 4000) top.pushImage(display.width() - 50, 1, 50, 22, b100);
    else if (batteryVoltage > 3800) top.pushImage(display.width() - 50, 1, 50, 22, b75);
    else if (batteryVoltage > 3600) top.pushImage(display.width() - 50, 1, 50, 22, b50);
    else if (batteryVoltage > 3400) top.pushImage(display.width() - 50, 1, 50, 22, b25);
    else if (batteryVoltage > 3200) top.pushImage(display.width() - 50, 1, 50, 22, b10);
    else top.pushImage(display.width() - 50, 1, 50, 22, b0);
*/
#endif

    top.setFreeFont(&FreeSansBold9pt7b);
    top.setTextDatum(MC_DATUM);
    switch (currentProfile) {
      case 0:
        top.fillRoundRect(52 + 24*currentProfile, 1, 22, 22, 4, TFT_RED);
        top.setTextColor(TFT_WHITE, TFT_RED);
        top.drawString("A", 52 + 24*currentProfile + 11, 11);
        break;
      case 1:
        top.fillRoundRect(52 + 24*currentProfile, 1, 22, 22, 4, TFT_ORANGE);
        top.setTextColor(TFT_WHITE, TFT_ORANGE);
        top.drawString("B", 52 + 24*currentProfile + 11, 11);
        break;
      case 2:
        top.fillRoundRect(52 + 24*currentProfile, 1, 22, 22, 4, TFT_BLUE);
        top.setTextColor(TFT_WHITE, TFT_BLUE);
        top.drawString("C", 52 + 24*currentProfile + 11, 11);
        break;
    }
  }

/*
  if (millis() < endMillis2) {
    if (MTC.getMode() == MidiTimeCode::SynchroClockMaster ||
        MTC.getMode() == MidiTimeCode::SynchroClockSlave) {

      display->setFont(ArialMT_Plain_10);
      display->setTextAlignment(TEXT_ALIGN_CENTER);
      display->drawString(64, 0, String(bpm) + "BPM");
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->setFont(block10x10);
      switch (timeSignature) {
        case PED_TIMESIGNATURE_2_4:
          display->drawString( 98, 0, String(0));
          display->drawString(108, 0, String(0));
          break;
        case PED_TIMESIGNATURE_4_4:
          display->drawString( 98, 0, String(0));
          display->drawString(108, 0, String(0));
          display->drawString(118, 0, String(0));
          display->drawString(128, 0, String(0));
          break;
        case PED_TIMESIGNATURE_3_4:
        case PED_TIMESIGNATURE_3_8:
        case PED_TIMESIGNATURE_6_8:
        case PED_TIMESIGNATURE_9_8:
        case PED_TIMESIGNATURE_12_8:
          display->drawString( 98, 0, String(0));
          display->drawString(108, 0, String(0));
          display->drawString(118, 0, String(0));
          break;
      }
      switch (MTC.getBeat()) {
        case 0:
          if (MTC.isPlaying())
            display->drawString(98, 0, String(2));
          else
            display->drawString(98, 0, String(1));
          break;
        case 1:
          if (MTC.isPlaying())
            display->drawString(108, 0, String(2));
          else
            display->drawString(108, 0, String(1));
          break;
        case 2:
          if (MTC.isPlaying())
            display->drawString(118, 0, String(2));
          else
            display->drawString(118, 0, String(1));
          break;
        case 3:
          if (MTC.isPlaying())
            display->drawString(128, 0, String(2));
          else
            display->drawString(128, 0, String(1));
          break;
      }

      //MTC.isPlaying() ? display->setColor(WHITE) : display->setColor(BLACK);
      switch (timeSignature) {
        case PED_TIMESIGNATURE_2_4:
          display->fillRect(64 * MTC.getBeat(), 0, 64, 10);
          break;
        case PED_TIMESIGNATURE_4_4:
          display->drawRect(77 + 13 * 0, 0, 12, 10);
          display->drawRect(77 + 13 * 1, 0, 12, 10);
          display->drawRect(77 + 13 * 2, 0, 12, 10);
          display->drawRect(77 + 13 * 3, 0, 12, 10);
          if (MTC.isPlaying())
            display->fillRect(79 + 13 * MTC.getBeat(), 2,  8,  6);
          else
            display->drawRect(81 + 13 * MTC.getBeat(), 3,  4,  4);
          break;
        case PED_TIMESIGNATURE_3_4:
        case PED_TIMESIGNATURE_3_8:
        case PED_TIMESIGNATURE_6_8:
        case PED_TIMESIGNATURE_9_8:
        case PED_TIMESIGNATURE_12_8:
          display->fillRect(43 * MTC.getBeat(), 0, 42, 10);
          break;
      }
      //display->setColor(WHITE);
    }
    else if (MTC.getMode() == MidiTimeCode::SynchroMTCMaster ||
             MTC.getMode() == MidiTimeCode::SynchroMTCSlave) {
      char buf[12];
      sprintf(buf, "%02d:%02d:%02d:%02d", MTC.getHours(), MTC.getMinutes(), MTC.getSeconds(), MTC.getFrames());
      display->setFont(ArialMT_Plain_10);
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->drawString(128, 0, buf);
    }
  }
  */

  top.pushSprite(0, 0);

  top.deleteSprite();
}

void bottomOverlay()
{
  TFT_eSprite bottom = TFT_eSprite(&display);

  bottom.setColorDepth(8);
  bottom.createSprite(display.width(), 24);
  bottom.fillRect(0, 0, display.width(), 24, TFT_BLACK);

  if (lastUsed == lastUsedPedal && lastUsed != 0xFF && millis() < endMillis2 && lastPedalName[0] != ':') {
    int p;
    switch (m1) {

      case midi::ControlChange:
        m3 = constrain(m3, rmin, rmax);
        p = map(m3, rmin, rmax, 0, 100);
        display_progress_bar(0, display.height() - 24, display.width(), 24, p);
        if (lastPedalName[0] != 0) display_progress_bar_2_label(m3, map(p, 0, 100, 3, 124));
        break;
/*
      case midi::PitchBend:
        p = map(((m3 << 7) | m2) + MIDI_PITCHBEND_MIN, MIDI_PITCHBEND_MIN, MIDI_PITCHBEND_MAX, -100, 100);
        if ( p >= 0 ) {
          display_progress_bar(60, 54, 67, 8, p);
          uint16_t radius = 8 / 2;
          uint16_t xRadius = 0 + radius;
          uint16_t yRadius = 54 + radius;
          uint16_t doubleRadius = 2 * radius;
          display.drawCircleHelper(xRadius, yRadius, radius, 0b00001001, TFT_WHITE);
          display.drawFastHLine(xRadius, 54, 68 - doubleRadius, TFT_WHITE);
          display.drawFastHLine(xRadius, 54 + 8, 68 - doubleRadius, TFT_WHITE);
          display.drawCircleHelper(0 + 68 - radius, yRadius, radius, 0b00000110, TFT_WHITE);
        }
        else {
          display_progress_bar(60, 54, 67, 8, 0);
          uint16_t radius = 8 / 2;
          uint16_t xRadius = 0 + radius;
          uint16_t yRadius = 54 + radius;
          uint16_t doubleRadius = 2 * radius;
          uint16_t innerRadius = radius - 2;
          display.drawCircleHelper(xRadius, yRadius, radius, 0b00001001, TFT_WHITE);
          display.drawFastHLine(xRadius, 54, 68 - doubleRadius, TFT_WHITE);
          display.drawFastHLine(xRadius, 54 + 8, 68 - doubleRadius, TFT_WHITE);
          display.drawCircleHelper(0 + 68 - radius, yRadius, radius, 0b00000110, TFT_WHITE);
          uint16_t maxProgressWidth = (68 - doubleRadius) * p / 100;
          display.fillCircle(68 + maxProgressWidth - xRadius, yRadius, innerRadius, TFT_WHITE);
          display.fillRect(68 + maxProgressWidth - xRadius + 1, 54 + 2, -maxProgressWidth, 8 - 3, TFT_WHITE);
          display.fillCircle(68 - xRadius, yRadius, innerRadius, TFT_WHITE);
        }
        break;
*/
      case midi::AfterTouchChannel:
        m3 = constrain(m2, rmin, rmax);
        p = map(m3, rmin, rmax, 0, 100);
        display_progress_bar(0, display.height() - 24, display.width(), 24, p);
        break;
    }
  }
  else if (scrollingMode || MTC.getMode() != MidiTimeCode::SynchroNone) {

    bottom.setFreeFont(&FreeSansBold9pt7b);
    bottom.setTextDatum(MC_DATUM);

#ifdef BLE
    if (bleEnabled) {
       if (bleMidiConnected) {
        bottom.fillRect(0, 0, display.width() / 4, 24, TFT_WHITE);
        bottom.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      else {
        bottom.fillRect(0, 0, display.width() / 4, 24, TFT_BLACK);
        bottom.setTextColor(TFT_DARKGREY, TFT_BLACK);
      }
      bottom.drawRoundRect(0, 0, display.width() / 4, 24, 0, TFT_BLACK);
      bottom.drawRoundRect(0, 0, display.width() / 4, 24, 2, TFT_BLACK);
      bottom.drawRoundRect(0, 0, display.width() / 4, 24, 4, TFT_BLACK);
      bottom.drawString("BLE", 1 * display.width() / 8, 11);
    }
#endif

#ifdef WIFI
    if (wifiEnabled) {
      if (appleMidiConnected) {
        bottom.fillRect(1 * display.width() / 4, 0, display.width() / 4, 24, TFT_WHITE);
        bottom.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      else {
        bottom.fillRect(1 * display.width() / 4, 0, display.width() / 4, 24, TFT_BLACK);
        bottom.setTextColor(TFT_DARKGREY, TFT_BLACK);
      }
      bottom.drawRoundRect(1 * display.width() / 4, 0, display.width() / 4, 24, 0, TFT_BLACK);
      bottom.drawRoundRect(1 * display.width() / 4, 0, display.width() / 4, 24, 2, TFT_BLACK);
      bottom.drawRoundRect(1 * display.width() / 4, 0, display.width() / 4, 24, 4, TFT_BLACK);
      bottom.drawString("MIDI",   3 * display.width() / 8, 11);
      if (interfaces[PED_IPMIDI].midiIn || interfaces[PED_IPMIDI].midiOut) {
        bottom.fillRect(2 * display.width() / 4, 0, display.width() / 4, 24, TFT_WHITE);
        bottom.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      else {
        bottom.fillRect(2 * display.width() / 4, 0, display.width() / 4, 24, TFT_BLACK);
        bottom.setTextColor(TFT_DARKGREY, TFT_BLACK);
      }
      bottom.drawRoundRect(2 * display.width() / 4, 0, display.width() / 4, 24, 0, TFT_BLACK);
      bottom.drawRoundRect(2 * display.width() / 4, 0, display.width() / 4, 24, 2, TFT_BLACK);
      bottom.drawRoundRect(2 * display.width() / 4, 0, display.width() / 4, 24, 4, TFT_BLACK);
      bottom.drawString("ipMIDI", 5 * display.width() / 8, 11);

      if (interfaces[PED_OSC].midiIn    || interfaces[PED_OSC].midiOut) {
        bottom.fillRect(3 * display.width() / 4, 0, display.width() / 4, 24, TFT_WHITE);
        bottom.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      else {
        bottom.fillRect(3 * display.width() / 4, 0, display.width() / 4, 24, TFT_BLACK);
        bottom.setTextColor(TFT_DARKGREY, TFT_BLACK);
      }
      bottom.drawRoundRect(3 * display.width() / 4, 0, display.width() / 4, 24, 0, TFT_BLACK);
      bottom.drawRoundRect(3 * display.width() / 4, 0, display.width() / 4, 24, 2, TFT_BLACK);
      bottom.drawRoundRect(3 * display.width() / 4, 0, display.width() / 4, 24, 4, TFT_BLACK);
      bottom.drawString("OSC", 7 * display.width() / 8, 11);
    }
#endif
    bottom.pushSprite(0, display.height() - 24);
  }

  //bottom.pushSprite(0, display.height() - 24);
}

void drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  display.drawLine(x0+1, y0,   x1-1, y0,   TFT_WHITE);
  display.drawLine(x1,   y0+1, x1,   y1-1, TFT_WHITE);
  display.drawLine(x1-1, y1,   x0+1, y1,   TFT_WHITE);
  display.drawLine(x0,   y1-1, x0,   y0+1, TFT_WHITE);
}

void drawFrame1(int16_t x, int16_t y)
{
  if (millis() < endMillis2 && lastPedalName[0] != ':') {

    //ui.disableAutoTransition();
    //ui.switchToFrame(0);
    if (strlen(lastPedalName) != 0 && lastPedalName[strlen(lastPedalName) - 1] == '.') lastPedalName[strlen(lastPedalName) - 1] = 0;
    if (lastPedalName[0] == 0) {
      TFT_eSprite sprite = TFT_eSprite(&display);
      sprite.setColorDepth(8);
      sprite.createSprite(100, 74);
      sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
      sprite.pushSprite(0, 30);
      sprite.pushSprite(display.width() - sprite.width(), 30);
      sprite.setTextColor(TFT_WHITE, TFT_BLACK);
      switch (m1) {
        case midi::InvalidType:
          display.drawRect(display.width() / 2 - 22, 30, display.width() / 2 + 24, 30 + 23, TFT_WHITE);
          display.setFreeFont(&FreeSans9pt7b);
          display.drawString("Bank", display.width() / 2 + x, 39 + y);
          display.setFreeFont(&FreeSans24pt7b);
          display.drawString(String(m2), display.width() / 2 + x, 14 + y);
          break;
        case midi::NoteOn:
        case midi::NoteOff:
          sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
          sprite.setFreeFont(&FreeSans9pt7b);
          sprite.setTextDatum(MC_DATUM);
          sprite.drawString("Note", sprite.width() / 2 + x, 62 + y);
          sprite.setFreeFont(&FreeSans24pt7b);
          sprite.drawString(String(m2), sprite.width() / 2 + x, 22 + y);
          sprite.drawRoundRect(0, 0, 100, 50, 8, TFT_WHITE);
          sprite.pushSprite(display.width() / 2 - 50, 30);
          sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
          sprite.setFreeFont(&FreeSans9pt7b);
          sprite.setTextDatum(MR_DATUM);
          sprite.drawString("Velocity", sprite.width() + x, 62 + y);
          sprite.setFreeFont(&FreeSans18pt7b);
          sprite.setTextDatum(MC_DATUM);
          sprite.drawString(String(m3), sprite.width() - display.width() / 8 + x, 22 + y);
          sprite.pushSprite(display.width() - sprite.width(), 30, TFT_BLACK);
          break;
        case midi::ControlChange:
          sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
          sprite.setFreeFont(&FreeSans9pt7b);
          sprite.setTextDatum(MC_DATUM);
          sprite.drawString("CC", sprite.width() / 2 + x, 62 + y);
          sprite.setFreeFont(&FreeSans24pt7b);
          sprite.drawString(String(m2), sprite.width() / 2 + x, 22 + y);
          sprite.drawRoundRect(0, 0, 100, 50, 8, TFT_WHITE);
          sprite.pushSprite(display.width() / 2 - 50, 30);
          sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
          sprite.setFreeFont(&FreeSans9pt7b);
          sprite.setTextDatum(MR_DATUM);
          sprite.drawString("Value", sprite.width() + x, 62 + y);
          sprite.setFreeFont(&FreeSans18pt7b);
          sprite.setTextDatum(MC_DATUM);
          sprite.drawString(String(m3), sprite.width() - display.width() / 8 + x, 22 + y);
          sprite.pushSprite(display.width() - sprite.width(), 30, TFT_BLACK);
          break;
        case midi::ProgramChange:
          sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
          sprite.setFreeFont(&FreeSans9pt7b);
          sprite.setTextDatum(MC_DATUM);
          sprite.drawString("PC", sprite.width() / 2 + x, 62 + y);
          sprite.setFreeFont(&FreeSans24pt7b);
          sprite.drawString(String(m2), sprite.width() / 2 + x, 22 + y);
          sprite.drawRoundRect(0, 0, 100, 50, 8, TFT_WHITE);
          sprite.pushSprite(display.width() / 2 - 50, 30);
          break;
        case midi::PitchBend:
sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
          sprite.setFreeFont(&FreeSans9pt7b);
          sprite.setTextDatum(MC_DATUM);
          sprite.drawString("Pitch", sprite.width() / 2 + x, 62 + y);
          sprite.setFreeFont(&FreeSans24pt7b);
          sprite.drawString(String(((m3 << 7) | m2) + MIDI_PITCHBEND_MIN), sprite.width() / 2 + x, 22 + y);
          sprite.drawRoundRect(0, 0, 100, 50, 8, TFT_WHITE);
          sprite.pushSprite(display.width() / 2 - 50, 30);
          break;
        case midi::AfterTouchChannel:
          sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
          sprite.setFreeFont(&FreeSans9pt7b);
          sprite.setTextDatum(MC_DATUM);
          sprite.drawString("Pressure", sprite.width() / 2 + x, 62 + y);
          sprite.setFreeFont(&FreeSans24pt7b);
          sprite.drawString(String(m2), sprite.width() / 2 + x, 22 + y);
          sprite.drawRoundRect(0, 0, 100, 50, 8, TFT_WHITE);
          sprite.pushSprite(display.width() / 2 - 50, 30);
          break;
      }
      if (m1 != midi::InvalidType) {
        sprite.fillRect(0, 0, sprite.width(), sprite.height(), TFT_BLACK);
        sprite.setFreeFont(&FreeSans9pt7b);
        sprite.setTextDatum(ML_DATUM);
        sprite.drawString("Channel", 0 + x, 62 + y);
        sprite.setFreeFont(&FreeSans18pt7b);
        sprite.setTextDatum(MC_DATUM);
        sprite.drawString(String(m4), display.width() / 8 + x, 22 + y);
        sprite.pushSprite(0, 30, TFT_BLACK);
      }
      sprite.deleteSprite();
    }
    else {
      String name = lastPedalName;
      switch (m1) {
        case midi::InvalidType:
          display.drawRect(64-22, 15, 64+24, 15+23, TFT_WHITE);
          display.setTextDatum(TC_DATUM);
          display.setFreeFont(&FreeSans9pt7b);
          display.drawString("Bank", 64 + x, 39 + y);
          display.setFreeFont(&FreeSans24pt7b);
          display.drawString(String(m2), 64 + x, 14 + y);
          break;
        case midi::NoteOn:
        case midi::NoteOff:
        case midi::ControlChange:
          name.replace(String("###"), String(m3));
        case midi::ProgramChange:
        case midi::AfterTouchChannel:
          name.replace(String("###"), String(m2));
        default:
          display.setTextDatum(MC_DATUM);
          display.setFreeFont(&FreeSans24pt7b);
          display.drawString(name, 64, 32);
          break;
      }
    }
  }
  else if (MTC.getMode() == MidiTimeCode::SynchroClockMaster ||
           MTC.getMode() == MidiTimeCode::SynchroClockSlave) {
    /*
    display->setFont(ArialMT_Plain_24);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(22 + x, 12 + y, String(bpm));
    display->setFont(ArialMT_Plain_10);
    display->drawString(22 + x, 36 + y, "BPM");
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    switch (timeSignature) {
      case PED_TIMESIGNATURE_2_4:
        display->drawString(128 + x, 36 + y, "2/4");
        break;
      case PED_TIMESIGNATURE_4_4:
        display->drawString(128 + x, 36 + y, "4/4");
        break;
      case PED_TIMESIGNATURE_3_4:
        display->drawString(128 + x, 36 + y, "3/4");
        break;
      case PED_TIMESIGNATURE_3_8:
        display->drawString(128 + x, 36 + y, "3/8");
        break;
      case PED_TIMESIGNATURE_6_8:
        display->drawString(128 + x, 36 + y, "6/8");
        break;
      case PED_TIMESIGNATURE_9_8:
        display->drawString(128 + x, 36 + y, "9/8");
        break;
      case PED_TIMESIGNATURE_12_8:
        display->drawString(128 + x, 36 + y, "12/8");
        break;
    }
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    if (MTC.getMode() == MidiTimeCode::SynchroClockMaster)
      display->drawString(68 + x, 36 + y, "Master");
    else if (MTC.getMode() == MidiTimeCode::SynchroClockSlave)
      display->drawString(68 + x, 36 + y, "Slave");

    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(block);
    switch (timeSignature) {
      case PED_TIMESIGNATURE_2_4:
        display->drawString( 68 + x, 16 + x, String(0));
        display->drawString( 88 + x, 16 + x, String(0));
        break;
      case PED_TIMESIGNATURE_4_4:
        display->drawString( 68 + x, 16 + x, String(0));
        display->drawString( 88 + x, 16 + x, String(0));
        display->drawString(108 + x, 16 + x, String(0));
        display->drawString(128 + x, 16 + x, String(0));
        break;
      case PED_TIMESIGNATURE_3_4:
      case PED_TIMESIGNATURE_3_8:
      case PED_TIMESIGNATURE_6_8:
      case PED_TIMESIGNATURE_9_8:
      case PED_TIMESIGNATURE_12_8:
        display->drawString( 68 + x, 16 + x, String(0));
        display->drawString( 88 + x, 16 + x, String(0));
        display->drawString(108 + x, 16 + x, String(0));
        break;
    }
    switch (MTC.getBeat()) {
      case 0:
        if (MTC.isPlaying())
          display->drawString( 68 + x, 16 + x, String(2));
        else
          display->drawString( 68 + x, 16 + x, String(1));
        break;
      case 1:
        if (MTC.isPlaying())
          display->drawString( 88 + x, 16 + x, String(2));
        else
          display->drawString( 88 + x, 16 + x, String(1));
        break;
      case 2:
        if (MTC.isPlaying())
          display->drawString(108 + x, 16 + x, String(2));
        else
          display->drawString(108 + x, 16 + x, String(1));
        break;
      case 3:
        if (MTC.isPlaying())
          display->drawString(128 + x, 16 + x, String(2));
        else
          display->drawString(128 + x, 16 + x, String(1));
        break;
    }
    ui.disableAutoTransition();
    */
  }
  else if (MTC.getMode() == MidiTimeCode::SynchroMTCMaster ||
           MTC.getMode() == MidiTimeCode::SynchroMTCSlave) {
    /*
    char buf[12];
    sprintf(buf, "%02d:%02d:%02d:%02d", MTC.getHours(), MTC.getMinutes(), MTC.getSeconds(), MTC.getFrames());
    display->setFont(ArialMT_Plain_24);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawString(64 + x, 32 + y, buf);
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    if (MTC.getMode() == MidiTimeCode::SynchroMTCMaster)
      display->drawString(128 + x, 0 + y, "Master");
    else if (MTC.getMode() == MidiTimeCode::SynchroMTCSlave)
      display->drawString(128 + x, 0 + y, "Slave");
    ui.disableAutoTransition();
    */
  }
  else {
    if (scrollingMode) {
      display.setTextColor(TFT_WHITE, TFT_BLACK);
      display.setFreeFont(&FreeSansBold12pt7b);
      display.setTextDatum(MC_DATUM);
      display.drawString(MODEL, display.width() / 2 + x, display.height() / 2 + y);
      display.setTextFont(1);
      display.setTextDatum(BL_DATUM);
      display.drawString("TM", display.width() - 40 + x, display.height() / 2 - 5 + y);
      //ui.enableAutoTransition();
    }
    else {
      if (banknames[currentBank][0] == 0) {
        TFT_eSprite sprite = TFT_eSprite(&display);

        String a;
        a = (currentProfile == 0 ? "A" : (currentProfile == 1 ? "B" : "C"));
        a += ".";
        a += ((currentBank >= 9  ? "" : "0") + String(currentBank + 1));
        display.setFreeFont(&DSEG14_Classic_Bold_72);
        int w = display.textWidth(a);
        sprite.setColorDepth(8);
        sprite.createSprite(w, DSEG14_Classic_Bold_72.yAdvance);
        sprite.setFreeFont(&DSEG14_Classic_Bold_72);
        switch (currentProfile) {
          case 0:
            sprite.setTextColor(TFT_RED, TFT_BLACK);
            break;
          case 1:
            sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
            break;
          case 2:
            sprite.setTextColor(TFT_BLUE, TFT_BLACK);
            break;
          default:
            sprite.setTextColor(TFT_WHITE, TFT_BLACK);
            break;
        }
        sprite.setTextDatum(MC_DATUM);
        sprite.drawString(a, sprite.width() / 2, sprite.height() / 2);
        sprite.pushSprite((display.width() - w) / 2 + x, 30 + y);
        sprite.deleteSprite();
      }
      else {
        /*
        String name;
        int offsetText       = 0;
        int offsetBackground = 0;
        static unsigned long ms = millis();

        // Display pedals name
        display->setFont(ArialMT_Plain_10);
        for (byte p = 0; p < PEDALS/2; p++) {
          switch (p) {
            case 0:
              display->setTextAlignment(TEXT_ALIGN_LEFT);
              offsetText = 1;
              offsetBackground = 0;
              break;
            case PEDALS / 2 - 1:
              display->setTextAlignment(TEXT_ALIGN_RIGHT);
              offsetText = -1;
              offsetBackground = 2;
              break;
            default:
              display->setTextAlignment(TEXT_ALIGN_CENTER);
              offsetText = 0;
              offsetBackground = 1;
              break;
          }
          // Top line
          name = String((banks[currentBank][p].pedalName[0] == ':') ? &banks[currentBank][p].pedalName[1] : banks[currentBank][p].pedalName);
          name.replace(String("###"), String(currentMIDIValue[currentBank][p][0]));
          if (pedals[p].function == PED_MIDI && currentMIDIValue[currentBank][p][0] == banks[currentBank][p].midiValue2) {
            display->fillRect((128 / (PEDALS / 2 - 1)) * p - offsetBackground * display->getStringWidth(name) / 2 + offsetText + x,
                              12 + y,
                              display->getStringWidth(name) + 1,
                              10);
            display->setColor(BLACK);
          }
          else
            display->setColor(WHITE);
          display->drawString((128 / (PEDALS / 2 - 1)) * p + offsetText + x, 10 + y, name);
          // Bottom line
          name = String((banks[currentBank][p + PEDALS / 2].pedalName[0] == ':') ? &banks[currentBank][p + PEDALS / 2].pedalName[1] : banks[currentBank][p + PEDALS / 2].pedalName);
          name.replace(String("###"), String(currentMIDIValue[currentBank][p + PEDALS / 2][0]));
          if (pedals[p + PEDALS / 2].function == PED_MIDI && currentMIDIValue[currentBank][p + PEDALS / 2][0] == banks[currentBank][p + PEDALS / 2].midiValue2) {
            display->fillRect((128 / (PEDALS / 2 - 1)) * p - offsetBackground * display->getStringWidth(name) / 2 + offsetText + x,
                              53 + y,
                              display->getStringWidth(name) + 1,
                              10);
            display->setColor(BLACK);
          }
          else
            display->setColor(WHITE);
          display->drawString((128 / (PEDALS / 2 - 1)) * p + offsetText + x, 51 + y, name);
          display->setColor(WHITE);
        }
        // Center area
        if (((millis() - ms < 4000) && (banknames[currentBank][0] != '.')) || (banknames[currentBank][0] == ':')) {
          // Display bank name
          display->drawRect(0, 23, 128, 29);
          name = (banknames[currentBank][0] == ':') ? &banknames[currentBank][1] : banknames[currentBank];
          name.replace(String("##"), String(currentBank));
          display->setFont(ArialMT_Plain_24);
          display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
          display->drawString( 64 + x, 37 + y, name);
        }
        else if (((millis() - ms < 8000) || (banknames[currentBank][0] == '.')) && (banknames[currentBank][0] != ':')) {
          // Display pedal values
          name = (banknames[currentBank][0] == '.') ? &banknames[currentBank][1] : banknames[currentBank];
          name.replace(String("##"), String(currentBank));
          display->setFont(ArialMT_Plain_10);
          display->setTextAlignment(TEXT_ALIGN_RIGHT);
          display->drawString(128 + x, y, name);
          for (byte p = 0; p < PEDALS/2; p++) {
            if ((pedals[p].function == PED_MIDI) && (banks[currentBank][p].midiMessage != PED_EMPTY)) {
              display->drawProgressBar((128 / (PEDALS / 2)) * p + 2 + x, 25 + y, 39, 11, constrain(map(currentMIDIValue[currentBank][p][0],
                                                                                                       banks[currentBank][p].midiValue1,
                                                                                                       banks[currentBank][p].midiValue2,
                                                                                                       0, 100),
                                                                                                   0, 100));
            }
            if ((pedals[p + PEDALS / 2].function == PED_MIDI) && (banks[currentBank][p + PEDALS / 2].midiMessage != PED_EMPTY)) {
              display->drawProgressBar((128 / (PEDALS / 2)) * p + 2 + x, 39 + y, 39, 11, constrain(map(currentMIDIValue[currentBank][p + PEDALS / 2][0],
                                                                                                       banks[currentBank][p + PEDALS / 2].midiValue1,
                                                                                                       banks[currentBank][p + PEDALS / 2].midiValue2,
                                                                                                       0, 100),
                                                                                                   0, 100));
            }
          }
        }
        else {
          ms = millis();
        }
        */
      }
      //ui.disableAutoTransition();
    }
  }
/*
#ifdef WEBSOCKET
  events.send(MTC.isPlaying() ? "1" : "0", "play");

  if (MTC.getMode() == MidiTimeCode::SynchroClockMaster ||
      MTC.getMode() == MidiTimeCode::SynchroClockSlave)  {
    char buf[4];
    events.send("", "mtc");
    sprintf(buf, "%3d", bpm);
    events.send(buf, "bpm");
    sprintf(buf, "%d", MTC.getBeat() + 1);
    events.send(buf, "beat");
    switch (timeSignature) {
      case PED_TIMESIGNATURE_2_4:
        events.send("2/4", "timesignature");
        break;
      case PED_TIMESIGNATURE_4_4:
        events.send("4/4", "timesignature");
        break;
      case PED_TIMESIGNATURE_3_4:
        events.send("3/4", "timesignature");
        break;
      case PED_TIMESIGNATURE_3_8:
        events.send("3/8", "timesignature");
        break;
      case PED_TIMESIGNATURE_6_8:
        events.send("6/8", "timesignature");
        break;
      case PED_TIMESIGNATURE_9_8:
        events.send("9/8", "timesignature");
        break;
      case PED_TIMESIGNATURE_12_8:
        events.send("12/8", "timesignature");
        break;
    }
  }

  if (MTC.getMode() == MidiTimeCode::SynchroMTCMaster ||
      MTC.getMode() == MidiTimeCode::SynchroMTCSlave) {
    char buf[12];
    sprintf(buf, "%02d:%02d:%02d:%02d", MTC.getHours(), MTC.getMinutes(), MTC.getSeconds(), MTC.getFrames());
    events.send(buf, "mtc");
    events.send("", "bpm");
    events.send("", "beat");
    events.send("", "timesignature");
  }
#endif
  */
}

void drawFrame2(int16_t x, int16_t y)
{
  if (!scrollingMode || MTC.isPlaying() || MTC.getMode() != PED_MTC_NONE || millis() < endMillis2) return;
    //ui.switchToFrame(0);

  display.fillRect(0, 24, display.width(), display.height() - 48, TFT_BLACK);
  display.setFreeFont(&FreeSansBold12pt7b);
  display.setTextColor(TFT_DARKGREY, TFT_BLACK);
  display.setTextDatum(TL_DATUM);
  display.drawString("Device:", 0 + x, 30 + y);
  display.setFreeFont(&FreeSans12pt7b);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextDatum(TR_DATUM);
  display.drawString(host, display.width() + x, 30 + y);
#ifdef WIFI
  switch (WiFi.getMode()) {
    case WIFI_AP:
    case WIFI_AP_STA:
      display.setFreeFont(&FreeSansBold12pt7b);
      display.setTextColor(TFT_DARKGREY, TFT_BLACK);
      display.setTextDatum(TL_DATUM);
      display.drawString( "AP:", 0 + x, 30 + y);
      display.setFreeFont(&FreeSans12pt7b);
      display.setTextColor(TFT_WHITE, TFT_BLACK);
      display.setTextDatum(TR_DATUM);
      display.drawString(ssidSoftAP, display.width() + x, 30 + y);
      display.setFreeFont(&FreeSansBold12pt7b);
      display.setTextColor(TFT_DARKGREY, TFT_BLACK);
      display.setTextDatum(TL_DATUM);
      display.drawString("AP IP:", 0 + x, 58 + y);
      display.setFreeFont(&FreeSans12pt7b);
      display.setTextColor(TFT_WHITE, TFT_BLACK);
      display.setTextDatum(TR_DATUM);
      display.drawString(WiFi.softAPIP().toString(), display.width() + x, 58 + y);
      break;
    case WIFI_STA:
      display.setFreeFont(&FreeSansBold12pt7b);
      display.setTextColor(TFT_DARKGREY, TFT_BLACK);
      display.setTextDatum(TL_DATUM);
      display.drawString("SSID:", 0 + x, 58 + y);
      display.setFreeFont(&FreeSans12pt7b);
      display.setTextColor(TFT_WHITE, TFT_BLACK);
      display.setTextDatum(TR_DATUM);
      display.drawString(wifiSSID, display.width() + x, 58 + y);
      display.setFreeFont(&FreeSansBold12pt7b);
      display.setTextColor(TFT_DARKGREY, TFT_BLACK);
      display.setTextDatum(TL_DATUM);
      display.drawString("IP:", 0 + x, 86 + y);
      display.setTextColor(TFT_WHITE, TFT_BLACK);
      display.setTextDatum(TR_DATUM);
      display.setFreeFont(&FreeSans12pt7b);
      display.drawString(WiFi.localIP().toString(), display.width() + x, 86 + y);
      break;
    case WIFI_MODE_MAX:
    case WIFI_MODE_NULL:
      break;
  }
#endif
}

void drawFrame3(int16_t x, int16_t y)
{
  if (!scrollingMode || MTC.isPlaying() || MTC.getMode() != PED_MTC_NONE || millis() < endMillis2) return;
    //ui.switchToFrame(0);

  display.fillRect(0, 24, display.width(), display.height() - 48, TFT_BLACK);
  display.setFreeFont(&FreeSansBold9pt7b);
  display.setTextColor(TFT_DARKGREY, TFT_BLACK);
  display.setTextDatum(TL_DATUM);
  display.drawString("Free heap:", 0 + x, 30 + y);
  display.setFreeFont(&FreeSans12pt7b);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextDatum(TR_DATUM);
  display.drawString(ESP.getFreeHeap()/1024 + String(" Kb"), display.width() + x, 30 + y);

#ifdef BATTERY
  display.setFreeFont(&FreeSansBold9pt7b);
  display.setTextColor(TFT_DARKGREY, TFT_BLACK);
  display.setTextDatum(TL_DATUM);
  display.drawString("Battery:", 0 + x, 58 + y);
  display.setFreeFont(&FreeSans12pt7b);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextDatum(TR_DATUM);
  display.drawString(batteryVoltage / 1000.0F + String(" mV"), display.width()+ x, 58 + y);
#endif

  display.setFreeFont(&FreeSansBold9pt7b);
  display.setTextColor(TFT_DARKGREY, TFT_BLACK);
  display.setTextDatum(TL_DATUM);
  display.drawString("Run time:", 0 + x, 86 + y);
  display.setFreeFont(&FreeSans12pt7b);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextDatum(TR_DATUM);
  long sec = (millis() / 1000) % 60;
  long min = (millis() / 1000 / 60) % 60;
  long h   = (millis() / 1000 / 3600);
  display.drawString(h + String("h ") + min + String("m ") + sec + String("s"), display.width() + x, 86 + y);
}


void display_init()
{
    display.init();
    display.setRotation(1);
    display.fillScreen(TFT_BLACK);
    /*
    if (TFT_BL > 0) {                           // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
        pinMode(TFT_BL, OUTPUT);                // Set backlight pin to output mode
        digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }
    */

    display.fillScreen(TFT_WHITE);
    display.setSwapBytes(true);
    display.pushImage((display.width() - PEDALINO_LOGO_WIDTH) / 2, (display.height() - PEDALINO_LOGO_HEIGHT) / 2, PEDALINO_LOGO_WIDTH, PEDALINO_LOGO_HEIGHT, PedalinoLogo);
    delay(1000);

#ifdef WIFI
  if (wifiEnabled) {
    display.fillScreen(TFT_WHITE);
    display.setSwapBytes(true);
    display.pushImage((display.width() - WIFI_LOGO_WIDTH) / 2, (display.height() - WIFI_LOGO_HEIGHT) / 2, WIFI_LOGO_WIDTH, WIFI_LOGO_HEIGHT, WiFiLogo);
    delay(1000);
  }
#endif

#ifdef BLE
  if (bleEnabled) {
    display.fillScreen(TFT_WHITE);
    display.setSwapBytes(true);
    display.pushImage((display.width() - BLUETOOTH_LOGO_WIDTH) / 2, (display.height() - BLUETOOTH_LOGO_HEIGHT) / 2, BLUETOOTH_LOGO_WIDTH, BLUETOOTH_LOGO_HEIGHT, BluetoothLogo);
    delay(1000);
  }
#endif

    display.fillScreen(TFT_BLACK);
}

void display_ui_update_disable()
{
  uiUpdate = false;
}

void display_ui_update_enable()
{
  uiUpdate = true;
}

void display_update()
{
  static byte frame = 8;

  if (uiUpdate) {
    if (millis() % 128 == 0) {
      topOverlay();
      bottomOverlay();
      if (scrollingMode) {
        frame = (frame + 1) % 9;
        switch (frame) {
          case 0:
            drawFrame1(0, 0);
            break;
          case 3:
            drawFrame2(0, 0);
            break;
          case 6:
#ifdef DIAGNOSTIC
            drawFrame3(0, 0);
#else
            drawFrame2(0, 0);
#endif
            break;
        }
      }
      else {
        drawFrame1(0, 0);
        frame = 8;
      }
    }
  }
}

void display_off()
{
  digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);

  display.writecommand(TFT_DISPOFF);
  display.writecommand(TFT_SLPIN);
}

