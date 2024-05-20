#include "integration.h"
#include "derivation.h"
#include <Servo.h>

//debug
const int pinLED = 13;

// servos
Servo servoX;
Servo servoY;

// unsigned long to prevent time issues
// millis() return unsigned long btw
unsigned long time = 0;
int period = 50;

struct Coordinates {
  int x;
  int y;
  //bool ready; //is useful??
};

Coordinates actualCoords;

bool isX = 0;
bool isY = 0;
bool isNegative = 0;
int num = 0;

// PID variables X axis
int actual_err_x = 0;
int prev_err_x = 0;

float xkp = 0.25; // 0.25
float xki = 0.5; //0.5
float xkd = 0.5; //0.5

float xPID_p = 0;
float xPID_i = 0;
float xPID_d = 0;

float xPID_total = 0;

// PID variables Y axis
int actual_err_y = 0;
int prev_err_y = 0;

float ykp = 0.06; // 0.06
float yki = 0.1; // 0.1
float ykd = 0.1; // 0.7

float yPID_p = 0;
float yPID_i = 0;
float yPID_d = 0;

float yPID_total = 0;

void setup(){
  Serial.begin(9600);
  
  servoX.attach(2);
  servoY.attach(3);

  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
  //delay(1000);
  actualCoords.x = 0;
  actualCoords.y = 0;
  time = millis();

  servoX.write(90);
  servoY.write(90);
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
            
            actual_err_x = actualCoords.x;

            xPID_p = xkp * actual_err_x;
            xPID_d = xkd * derivation(period, actual_err_x, prev_err_x);
            //if (-200 < actual_err_x && actual_err_x < 200){
              xPID_i = xPID_i + (xki * integration(period, actual_err_x));
            //} else {
              //xPID_i = 0;
            //}

            xPID_total = xPID_p + xPID_i + xPID_d;

            servoX.write(map(-xPID_total, -320, 320, 0, 180));

            prev_err_x = actual_err_x;
            
            
            //=========================
            if (millis() > 20000){
              actual_err_y = actualCoords.y;

              yPID_p = ykp * actual_err_y;
              yPID_d = ykd * derivation(period, actual_err_y, prev_err_y);

              //if (-120 > actual_err_y && actual_err_y < 120){
                yPID_i = yPID_i + (yki * integration(period, actual_err_y));
              //} else {
                //yPID_i = 0;
              //}

              yPID_total = yPID_p + yPID_i + yPID_d;

              servoY.write(map(yPID_total, -240, 240, 60, 140));

              prev_err_y = actual_err_y;
            }
            
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