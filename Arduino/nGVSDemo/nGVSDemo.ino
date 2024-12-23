#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <math.h>

Adafruit_MCP4725 dac;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

const int maxAmplitude = 2048;    
const int baseOffset = 2048;      
float offsetD = 0.04;             
float limitedArea = 3.6;          
const int numSteps = 100;          

int noiseTable[numSteps];         

void setup(void) {
  Serial.begin(115200);
  dac.begin(0x62);  
  Wire.setClock(1000000);         

  float mappedOffsetD = mapFloat(offsetD, 0, 8.0, 0, 4095);
  float mappedLimitedArea = mapFloat(limitedArea, 0, 8.0, 0, 4095);
  float actualAmplitude = mappedLimitedArea / 2.0;  
  float actualOffset = baseOffset + mappedOffsetD;
  dac.setVoltage((int)actualOffset, false); 
  delay(5000);  
  
  for (int i = 0; i < numSteps; i++) {
    noiseTable[i] = (int)(random(-actualAmplitude, actualAmplitude) + actualOffset);
  }
}

void loop() {
  int delayTime = 100000 / numSteps; 

  for (int i = 0; i < numSteps; i++) {
    dac.setVoltage(noiseTable[i], false);
    delayMicroseconds(delayTime);  

    noiseTable[i] = (int)(random(-actualAmplitude, actualAmplitude) + baseOffset);
  }
}
