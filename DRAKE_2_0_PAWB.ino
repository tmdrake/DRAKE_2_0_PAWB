/*
 * PAWB Pro Mini – modes 0-10 + solid color C via ASK
 */
#if !defined(__AVR_ATmega328P__)
#error Pro Mini 5V 16MHz required.
#endif

#include <EEPROM.h>
const int MODE_ADDR = 0;

#include <Adafruit_NeoPixel.h>
const int neopixel_pin = 8;
Adafruit_NeoPixel spikes = Adafruit_NeoPixel(5, neopixel_pin, NEO_GRB + NEO_KHZ800);

#include <Timer.h>
Timer t;

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h>
#endif
RH_ASK driver(2000, A0, 0, 0);

bool flashed = false;
long micLevel = 0;
bool soundmode = false;
bool enableSound = true;
int mode = 0;
unsigned long lastime = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Drake's PAWB's...GO! (modes 0-10 + C)");

  pinMode(LED_BUILTIN, OUTPUT);
  spikes.begin();
  spikes.show();

  if (!driver.init()) Serial.println("ASK init failed.");

  mode = EEPROM.read(MODE_ADDR);
  if (mode < 0 || mode > 10) mode = 0;
}

void loop() {
  checkASK();
  if (!flashed) sound_detect();
  t.update();
}

void sound_detect() {
  if (mode >= 3 && mode <= 10) {
    mode_selector(mode);
    return;
  }
  if (soundmode && enableSound) {
    mode_selector(mode);
    if (millis() - lastime > 10000) {
      soundmode = false;
      resetBrightnessandDirection();
    }
  } else {
    fading();
  }
  if (micLevel > 100) {
    soundmode = true;
    lastime = millis();
  }
}

void checkASK() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    digitalWrite(LED_BUILTIN, HIGH);
    char cmd = (char)buf[0];

    if (cmd == 'R') {
      resetfading();
      resetPawModeState();
    } else if (cmd == 'L') {
      flash_lamp();
    } else if (cmd == 'M') {
      int newMode = 0;
      if (buflen >= 2) {
        char d = (char)buf[1];
        if (d >= '0' && d <= '9') newMode = d - '0';
        else if (d == 'A' || d == 'a') newMode = 10;
      }
      if (newMode < 0) newMode = 0;
      if (newMode > 10) newMode = 10;
      mode = newMode;
      EEPROM.write(MODE_ADDR, (byte)mode);
      resetPawModeState();
    } else if (cmd == 'C') {
      buf[buflen] = 0;
      int r = 0, g = 0, b = 0;
      if (sscanf((char*)buf + 1, "%d,%d,%d", &r, &g, &b) == 3) {
        if (r < 0) r = 0; if (r > 255) r = 255;
        if (g < 0) g = 0; if (g > 255) g = 255;
        if (b < 0) b = 0; if (b > 255) b = 255;
        setSolidColor((uint8_t)r, (uint8_t)g, (uint8_t)b);
        mode = 9;
        EEPROM.write(MODE_ADDR, 9);
        resetPawModeState();
      }
    } else if (cmd == 'm') {
      buf[0] = '0';
      buf[buflen] = 0;
      micLevel = atol((char*)buf);
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void flash_lamp() {
  for (uint16_t i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, spikes.Color(150, 150, 150));
  spikes.show();
  flashed = true;
  t.after(100, turn_all_off);
}

void turn_all_off() {
  for (uint16_t i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, 0);
  spikes.show();
  flashed = false;
}
