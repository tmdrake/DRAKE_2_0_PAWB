int brightness = 0;
bool _direction;
void resetfading() {
  brightness = 0;
  _direction = false;
}

void fading() {

  static unsigned long previousMillis;

  if (millis() - previousMillis >= 50)  //time expired
  {

    if (_direction)
      brightness--;
    else
      brightness++;

    if (brightness >= 75)
      _direction = true;
    else if (brightness <= 0)
      _direction = false;

    set_brightness(brightness);
    //Serial.println(brightness);


    previousMillis = millis();
  }
}

void set_brightness(byte brightness) {
  float _brightness = (float)brightness / 100.0;

  for (uint16_t i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, spikes.Color(150 * _brightness, 0, 255 * _brightness));


  spikes.show();
}


void resetBrightnessandDirection(){
  /*A Wrapper i know!*/
  resetfading();
}