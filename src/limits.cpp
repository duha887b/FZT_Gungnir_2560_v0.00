/*

limits.cpp - controle limits
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
#include "limits.h"
#include "stepper.h"

//TODO debounce switches (done with hardware)

volatile bool limit_TOP = 1;
volatile bool limit_BOTTON = 1;

bool crashFlag = 0;

bool get_limitTop(){
    return limit_TOP;
}

bool get_limitBottom(){
    return limit_BOTTON;
}
int i = 0;
void ISR_TOP(){ // ISR 
    
    //stepper_timerModeStop(); // stop Motor
    limit_TOP = digitalRead(sw_TOP);

    if(getDir() == UP && !limit_TOP ){            //crash detection 
        stepper_timerModeStop();
        crashFlag = true;
        return;
    }
    

    

}

void ISR_BOTTOM(){ //ISR 
    //stepper_timerModeStop(); //stop Motor
    limit_BOTTON = digitalRead(sw_BOTTOM);
    //Serial.print("bootom");
    if(getDir() == DOWN && !limit_BOTTON){            //crash detection 
        stepper_timerModeStop();
        //Serial.print("stop");
        crashFlag = true;
        return;
    }
    
}

// FIXME interrupt levels
void setup_limits(){
    pinMode(sw_TOP,INPUT);   //setup pins
    pinMode(sw_BOTTOM,INPUT);

    limit_BOTTON = digitalRead(sw_BOTTOM);
    limit_TOP = digitalRead(sw_TOP);
    //Serial.begin(115200);
    //Serial.println("inidone");

    attachInterrupt(digitalPinToInterrupt(sw_TOP),ISR_TOP,CHANGE);      //setup pin interupts
    attachInterrupt(digitalPinToInterrupt(sw_BOTTOM),ISR_BOTTOM,CHANGE);


}


