/*
 * Used Arduino Pro Mini at 5V level 16MHZ
 * 1-5-2025 
 */
#if !defined(__AVR_ATmega328P__)
//#if !defined(ARDUINO_AVR_PROMICRO)
#error This code is designed to run on Sparkfun's Pro Mini at 5V level 16MHZ (Sparkfun boards).
#endif

//Other pin Def
//const int led_pin = 13;
#include <EEPROM.h>
const int MODE_ADDR = 0;


//Claws pixel
#include <Adafruit_NeoPixel.h>
const int neopixel_pin = 8;
Adafruit_NeoPixel spikes = Adafruit_NeoPixel(5, neopixel_pin, NEO_GRB + NEO_KHZ800);

//timer setup
#include <Timer.h>
Timer t;

//Load ASK Reciever to Sync against
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h>  // Not actually used but needed to compile
#endif
RH_ASK driver(2000, A0, 0, 0);

/*Some global varables*/
bool flashed = false;
long micLevel = 0;
bool soundmode = false;
bool enableSound = true;
int mode = 0;
unsigned long lastime = 0;
/********************/

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(__FILE__);
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  Serial.println("Drake's PAWB's...GO!");

  pinMode(LED_BUILTIN, OUTPUT);  //Built in LED pin

  spikes.begin();
  //spikes.setBrightness(100);
  spikes.show();  // Initialize all pixels to 'off'


#ifdef RH_HAVE_SERIAL
  Serial.begin(115200);  // Debugging only
#endif
  if (!driver.init())
#ifdef RH_HAVE_SERIAL
    Serial.println("init failed.");
#else
    ;
#endif
  /*Default mode that isn't saved*/
  mode = EEPROM.read(MODE_ADDR);
  if (mode < 0 || mode > 1) {
    mode = 0;
  }
}

void loop() {
  /*Check ask reciever*/
  checkASK();

  //For Direct Serial Command
  if (Serial.available() > 0) {
    int inByte = Serial.read();

    if (inByte == 'R') {
      resetfading();
    } else if (inByte == 'L') {
      flash_lamp();
    } else {
      Serial.println("R/L-Commands!");
    }
  }

  //Fading or flashed
  if (!flashed) {
    //fading();
    sound_detect();
  }

  //Update Timer
  t.update();
  ////////////////
}

void sound_detect() {
  if (soundmode && enableSound) {

    if (mode == 1) {  //MO to M1 selection
      //soundloop(millis(), 50, true, 100);  //false = color phase, true = distinct colors
      soundloop(millis(), 50, true, micLevel);  //false = color phase, true = distinct colors
    } else {
      //soundloop(millis(), 50, false, 100);  //false = color phase, true = distinct colors
      soundloop(millis(), 50, false, micLevel);  //false = color phase, true = distinct colors
    }


    //mode_selector(mode);

    //digitalWrite(LED_BUILTIN, HIGH);
    if (millis() - lastime > 10000) {
      soundmode = false;  //put the system back into fading mode, after 10 seconds.
      resetBrightnessandDirection();
    }
  } else {


    fading();
    //digitalWrite(LED_BUILTIN, LOW);
    //lastmiclevel = 0;
  }
  ///Check for Sound!
  //long micLevel = analogRead(MIC) - OFFSET;  //adafruit offset
  //Serial.println(micLevel);
  //delay(10);
  if (micLevel > 100 /*TRIGGER SENSITIVITY*/) {
    soundmode = true;
    lastime = millis();  //reset our timeout
  }
}

void checkASK() {
  /*Check ask reciever for messages and process them*/
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen))  // Non-blocking
  {
    //int i;
    //Serial.write(buf, buflen);
    //Serial.println();
    // Message with a good checksum received, dump it.
    digitalWrite(LED_BUILTIN, HIGH);
    //Serial.print((char)buf[0]);
    //Serial.println((char)buf[1]);

    if ((char)buf[0] == 'R') {
      resetfading();
    } else if ((char)buf[0] == 'L') {
      flash_lamp();
    } else if ((char)buf[0] == 'M') {
      buf[0] = '0';
      buf[buflen] = '\0';
      mode = atoi(buf);
      EEPROM.write(MODE_ADDR, (byte)mode);  //Save for next startup
    } else if ((char)buf[0] == 'm') {       //Lowercase M
      /*Decoding Mic values*/
      buf[0] = '0';
      buf[buflen] = '\0';
      micLevel = atol(buf);  //Skipping the first Charactor and the lenth
      //Serial.print("m"); Serial.println(micLevel);
      //Serial.print((char*)buf);
      //Serial.println();
    }
    digitalWrite(LED_BUILTIN, LOW);
    //driver.printBuffer("Got:", buf, buflen);
  }
}

void flash_lamp() {
  turn_all_on();
  digitalWrite(LED_BUILTIN, HIGH);
  flashed = true;  //Nonblocking
  t.after(100, turn_all_off);

  //delay(100); //Need to do this without delay
  //turn_all_off();
}

//Below are routines to save code
void turn_all_off() {
  //todo: change to begin to reset
  for (uint16_t i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, spikes.Color(0, 0, 0));
  spikes.show();
  flashed = false;  //non blocking reset
  digitalWrite(LED_BUILTIN, LOW);
}

void turn_all_on() {
  //todo: change to begin to reset
  for (uint16_t i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, spikes.Color(150, 150, 150));
  spikes.show();
}
