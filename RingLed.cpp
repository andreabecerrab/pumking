#include <Arduino.h>
#include "RingLed.h"

#include "RingLed.h"

RingLed::RingLed(int rinLedsCount, int ringLedPin)
    : ring(rinLedsCount, ringLedPin, NEO_GRB + NEO_KHZ800), ledsCount(rinLedsCount) {}

void RingLed::begin() {
    ring.begin();
    ring.show(); //turn off
    ring.setBrightness(250); // Brightness (0-255)
    // set to orange
    orangePumpking(0);
    // Serial.println("Ring initialized succesfully");
}

//  turn single led on
void RingLed::changeLedColor(int n, int r, int g, int b){
  ring.setPixelColor(n, r, g, b);
  ring.show();
}
// turn complete ring on
void RingLed::changeRingColor(int r, int g, int b, int d){
  for(int i = 0; i < ledsCount; i++){
    changeLedColor(i, r, g, b);
    delay(d);
  }
}
// turn ring off
void RingLed::turnRingOff(){
  ring.clear(); // sets all LEDs to 0,0,0
  ring.show();  // update the ring
}

// alert for plant sugegstion ready
void RingLed::flashing(){
  for(int i = 10; i <= 100; i += 10){
    ring.setBrightness(i + 20);
    ring.show();
    delay(100);
  }
}

void RingLed::orangePumpking(int d){
  // 255, 102, 0
    changeRingColor(255, 102, 0, d);
}
// for water related
void RingLed::blueDance(){
  for(int i = 0; i < 2; i++){
    changeRingColor(0, 225, 255, 30);  //blue color
    changeRingColor(0, 123, 255, 30); 
    changeRingColor(0, 64, 255, 30);  
  }
  delay(1000);
  orangePumpking(0);
}

// for prunning related
void RingLed::greenDance(){
  for(int i = 0; i < 3; i++){
    changeRingColor(38, 207, 0, 0);
    flashing();
  }
}

// for fertilizing related
void RingLed::roseDancing(){
  // on
  for(int i = 0; i < 4; i++){
    orangePumpking(50);
    for(int j = 0; j < 12; j++){
       int led = random(0, ledsCount);
      changeLedColor(led, 255, 0, 255);
    }
    delay(10);
  }
}

// rainbow 
void RingLed::rainbow(){
  Serial.println("rainbow");
  // rainbow colors
  const int RAINBOW[7][3] = {
    {255, 0, 0,},   // Red
    {255, 161, 0,},   // Orange
    {237, 237, 0,},   // Yellow
    { 0, 219, 0,},   // Green
    {0, 60, 255},   // Blue
    {128, 0, 255,},   // Indigo
    {225, 0, 255,}    // Violet
  };
  for(int j = 0; j < 1; j++){
    for (int i = 0; i < ledsCount; i++) {
      int r = RAINBOW[i % 7][0];
      int g = RAINBOW[i % 7][1];
      int b = RAINBOW[i % 7][2];
      changeRingColor( r, g, b, 0);
      delay(300);
      // d = d  >= 0 ? d - 50 : 0;
    }
  }
}


void RingLed::madurate(){
  // start color 63, 245, 39
  int r = 63;
  int g = 245;
  int b = 39;
  changeRingColor(r, g, b, 0);
  // fade
  for(int i = 0; i <= 8; i++){
    delay(1000);
    if( r < 245){
     r += 60;
      if (r > 255){
        r = 245;
      }
    } else {
     g -= 40;
    }
    changeRingColor(r, g, b, 0);
  }
}