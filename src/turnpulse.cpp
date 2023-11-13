/*

turnpulse.cpp - turn impulse encoder 
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

#include "turnpulse.h"
#include "stepper.h"

volatile int count_t1 = 0;
volatile int count_t2 = 0;
volatile bool lock_t1 = 1;

bool get_lockT1(){
  return lock_t1;
}
int get_count_t1(){
  return count_t1;
}

void reset_count_t1(){
  count_t1 = 0;
}

void ISR_T1A(){

  

    digitalRead(T1_B) ? count_t1++ : count_t1--;
    
    
    //Serial.println(count_t1);
    //Serial.println(digitalRead(T1_Switch));
    
}

void ISR_switch_T1(){
  
    lock_t1 = !lock_t1;
    Serial.println(lock_t1);
    
}

void ISR_T2A(){



    digitalRead(T1_B) ? count_t1++ : count_t1--;
    //Serial.println(count_t1);
    //Serial.println(digitalRead(T1_Switch));
    
}

void ISR_switch_T2(){
  
    //Serial.println("press");
    //Serial.println(digitalRead(T1_Switch));
    
}

void setup_turnimpuls(){
  pinMode (T1_A, INPUT_PULLUP);
  pinMode (T1_B, INPUT_PULLUP);
  pinMode (T1_Switch, INPUT_PULLUP);

  pinMode (T2_A, INPUT_PULLUP);
  pinMode (T2_B, INPUT_PULLUP);
  pinMode (T2_Switch, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Start");

  count_t1 = 0;
  count_t2 = 0;

  attachInterrupt(digitalPinToInterrupt(T1_A),ISR_T1A,RISING);
  attachInterrupt(digitalPinToInterrupt(T1_Switch),ISR_switch_T1,FALLING);

  attachInterrupt(digitalPinToInterrupt(T2_A),ISR_T2A,RISING);
  attachInterrupt(digitalPinToInterrupt(T2_Switch),ISR_switch_T2,FALLING);


}

