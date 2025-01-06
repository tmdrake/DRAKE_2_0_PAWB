/*added on 1-5-2025
We have enough resources to make this work but checking to see if its stable

*/
float cR = 0, cG = 0, cB = 0;  // Need to use descriptive varables....
void soundloop(unsigned long millis, long refresh_ms, bool color, long _micinput) {
  /*sample and sound was done at <intervals in mS> aka frames*/
  /*added micLevel, make -1 to ignore*/
  static unsigned long soundloop_previousMillis = 0;  //persistant varable
  static int k = 0;
  static int col = 0;

  if (millis - soundloop_previousMillis >= refresh_ms /*ms*/) {
    //Serial.println("Soundloop fired!");
    soundloop_previousMillis = millis;  //reset counter

    //get our sound audio
    long squareLevel = _micinput;  //sampleaudio(micLevel);

    //Shift everything down 1 spike, leave spike 0 with the last sound
    for (uint16_t i = spikes.numPixels(); i > 0; i--) {
      spikes.setPixelColor(i, spikes.getPixelColor(i - 1));
    }

    if (squareLevel / 1.0f > 0.5) {
      spikes.setPixelColor(0, spikes.Color(0, 0, 0));  //reset 0 to all
    }

    //sets the first pixel in the stream
    setRgb(squareLevel / 1.0f);

 
    spikes.show();  //push the color out to all
  
    //delay(5);  //***this is needed but will multiply out, needs a asyn clock

    if (!color) {
      fadeRgb();  //Makes the color more warmer (optional)
    }


    k++;
  }

  if (k >= spikes.numPixels()) {
    col = ++col % 6;  // col = col modulus 6, Plus 1
    if (color) {
      cycleRgb(col);  //More Distinct RGB mode
    }

    k = 0;  //reset
  }
}


/*Cycle col based on the rotational values in COL*/
void cycleRgb(int col) {
  switch (col) {
    case 0:
      cR = 1;
      cG = 0;
      cB = 0;
      break;

    case 1:
      cR = 0.5;
      cG = 0.5;
      cB = 0;
      break;

    case 2:
      cR = 0;
      cG = 1;
      cB = 0;
      break;

    case 3:
      cR = 0;
      cG = 0.5;
      cB = 0.5;
      break;

    case 4:
      cR = 0;
      cG = 0;
      cB = 1;
      break;

    case 5:
      cR = 0.5;
      cG = 0;
      cB = 0.5;
      break;
  }
}




/*Set output for the first neopixel*/
void setRgb(float val) {
  //Drakes; Drops the first pixel on each set
  if (val > 1.0f) val = 1.0f;
  //        rgb[0].r = (int)(val * cR * 255);
  //        rgb[0].g = (int)(val * cG * 255);
  //        rgb[0].b = (int)(val * cB * 255);
  //        back.setPixelColor(0, back.Color((int)(val * cR * 255), (int)(val * cG * 255), (int)(val * cB * 255)));
  spikes.setPixelColor(0, (int)(val * cR * 255), (int)(val * cG * 255), (int)(val * cB * 255));
}

/*Sets the phase of the cR, cB, cG values & Phase*/
void fadeRgb() {
  const int phaseLength = 100;         // thinking that it dose the color lenght or the linth of the power coming in
  const int period = phaseLength * 5;  //the changes depend pon the delays...probabbly should have it set into a value
  static int iteration = 0;
  // Determine which phase we are in
  int phase = iteration / phaseLength;
  int step = iteration % phaseLength;

  switch (phase) {
    case 0:  // Red on, green increasing, blue off
      cR = 1;
      cG = step / ((float)phaseLength);
      cB = 0;
      break;

    case 1:  // Red decreasing, green on, blue off
      cR = (phaseLength - step) / ((float)phaseLength);
      cG = 1;
      cB = 0;
      break;

    case 2:  // Red off, green on, blue increasing
      cR = 0;
      cG = 1;
      cB = step / ((float)phaseLength);
      break;

    case 3:  // Red off, green decreasing, blue on
      cR = 0;
      cG = (phaseLength - step) / ((float)phaseLength);
      cB = 1;
      break;

    case 4:  // Red increasing, green off, blue on
      cR = step / ((float)phaseLength);
      cG = 0;
      cB = 1;
      break;

    case 5:  // Red on, green off, blue decreasing
      cR = 1;
      cG = 0;
      cB = (phaseLength - step) / ((float)phaseLength);
      break;
  }

  iteration = ++iteration % period;
}