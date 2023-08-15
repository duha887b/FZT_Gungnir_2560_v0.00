#include <Arduino.h>
#include "pages.h"
#include "stepper.h"

#define MOTOR_Y_ENABLE_PIN 49
#define MOTOR_Y_STEP_PIN 44
#define MOTOR_Y_DIR_PIN 51

void setup(){
    
    //Serial.println("Startet");

    setupPages();
    landingPage();
    runPage();

    pinSetup(MOTOR_Y_ENABLE_PIN,MOTOR_Y_DIR_PIN,MOTOR_Y_STEP_PIN);
    enableMotor(0);
    setDir(0);

    

    stepper_TimerInteruptModeSetup();
    setFrequenz_Timer1(10000);
    stepper_timerModeRun();

}

void loop(){
    //main loop ....
    startTouch();
    
    

}