/*
 * New_Modes.ino – PAWB modes + setSolidColor
 */
static unsigned long pawModePrev = 0;
static uint16_t pawStep = 0;
static uint8_t solidR = 150, solidG = 0, solidB = 255;
static int lastPawMode = -1;

void resetPawModeState() {
  pawModePrev = 0;
  pawStep = 0;
}

void setSolidColor(uint8_t r, uint8_t g, uint8_t b) {
  solidR = r; solidG = g; solidB = b;
}

void mode_rainbow_chase() {
  if (millis() - pawModePrev < 40) return;
  pawModePrev = millis();
  for (int i = 0; i < spikes.numPixels(); i++) {
    uint16_t h = pawStep + (i * 65536L / spikes.numPixels());
    spikes.setPixelColor(i, spikes.gamma32(spikes.ColorHSV(h)));
  }
  spikes.show();
  pawStep += 256;
}

void mode_comet() {
  if (millis() - pawModePrev < 50) return;
  pawModePrev = millis();
  for (int i = 0; i < spikes.numPixels(); i++) {
    uint32_t c = spikes.getPixelColor(i);
    spikes.setPixelColor(i, ((c >> 16) & 0xFF) * 0.65, ((c >> 8) & 0xFF) * 0.65, (c & 0xFF) * 0.65);
  }
  int head = pawStep % spikes.numPixels();
  spikes.setPixelColor(head, 255, 255, 255);
  if (head > 0) spikes.setPixelColor(head - 1, 160, 80, 255);
  spikes.show();
  pawStep++;
}

void mode_breathing() {
  if (millis() - pawModePrev < 30) return;
  pawModePrev = millis();
  static int breath = 40;
  static int dir = 1;
  breath += dir * 4;
  if (breath >= 255) { breath = 255; dir = -1; }
  if (breath <= 30) { breath = 30; dir = 1; }
  uint32_t color = spikes.gamma32(spikes.ColorHSV(pawStep, 255, breath));
  for (int i = 0; i < spikes.numPixels(); i++) spikes.setPixelColor(i, color);
  spikes.show();
  pawStep += 30;
}

void mode_fire() {
  if (millis() - pawModePrev < 45) return;
  pawModePrev = millis();
  for (int i = 0; i < spikes.numPixels(); i++) {
    int heat = random(40, 255) - (i * 20);
    if (heat < 0) heat = 0;
    uint8_t r, g, b;
    if (heat < 85) { r = heat * 3; g = 0; b = 0; }
    else if (heat < 170) { r = 255; g = (heat - 85) * 3; b = 0; }
    else { r = 255; g = 255; b = heat - 170; }
    spikes.setPixelColor(i, r, g, b);
  }
  spikes.show();
}

void mode_sparkle() {
  if (millis() - pawModePrev < 55) return;
  pawModePrev = millis();
  for (int i = 0; i < spikes.numPixels(); i++) {
    uint32_t c = spikes.getPixelColor(i);
    spikes.setPixelColor(i, ((c >> 16) & 0xFF) * 0.8, ((c >> 8) & 0xFF) * 0.8, (c & 0xFF) * 0.8);
  }
  if (random(0, 100) < 45) {
    int p = random(0, spikes.numPixels());
    spikes.setPixelColor(p, random(0, 2) ? 0xFFFFFF : spikes.Color(180, 60, 255));
  }
  spikes.show();
}

void mode_wave() {
  if (millis() - pawModePrev < 40) return;
  pawModePrev = millis();
  for (int i = 0; i < spikes.numPixels(); i++) {
    int phase = (pawStep + i * 20) % 100;
    int w = phase < 50 ? phase * 2 : (100 - phase) * 2;
    uint8_t bri = map(w, 0, 100, 20, 220);
    spikes.setPixelColor(i, (bri * 150) / 255, 0, bri);
  }
  spikes.show();
  pawStep++;
}

void mode_solid() {
  if (millis() - pawModePrev < 200) return;
  pawModePrev = millis();
  for (int i = 0; i < spikes.numPixels(); i++)
    spikes.setPixelColor(i, solidR, solidG, solidB);
  spikes.show();
}

void mode_off() {
  if (millis() - pawModePrev < 300) return;
  pawModePrev = millis();
  for (int i = 0; i < spikes.numPixels(); i++) spikes.setPixelColor(i, 0);
  spikes.show();
}

void mode_selector(int m) {
  if (m != lastPawMode) {
    resetPawModeState();
    lastPawMode = m;
  }
  switch (m) {
    case 0: soundloop(millis(), 50, false, micLevel); break;
    case 1: soundloop(millis(), 50, true, micLevel); break;
    case 2: {
      int n = map(constrain(micLevel, 0, 800), 0, 800, 0, spikes.numPixels());
      for (int i = 0; i < spikes.numPixels(); i++)
        spikes.setPixelColor(i, i < n ? spikes.Color(150, 0, 255) : 0);
      spikes.show();
      break;
    }
    case 3: mode_rainbow_chase(); break;
    case 4: mode_comet(); break;
    case 5: mode_breathing(); break;
    case 6: mode_fire(); break;
    case 7: mode_sparkle(); break;
    case 8: mode_wave(); break;
    case 9: mode_solid(); break;
    case 10: mode_off(); break;
    default: mode = 0; break;
  }
}
