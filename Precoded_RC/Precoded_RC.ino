#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"
const int M_pin = A1;
int digitalValue;

DeviceDriverSet_Motor AppMotor;
Application_xxx Application_ConquerorCarxxx0;
MPU6050_getdata AppMPU6050getdata;

// sets the structure as well as state as being an integer 
struct StateInfo {
  int state;
  unsigned long duration;
  ConquerorCarMotionControl status;
  int speed;  // Speed for each state
};
// {state, time in sec, direction, speed},
// all lowercase
StateInfo stateDurations[] = {
  {0, 3, forward, 250},  
  {1, 2, right, 200},    
  {2, 3, forward, 250},  
  {3, 4, left, 150},     
  {4, 1, forward, 250},  
};

void setup() {
  Serial.begin(9600);
  AppMotor.DeviceDriverSet_Motor_Init();
  AppMPU6050getdata.MPU6050_dveInit();
  delay(1500);
  AppMPU6050getdata.MPU6050_calibration();
  pinMode(M_pin, INPUT);
}

void loop() {
  static int state = 0;  // Initialize the state to 0
  static unsigned long previousTime = 0;  // Initialize the timer
  unsigned long currentTime = millis() / 1000;// remove if you want to make millisecs again
  digitalValue = digitalRead(M_pin);

  Serial.print(" M_value: ");
  Serial.println(digitalValue);
  if (digitalValue == HIGH){
    ApplicationFunctionSet_ConquerorCarMotionControl(stop, 0);
    ApplicationFunctionSet_ConquerorCarMotionControl(backward, 200);
    delay(500);
    ApplicationFunctionSet_ConquerorCarMotionControl(stop, 0);
    state = 100; // this state is outside of the range and therefore will skip to stoping forever
  }
  else if (state >= 0 && state < 5) {// you need to add the amount of states +1 to give range
  //EX Two states = state >= 0 && state < 3
    // Check if it's time to transition to the next state
    if (currentTime - previousTime >= stateDurations[state].duration) {
      previousTime = currentTime;

      // Transition to the next state
      state++;
    }
  } else {
    ApplicationFunctionSet_ConquerorCarMotionControl(stop, 0);
  }

  // Use the 'state' variable to determine the current state's motion control and speed
  ApplicationFunctionSet_ConquerorCarMotionControl(stateDurations[state].status, stateDurations[state].speed);
}


































