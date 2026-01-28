

#include "Pumking.h"
#include "MessageParser.h"

//           temp, uv, ss, rst, rinpin ring 
Pumking pumking(4, A1, 10, 7, 3, 24);

// variables for measurement
static bool dataMode = false;
static bool measureStarted = false;
// message from protopie
MessageParser messageRecieved;
// for interaction option
int opt = 1;

void setup() {
  Serial.begin(9600);
  pumking.begin();
  Serial.println("Pumpking and friends init");
  // set init value to the screen
  pumking.prinTempData();
  pumking.levelUp();
  pumking.increaseLevelProgress(0);
}

// this runs forever
void loop() {
  if( dataMode && !measureStarted){
    measureStarted = true;
    pumking.startMeasure();
  }
  if (dataMode && measureStarted){
    pumking.measureEnvironment();
  }
  // nfc 
  if(pumking.getRFIDData(opt)){
    opt = 0;
  }
  // for protopie
  readProtopieMessages();
}

void turnDataModeOn(){
  dataMode = true;
}

void turnDataModeOff(){
  dataMode = false;
  measureStarted = false;
}

void resetStartMeasureFlag(){
  // if datamode is on, it will restart the measurement
  measureStarted = false;
}

// Protopie States
void readProtopieMessages(){
  while (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\0');
		messageRecieved = getMessage(receivedString);

    switch(messageRecieved.message){
      case MESSAGETYPE::START:
        turnDataModeOn();
        break;
      case MESSAGETYPE::SEEN:
        turnDataModeOff();
        break;
      case MESSAGETYPE::WATER:
        pumking.waterPumking();
        break;
      case MESSAGETYPE::FERTILIZE:
        opt = 1;
        break;
      case MESSAGETYPE::PRUNING:
        opt = 2;
        break;
    }
    
  }
}
