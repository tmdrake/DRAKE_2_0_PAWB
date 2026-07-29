/*
 * Used Arduino Pro Mini at 5V level 16MHZ
 * Updated July 2026: modes 0-10 follow Tail/Head
 */
#if !defined(__AVR_ATmega328P__)
#error This code is designed to run on Sparkfun's Pro Mini at 5V level 16MHZ (Sparkfun boards).
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
  Serial.println(__FILE__);
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  Serial.println("Drake's PAWB's...GO! (modes 0-10)");

  pinMode(LED_BUILTIN, OUTPUT);

  spikes.begin();
  spikes.show();

  if (!driver.init()) {
    Serial.println("ASK init failed.");
  }

  mode = EEPROM.read(MODE_ADDR);
  if (mode < 0 || mode > 10) {
    mode = 0;
  }
  Serial.print("Mode=");
  Serial.println(mode);
}

void loop() {
  checkASK();

  if (Serial.available() > 0) {
    int inByte = Serial.read();
    if (inByte == 'R') {
      resetfading();
    } else if (inByte == 'L') {
      flash_lamp();
    }
  }

  if (!flashed) {
    sound_detect();
  }

  t.update();
}

void sound_detect() {
  // Visual modes 3-10 run continuously (match Tail behaviour)
  if (mode >= 3 && mode <= 10) {
    mode_selector(mode);
    return;
  }

  // Modes 0-2: sound-reactive when mic is hot, otherwise idle fade
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
      // Tail sends "M0".."M9" or "MA" for mode 10
      int newMode = 0;
      if (buflen >= 2) {
        char d = (char)buf[1];
        if (d >= '0' && d <= '9')
          newMode = d - '0';
        else if (d == 'A' || d == 'a')
          newMode = 10;
        else {
          // fallback parse
          buf[buflen] = '\0';
          newMode = atoi((char*)buf + 1);
        }
      }
      if (newMode < 0) newMode = 0;
      if (newMode > 10) newMode = 10;
      mode = newMode;
      EEPROM.write(MODE_ADDR, (byte)mode);
      resetPawModeState();
      Serial.print("Mode=");
      Serial.println(mode);
    } else if (cmd == 'm') {
      // Mic value: "m1234"
      buf[0] = '0';
      buf[buflen] = '\0';
      micLevel = atol((char*)buf);
    }

    digitalWrite(LED_BUILTIN, LOW);
  }
}

void flash_lamp() {
  turn_all_on();
  digitalWrite(LED_BUILTIN, HIGH);
  flashed = true;
  t.after(100, turn_all_off);
}

void turn_all_off() {
  for (uint16_t i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, spikes.Color(0, 0, 0));
  spikes.show();
  flashed = false;
  digitalWrite(LED_BUILTIN, LOW);
}

void turn_all_on() {
  for (uint16_t i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, spikes.Color(150, 150, 150));
  spikes.show();
}
