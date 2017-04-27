#include <Adafruit_CircuitPlayground.h>

#define NUMPIXELS 10
#define SPEAKER 5
#define COLORTHRESH .66

float pixels[] = {210, 240, 270, 300, 330, 30, 60, 90, 120, 150};

float X[16] = {0};
float Y[16] = {0};
float meanX = 0;
float meanY = 0;
int index = 0;

int color[] = {256, 0};

void setup() {
  Serial.begin(9600);
  
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(255);
  CircuitPlayground.clearPixels();
}

void loop() {
  X[index] = CircuitPlayground.motionX();
  Y[index] = CircuitPlayground.motionY();
  ++index %= 16;
  
  meanX = meanY = 0;
  for(int i = 0; i < 16; ++i) {
    meanX += X[i];
    meanY += Y[i];
  }
  meanX /= 16;
  meanY /= 16;

  #ifdef SPEAKER
    if(sqrt(pow(meanX, 2) + pow(meanY, 2)) > COLORTHRESH) tone(SPEAKER, 500);
    else noTone(SPEAKER);
  #endif
  
  for(int i = 0; i < NUMPIXELS; ++i)
    CircuitPlayground.setPixelColor(i, hsb2rgb((sqrt(pow(meanX, 2) + pow(meanY, 2)) > COLORTHRESH) ? color[1] : color[0], 255, pow(constrain((cos(atan2(meanY, meanX) - (pixels[i] / 180 * 3.14159)) + 1) / 2, 0, 1), sqrt(pow(meanX, 2) + pow(meanY, 2))) * (20 + 230 * constrain(sqrt(pow(meanX, 2) + pow(meanY, 2)) / 10, 0, 1))));
  delay(1);
}

uint32_t hsb2rgb(int16_t index, uint8_t sat, uint8_t bright) {
    uint32_t color = 0;

    uint8_t temp[3], n = (index >> 8) % 3;
    uint8_t x = ((((index & 255) * sat) >> 8) * bright) >> 8;
    uint8_t s = ((256 - sat) * bright) >> 8;
    temp[0] = s;
    temp[1] = x + s * 0;
    temp[2] = bright - x;
    
    
    /**/color += temp[(n + 2) % 3];
    color = color << 8;
    color += temp[(n + 1) & 3];
    color = color << 8;
    color += temp[n];
    return color;
}
