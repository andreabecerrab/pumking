#include <Arduino.h>
#include <DHT11.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>

#include "Pumking.h"
#include "RingLed.h"

Pumking::Pumking(int tempPin, int uvPin, int rfidSSPin, int rfidRSTPin,  int ringLedPin, int rinLedsCount)
    : temperatureSensor(tempPin), uvPin(uvPin), rfidSensor(rfidSSPin, rfidRSTPin), ring(rinLedsCount, ringLedPin){
      // init time variables
      hourIndex = 0;
      // oneHour = 30000UL;
      // for testing change time from 1 hour to 10 seconds
      oneHour = 2000UL;
      lastUpdate = 0;
      NUM_HOURS = 5;
      viewedSuggestion = false;
      finishSuggestions = false;
      // level
      gardeningLevel = 0;
      levelProgress = 0;
    }

void Pumking::begin() {
    SPI.begin();
    rfidSensor.PCD_Init();
    ring.begin();
    getTempData();
    // Serial.println("Pumking Succesfully Initialized");
}

/** Pumkin functionality */
int p = 0;
void Pumking::startMeasure(){
  // clean all variables
  hourIndex = 0;
  p = 5;
  viewedSuggestion = false;
  finishSuggestions = false;
}

void measureProgress(int p){
  Serial.print("MEASUREP||");
  Serial.println(p);
  }

// start measurement
void Pumking::measureEnvironment(){
  // one day at a time
  unsigned long now = millis();
  if (now - lastUpdate >= oneHour && hourIndex < NUM_HOURS) {
    measureProgress(p);
    p = p - 1;
    lastUpdate = now;
    // update data
    getTempData();
    getUVData();
    
    // add data to measurement
    hourlyTempValues[hourIndex] = temperatureValue;
    hourlyHumidityValues[hourIndex] = humidityValue;
    hourlyUVValues[hourIndex] = uvLightValue;

    // printsss for protopie connect
    sendSensorsDataToProtopie();

    hourIndex++;
  }
  // if time passed then build suggestions
  if(hourIndex >= NUM_HOURS && !viewedSuggestion){
    createPlantSuggestions();
  }
}

float Pumking::getAvg(float arr[]) {
    float sum = 0.0;
    for (int i = 0; i < NUM_HOURS; i++) {
        sum += arr[i];
    }
    return sum / (NUM_HOURS);
}

String* Pumking::plantSuggestion(float avgTemp, float avgHum, float avgUV){
  // TODO: based on user level
  static String results[3];
  String description = "Must try at home once";
  // Classify humidity
  String hum = (avgHum > 70) ? "high" : (avgHum > 40) ? "medium" : "low";

  // Classify temperature
  String temp = (avgTemp > 28) ? "hot" : (avgTemp > 18) ? "warm" : "cold";

  // Classify UV intensity
  String uv = (avgUV > 10) ? "strong" : (avgUV > 4) ? "medium" : "low";

  // default values in case measures failed
  results[0] = "Spider Plant";
  results[1] = "Air Plant";
  results[2] = "Ficus";

  // The AI plant suggestion
  if (temp == "hot" && hum == "low" && uv == "strong") {
    results[0] = "Cactus";
    results[1] = "Aloe Vera";
    results[2] = "Yucca";
    description = "Dry, sunny conditions with minimal moisture stress";
    return results;
  }

  if ((temp == "hot" || temp == "warm") && hum == "high") {
    results[0] = "Monstera";
    results[1] = "Banana Plant";
    results[2] = "Philodendron";
    description = "Warm, wet environment encouraging lush growth";
    return results;
  }
  if (temp == "warm" && hum == "low" && uv == "low") {
    results[0] = "Snake Plant";
    results[1] = "Moss";
    results[2] = "Parsley";
    description = "Cool, moist environment with minimal light";
    return results;
  }

  if (temp == "warm" && hum == "medium" && uv == "medium") {
    results[0] = "Basil";
    results[1] = "Tomato";
    results[2] = "Rosemary";
    description = "Moderate warmth with balanced sunlight and moisture";
    return results;
  }

  if (temp == "warm" && hum == "medium" && uv == "low") {
    results[0] = "Ferns";
    results[1] = "Peace Lily";
    results[2] = "Calathea";
    description = "Humid, shaded environment with gentle light";
    return results;
  }

  if (temp == "cold" && hum == "low" && uv == "low") {
    results[0] = "Succulent";
    results[1] = "Snake Plant";
    results[2] = "Haworthia";
    description = "Cool, dry conditions with low sunlight";
    return results;
  }

  if (temp == "cold" && hum == "high" && uv == "low") {
    results[0] = "Mint";
    results[1] = "Moss";
    results[2] = "Parsley";
    description = "Cool, moist environment with minimal light";
    return results;
  }

  Serial.print("PLANTDESC||");
  Serial.println(description);

  return results;
}

void Pumking::createPlantSuggestions(){
  // to do it only once
  if( !finishSuggestions ){
    float avgTemp = getAvg(hourlyTempValues);
    float avgHum = getAvg(hourlyHumidityValues);
    float avgUv = getAvg(hourlyUVValues);

    plantRecomendation = plantSuggestion(avgTemp, avgHum, avgUv);
    // print recommendations
    for (int i = 0; i < 1; i++) {
      Serial.print("PLANT||");
      Serial.println(plantRecomendation[i]);
    }
    // for future push into the saved recommendation
    finishSuggestions = true;
    // increale level
    increaseLevelProgress(35);
  }
  // when finishes blink
  ring.flashing();
  // phone notification here
}

// these will afccet the gardeningLevel
void Pumking::waterPumking(){
  increaseLevelProgress(18);
  ring.blueDance();
}

void Pumking::prunePumking(){
  increaseLevelProgress(15);
  ring.greenDance();
  ring.orangePumpking(0);
}

void Pumking::fertilizePumking(){
  increaseLevelProgress(25);
  ring.roseDancing();
  ring.orangePumpking(0);
}

void Pumking::levelUp(){
  gardeningLevel = gardeningLevel + 1;
  Serial.print("LEVEL||");
  Serial.println(gardeningLevel);
  if(gardeningLevel > 1){
    ring.rainbow();
    ring.orangePumpking(0);
  }
}

void Pumking::alert(){
  for(int i = 0; i < 3; i++){
    ring.flashing();
  }
  ring.orangePumpking(0);
}

void Pumking::increaseLevelProgress(int prog){
  if(levelProgress + prog >= 100){
    levelProgress = 0;
    Serial.print("PROGRESS||");
    Serial.println(levelProgress);
    levelUp();
  } else {
    levelProgress = levelProgress + prog;
    Serial.print("PROGRESS||");
    Serial.println(levelProgress);
  }
}

/** SENSORS */

/** RFDI Sensor */
bool Pumking::getRFIDData(int opt){
   if (!rfidSensor.PICC_IsNewCardPresent()) {
    delay(500);
    return;
  }
   if (!rfidSensor.PICC_ReadCardSerial()) {
    return;
  }

  String uidString = "";
  // Serial.print("UID: ");
  for (byte i = 0; i < rfidSensor.uid.size; i++) {
    uidString += String(rfidSensor.uid.uidByte[i], HEX);
  }
  if(uidString == "44a9cb1" && opt == 1){
    fertilizePumking();
    ring.orangePumpking(0);
    return true;
  }
  if(uidString == "44a9cb1" && opt == 2){
    prunePumking();
    ring.orangePumpking(0);
    return true;
  }
  // Serial.println(uidString);
  rfidSensor.PICC_HaltA();
  rfidSensor.PCD_StopCrypto1();
  delay(100);
  // return 0 if no action triggered
  return false;
}

/** UV Sensor **/
void Pumking::getUVData(){
  int rawValue = analogRead(A1);
  float voltage = rawValue * (5.0 / 1023.0);

  // to UV
  float uvIndex = map(voltage * 100, 99, 280, 0, 15) / 1.0;

  // clamp values
  uvLightValue = constrain(uvIndex, 0, 20);
}

/** Temperature Sensor **/

void Pumking::getTempData() {
    humidityValue = temperatureSensor.readHumidity();
    temperatureValue = temperatureSensor.readTemperature();
}

float Pumking::getHumidityValue() const {
    return humidityValue;
}

float Pumking::getTemperatureValue() const {
    return temperatureValue;
}

// for testing all sensors
void Pumking::prinTempData() const {
      //print uv light sensor 
    getTempData();
    getUVData();
    getRFIDData(0);
    //
    sendSensorsDataToProtopie(); 

}

/* PROTOPIE */
void Pumking::sendSensorsDataToProtopie(){
  Serial.print("TEMPERATUREVALUE||");
  Serial.println(int(temperatureValue));

  Serial.print("HUMIDITYVALUE||");
  Serial.println(int(humidityValue));

  Serial.print("UVVALUE||");
  Serial.println(int(uvLightValue));
}
