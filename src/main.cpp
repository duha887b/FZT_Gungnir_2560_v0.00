/*

main.cpp - main loop
Part of FZTg for Atmega2560 based controller

MIT License

Copyright (c) 2023 Dustin Hanusch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "pages.h"
#include "stepper.h"
#include "limits.h"
#include "turnpulse.h"
#include "interface.h"

// TODO Steuerung Spulenmotor
// FIXME Kommentierung


void setup(){
    //Serial.begin(115200);
    
    //Serial.println("Startet");

   /*

    setupPages();
    landingPage();

    pinSetup(MOTOR_Y_ENABLE_PIN,MOTOR_Y_DIR_PIN,MOTOR_Y_STEP_PIN);
    enableMotor(0);
    setDir(0);

    stepper_TimerInteruptModeSetup();
    stepper_timerModeStop();

    setup_turnimpuls();
*/

    //pinSetup(MOTOR_Y_ENABLE_PIN,MOTOR_Y_DIR_PIN,MOTOR_Y_STEP_PIN);
    pinSetup();
    enableMotor(0);
    setDir(0);

    stepper_TimerInteruptModeSetup();
    stepper_timerModeStop();

    
    

    Serial.begin(115200);
}

void loop(){
    long int tmp = millis();
    stepper_timerModeRun();
    set_stepperSpeed(1);
    Serial.println("START");
    int i = 1;
    while (1)
    {
        if(millis()-tmp>1000){
            set_stepperSpeed(i);
            Serial.println(i);
            tmp = millis();
            i++;
        }
    }
    
   
   }


