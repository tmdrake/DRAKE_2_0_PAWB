/*
 * Used Arduino Pro Mini at 5V level 16MHZ
 *
 */
//#if !defined(__AVR_ATmega328P__)
#if !defined(ARDUINO_AVR_PROMICRO)
#error This code is designed to run on Sparkfun's Pro Mini at 5V level 16MHZ (Sparkfun boards).
#endif

//Other pin Def
//const int led_pin = 13;

//Claws pixel
#include <Adafruit_NeoPixel.h>
const int neopixel_pin = 8;
Adafruit_NeoPixel spikes = Adafruit_NeoPixel(4, neopixel_pin, NEO_GRB + NEO_KHZ800);

//timer setup
#include <Timer.h>
Timer t;

//Load ASK Reciever to Sync against
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
RH_ASK driver(2000, A0, 0, 0);

bool flashed = false;

void setup() {
  Serial.begin(115200);
  Serial.println(F(__FILE__));
  Serial.println(F(__DATE__));
  Serial.println(F(__TIME__));
  
  pinMode(LED_BUILTIN, OUTPUT); //Built in LED pin

  spikes.begin();
  //spikes.setBrightness(100);
  spikes.show(); // Initialize all pixels to 'off'


  #ifdef RH_HAVE_SERIAL
    Serial.begin(115200);	  // Debugging only
  #endif
    if (!driver.init())
  #ifdef RH_HAVE_SERIAL
         Serial.println("init failed.");
  #else
	;
  #endif


}

void loop() {
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Non-blocking
    {
	  //int i;
    //Serial.write(buf, buflen);
    //Serial.println();
	  // Message with a good checksum received, dump it.
    //digitalWrite(LED_BUILTIN, HIGH);
    Serial.print((char) buf[0]);
    Serial.println((char) buf[1]);

      if ((char) buf[0] == 'R'){
      resetfading();
      }
      else if((char) buf[0] == 'L'){
        flash_lamp();
      }
    //digitalWrite(LED_BUILTIN, LOW);
	  //driver.printBuffer("Got:", buf, buflen);
    }

  //For Direct Serial Command
  if (Serial.available() > 0) {
    int inByte = Serial.read();

    if (inByte == 'R'){
      resetfading();
    }
    else if (inByte == 'L'){
      flash_lamp();
    }
  }

  //Fading or flashed
  if (!flashed)
  {
    fading();
  }

  //Update Timer
  t.update();
  ///////////////?/
}


void flash_lamp(){
  turn_all_on();
  digitalWrite(LED_BUILTIN, HIGH);
  flashed = true; //Nonblocking
  t.after(100, turn_all_off);
  
  //delay(100); //Need to do this without delay
  //turn_all_off();
  
}

//Below are routines to save code
void turn_all_off()
{
  //todo: change to begin to reset
  for(uint16_t i=0; i<spikes.numPixels(); i++)
      spikes.setPixelColor(i, spikes.Color(0, 0, 0));
  spikes.show();
  flashed = false; //non blocking reset
  digitalWrite(LED_BUILTIN, LOW);
}

void turn_all_on()
{
  //todo: change to begin to reset
  for(uint16_t i=0; i<spikes.numPixels(); i++)
      spikes.setPixelColor(i, spikes.Color(150, 150, 150));
  spikes.show();
 
}

