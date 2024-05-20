#include "integration.h"
#include "derivation.h"
#include <Servo.h>

const int pinLED = 13;
Servo servoX;

// unsigned long to prevent time issues
// millis() return unsigned long btw
unsigned long time = 0;
int period = 30;

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

// PID variables
int actual_err_x = 0;
int prev_err_x = 0;

float kp = 0.25;
float ki = 0.5;
float kd = 0.5;

float PID_p = 0;
float PID_i = 0;
float PID_d = 0;

float PID_total = 0;

void setup(){
  Serial.begin(9600);
  servoX.attach(2);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
  //delay(1000);
  actualCoords.x = 0;
  actualCoords.y = 0;
  time = millis();
  servoX.write(90);
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
        if(millis() > 4000){
          if(millis() >= time + period){ // + period
            time = millis();
            // debug blinking

            actual_err_x = actualCoords.x;

            PID_p = kp * actual_err_x;
            PID_d = kd * derivation(period, actual_err_x, prev_err_x);
            PID_i = PID_i + (ki * integration(period, actual_err_x));

            PID_total = PID_p + PID_i + PID_d;

            servoX.write(map(-PID_total, -320, 320, 0, 180));

            prev_err_x = actual_err_x;

            /*
            if(actualCoords.x < 0 && actualCoords.y < 0){
              digitalWrite(pinLED, HIGH);
              //delay(3000);
              //
              delay(700);
              digitalWrite(pinLED, LOW);
              delay(400);
              digitalWrite(pinLED, HIGH);
              delay(100);
              digitalWrite(pinLED, LOW);
              delay(100);
              //
            } else {
              digitalWrite(pinLED, LOW);
              //delay(3000);
            }
            */
            //break;
            
          }
        }
        break;
    }
  }

  // maybe check here if actualCoords.ready is true
  // to apply pid and move servos.
}