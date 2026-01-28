#ifndef PUMKING_H
#define PUMKING_H

#include <Arduino.h>
#include <DHT11.h>
#include <MFRC522.h>
#include "RingLed.h"

class Pumking {
private:
  // pins
  String uvPin;

  //sensors
  DHT11 temperatureSensor;
  MFRC522 rfidSensor;

  //ringLed
  RingLed ring;

  // values
  float humidityValue;
  float temperatureValue;
  int uvLightValue;

  // measure values
  // 3 days
  int NUM_HOURS;
  float hourlyTempValues[5];
  float hourlyHumidityValues[5];
  float hourlyUVValues[5];

  // hours values
  int hourIndex;
  // One hour in milliseconds
  unsigned long oneHour;
  unsigned long lastUpdate;

  // plant suggestions
  String* plantRecomendation;
  static String plantGroups[][3];
  bool viewedSuggestion;
  bool finishSuggestions;

  // user gardener level
  int gardeningLevel;
  int levelProgress;
  

public:
  Pumking(int tempPin, int uvPin, int rfidSSPin, int rfidRSTPin, int ringLedPin, int rinLedsCount);
  void begin();
  // rfid
  bool getRFIDData(int opt);
  // uv light
  void getUVData();
  // temperature
  void getTempData();
  float getHumidityValue() const;
  float getTemperatureValue() const;
  void prinTempData() const;
  // measure (main)
  void measureEnvironment();
  float getAvg(float arr[]);
  void startMeasure();
  // suggestions
  void createPlantSuggestions();
  String* plantSuggestion(float avgTemp, float avgHum, float avgUV);
  void plantsSuggViwed();
  // interactions
  void waterPumking();
  void prunePumking();
  void fertilizePumking();
  void levelUp();
  void alert();
  void increaseLevelProgress(int prog);
  // protopie
  void sendSensorsDataToProtopie();
};

#endif
