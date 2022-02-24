#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESP_MICRO.h"

char buff [50];
volatile byte indx;

void setup() {
  Serial.begin(9600);
  start("WLAN1-A5427L","eYEaBt8iiFrfaBJ3");  // It will connect to your wifi with given details
}

void loop() {
  //waitUntilNewReq();  //Waits until a new request from python come

  if (Serial.available() > 0) {
    byte c = Serial.read();
    if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == '\r') { //check for the end of the word
        buff [indx] = ' ';
        
        CheckNewReq();  // Check if there was a request
        if (isReqCame)
        {
          isReqCame = false;
          returnThisStr(buff); // Returns the data to python
        }
        //Serial.print(buff);
        indx= 0; //reset button to zero
        delay(10);
      }
    }
  }
}