#include "integration.h"
#include "derivation.h"

const int pinLED = 13;

struct Coordinates {
  int x;
  int y;
  bool ready; //is useful??
};

Coordinates actualCoords;
bool isX = 0;
bool isY = 0;
bool isNegative = 0;
int num = 0;

void setup(){
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
  delay(1000);
}

void loop(){
  if (Serial.available()>0){
    // read face coordinates values
    char option = Serial.read();
    switch(option){
      case 'x': // x-axis flag
        isX = 1;
        break;
      case 'y': // y-axis flag
        isY = 1;
        break;
      case 'n': // negative flag
        isNegative = 1;
        break;
      case '1': // bit with value "1" insertion to num variable
        num = num << 1;
        num = num | 1;
        break;
      case '0': // bit with value "0" insertion to num variable
        num = num << 1;
        break;
      case 'f': // one axis readed
        if (isNegative){
        num = -1*num;
        }
        if (isX){
          actualCoords.x = num;
        }
        if (isY){
          actualCoords.y = num;
        }
        num = 0;
        isX = 0;
        isY = 0;
        isNegative = 0;
        break;
      case 'r': // both axis ready to use
        // debug blinking
        if(actualCoords.x == -372 && actualCoords.y == -146){
          digitalWrite(pinLED, HIGH);
          delay(700);
          digitalWrite(pinLED, LOW);
          delay(400);
          digitalWrite(pinLED, HIGH);
          delay(100);
          digitalWrite(pinLED, LOW);
          delay(100);
        }
        break;
    }
  }

  // maybe check here if actualCoords.ready is true
  // to apply pid and move servos.
}