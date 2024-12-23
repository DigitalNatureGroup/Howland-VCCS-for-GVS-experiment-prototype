#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <math.h>

Adafruit_MCP4725 dac;

#define DAC_RESOLUTION    (4096)   
#define OFFSET            (2048)   
#define AMPLITUDE         (512)    
#define NUM_POINTS        (128)  
float offsetX = 0.04;  
int Correct = 0;

void setup(void) {
  Wire.begin();
  Wire.setClock(400000);   
  dac.begin(0x62);  
  Correct = mapFloat(offsetX, -4.0, 4.0, 0, 4095);
  dac.setVoltage(Correct, false); 
  delay(10000);  
}

float linspace(int start, int end, int step, int numSteps) {
  return start + (float)(end - start) * step / (numSteps - 1);
}

void loop(void) {
  static bool reachedMaxFrequency = false; 
  static int step = 0;                     
  
  float targetFrequency;
  
  if (!reachedMaxFrequency) {
    targetFrequency = linspace(1, 20, step, 20); 
    
    if (step < 19) {
      step++; 
    } else {
      reachedMaxFrequency = true; 
    }
  } else {
    targetFrequency = 20; 
  }
  
  float sampleRate = targetFrequency * NUM_POINTS;    
  unsigned long delayTime = 1000000 / sampleRate;  

  for (int repeat = 0; repeat < 3; repeat++) {
    unsigned long lastTime = micros();  
    for (uint16_t i = 0; i < NUM_POINTS; i++) {
      float angle = (2 * PI * i) / NUM_POINTS;
      uint16_t voltage = Correct + (AMPLITUDE * sin(angle));

      dac.setVoltage(voltage, false);
      while (micros() - lastTime < delayTime);
      lastTime += delayTime;
    }
  }
}

int mapFloat(float x, float in_min, float in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
