
#include "SPI.h"
#include "Adafruit_WS2801.h"

#define HEADER_SIZE 1
#define COLOR_SIZE 1

int dataPin=2;
int clockPin=3;
int numberPixels=25; // #LEDs in the string

// size of a packet, in bytes
int packetSize= HEADER_SIZE + 3 * COLOR_SIZE;

struct rgb {
  uint8_t red;
  uint8_t green; 
  uint8_t blue;
};

struct rgb currentRGB;
Adafruit_WS2801 strip = Adafruit_WS2801(numberPixels, dataPin, clockPin);


uint32_t makeColor(struct rgb r) {
  uint32_t c;
  
  c = r.red;
  c <<= 8;
  c |= r.green;
  c <<= 8;
  c |= r.blue;
  
  return c;
}


int readValueFromSerial() {
  int tmp;
  
  tmp = Serial.read();

#if (COLOR_SIZE > 1)
  tmp = tmp << 8;

  tmp += Serial.read();
#endif
  return tmp;
}

void setStripColors(struct rgb colors) {
  uint32_t color;

  color = makeColor(colors);
  for (int i = 0; i < numberPixels; i++) {
    strip.setPixelColor(i, color);
    
  }
  strip.show();
}

void readFromSerial(struct rgb colors) {

  // an entire packet is available
  if (Serial.available() >= packetSize) {    
    if (Serial.read() == 0xda) {
      colors.red   = readValueFromSerial();
      colors.green = readValueFromSerial();
      colors.blue  = readValueFromSerial();
    }
  }
}

void setup() {
  strip.begin();
  Serial.begin(9600);

  currentRGB.red = currentRGB.green = currentRGB.blue = 0;
}

void loop() {

  readFromSerial(currentRGB);
  setStripColors(currentRGB);
  delay(100);

}
