#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <math.h>
#include <Uduino_Wifi.h>
#include <WiFi.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
Uduino_Wifi uduino("GVSTester");
Adafruit_MCP4725 dac;

float currentValue = 0;
int slopeValue = 0;
int durationValue = 0;
float offset = 0.04;
float current_mA = 0;

unsigned long lastPrintTime = 0;
const unsigned long printInterval = 50;

unsigned long sineWaveStartTime = 0;
int sineWaveStep = 0;
bool sineWaveRunning = false;
int sineWaveSteps = 0;
int sineWaveDelayTime = 0;
int sineWaveAmplitude = 0;
int sineWaveOffsetValue = 0;
int sineWaveFinalDuration = 0;
unsigned long sineWaveFinalStartTime = 0;
bool sineWaveHoldFinalVoltage = false;
bool sineWaveReturnToMidpoint = false;

void setup(void) {
  Serial.begin(115200);
  dac.begin(0x62);
  ina219.begin();
  ina219.setCalibration_16V_400mA();
  uduino.setPort(4222);
  uduino.useSendBuffer(true);
  uduino.setConnectionTries(35);
  uduino.useSerial(true);
  uduino.connectWifi("WifiName", "Password");
  uduino.addCommand("a", Apply);
}

void loop() {
  uduino.update();
  printCurrentValues();
  handleSineWave();
}

void Apply() {
  int parameters = uduino.getNumberOfParameters();
  if (parameters >= 3) {
    currentValue = atof(uduino.getParameter(0));
    slopeValue = atoi(uduino.getParameter(1));
    durationValue = atoi(uduino.getParameter(2));

    if (currentValue >= -4 && currentValue <= 4 && slopeValue > 0 && durationValue > 0) {
      int inputValue = mapFloat(currentValue, -4.0, 4.0, 0, 4095);
      sineWaveOffsetValue = mapFloat(offset, -4.0, 4.0, 0, 4095) - 2048;
      sineWaveAmplitude = inputValue - 2048;
      sineWaveSteps = slopeValue;
      sineWaveDelayTime = slopeValue / sineWaveSteps; 
      sineWaveFinalDuration = durationValue;
      sineWaveStep = 0;
      sineWaveRunning = true;
      sineWaveHoldFinalVoltage = false;
      sineWaveReturnToMidpoint = false;
      sineWaveStartTime = millis(); 
    } else {
      //Serial.println("Invalid parameters");
    }
  } else {
    //Serial.println("Expected 3 parameters.");
  }
}

void handleSineWave() {
  if (sineWaveRunning) {
    if (sineWaveStep < sineWaveSteps) {
      // Ascending
      if (millis() - sineWaveStartTime >= sineWaveDelayTime) {
        float angle = (PI / 2.0) * (float)sineWaveStep / (float)sineWaveSteps;
        int voltage = 2048 + sineWaveAmplitude * sin(angle) + sineWaveOffsetValue;
        dac.setVoltage(voltage, false);
        sineWaveStep++;
        sineWaveStartTime = millis();
      }
    } else if (!sineWaveHoldFinalVoltage) {
      // Holding
      int finalVoltage = 2048 + sineWaveAmplitude * sin(PI / 2.0) + sineWaveOffsetValue;
      dac.setVoltage(finalVoltage, false);
      sineWaveFinalStartTime = millis();
      sineWaveHoldFinalVoltage = true;
    } else if (sineWaveHoldFinalVoltage && (millis() - sineWaveFinalStartTime >= sineWaveFinalDuration)) {
      // Descending 
      if (sineWaveStep < 2 * sineWaveSteps) {
        if (millis() - sineWaveStartTime >= sineWaveDelayTime) {
          float angle = (PI / 2.0) * (float)(sineWaveStep - sineWaveSteps) / (float)sineWaveSteps;
          int voltage = 2048 + sineWaveAmplitude * sin(PI / 2.0 - angle) + sineWaveOffsetValue;
          dac.setVoltage(voltage, false);
          sineWaveStep++;
          sineWaveStartTime = millis();
        }
      } else if (sineWaveStep >= 2 * sineWaveSteps) {
        // Reset to midpoint
        dac.setVoltage(2048 + sineWaveOffsetValue, false);
        sineWaveRunning = false;
      }
    }
  }
}


void printCurrentValues() {
  if (millis() - lastPrintTime >= printInterval) {
    lastPrintTime = millis();
    current_mA = ina219.getCurrent_mA();
    uduino.println(current_mA);
  }
}

int mapFloat(float x, float in_min, float in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
