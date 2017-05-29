
#include "Arduino.h"
#include <NewPing.h>
#include <Wire.h>

// the i2c addess used by both the master and slave device
#define SLAVE_ADDRESS 0x04

// sensor board pinout
// sensor : trigger : Echo
// 1      : 2       : 5
// 2      : 7       : 6
// 3      : 8       : 3
// 4      : 9       : 4

//Sensor 1
#define TRIGGER_PIN_S1 2
#define ECHO_PIN_S1 5      // you  can use a single pin for trrigger/echo if the bord is a 5v one.
//Sensor 2
#define TRIGGER_PIN_S2 7
#define ECHO_PIN_S2 6
//Sensor 3
#define TRIGGER_PIN_S3 8
#define ECHO_PIN_S3 3
//Sensor 4
#define TRIGGER_PIN_S4 9
#define ECHO_PIN_S4 4

#define MAX_DISTANCE 200 // max distance in cm. same for all



//initialise the sensors
NewPing sonar1(TRIGGER_PIN_S1, ECHO_PIN_S1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_S2, ECHO_PIN_S2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN_S3, ECHO_PIN_S3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN_S4, ECHO_PIN_S4, MAX_DISTANCE);

// if you change the sonars array change
// this number as well..
// remember it is a zero based array
const int NUMBER_OF_SENSORS =4;

// create an array of sensors
NewPing sonars[NUMBER_OF_SENSORS]={
  sonar1,
  sonar2,
  sonar3,
  sonar4
};


int returnValueForI2c = 0;


//initialise the sensors values
//
// int sensor1_val =23;
// int sensor2_val =16;
// int sensor3_val =256;
// int sensor4_val =33;

int sensorValues[4]={0,0,0,0};
// callback for received data
void receiveData(int byteCount){

 // Serial.print("byteCount: ");
 // Serial.println(byteCount);
while(Wire.available()) {
    //c = Wire.read();
    int num = Wire.read();
    Serial.print("data received: ");
    Serial.println(num);

    switch (num) {
      case 1:  returnValueForI2c = sensorValues[0];
      break;

      case 2:returnValueForI2c = sensorValues[1];
      break;

      case 3:returnValueForI2c = sensorValues[2];
      break;

      case 4: returnValueForI2c = sensorValues[3];
      break;

      default :
      returnValueForI2c = 0;
    }

    }
}


// callback for sending data
void sendData(){
  Wire.write(returnValueForI2c);
}


void setup() {
  Serial.begin(115200); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void printValue(int sensorId,int distance){
  Serial.print("PingSensor");
  Serial.print(++sensorId);
  Serial.print(": ");
  Serial.print(distance);
    Serial.println("cm");
}
void loop() {

  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    int tempval = sonars[i].ping_cm();

    //only change value if it has changed.
    if (tempval != sensorValues[i]) {
      //print values to serial
      printValue(i,tempval);
      //set values
      sensorValues[i] = tempval;
    }
  }
  delay(100);

}
