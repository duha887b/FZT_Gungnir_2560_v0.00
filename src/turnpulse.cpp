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
volatile bool lock_t2 = 1;

bool interrupt_lock1 = 0; // T1
bool interrupt_lock2 = 0; // T1 switch
bool interrupt_lock3 = 0; // T2
bool interrupt_lock4 = 0; // T2 switch

unsigned long lastInterruptTime1 = 0; // T1
unsigned long lastInterruptTime2= 1; // T1 switch
unsigned long lastInterruptTime3 = 0; // T2
unsigned long lastInterruptTime4 = 1; //T2 switch

unsigned int debounce = 150; // Entprellzeit in ms
unsigned int debounce_t = 50; // Entprellzeit in ms

bool get_lockT1(){
  return lock_t1;
}
int get_count_t1(){
  return count_t1;
}

void reset_count_t1(int o){
  count_t1 = o;
}

bool get_lockT2(){
  return lock_t2;
}
int get_count_t2(){
  return count_t2;
}

void reset_count_t2(int o){
  count_t2 = o;
}


void ISR_T1A(){

    if(millis() - lastInterruptTime1 > debounce_t){
     digitalRead(T1_B) ? count_t1-- : count_t1++; 
     lastInterruptTime1 = millis();

     //Serial.println(count_t1);
    }

}

void ISR_switch_T1(){

    if(millis() - lastInterruptTime2 > debounce){
    lock_t1 = !lock_t1;
    lastInterruptTime2 = millis();

    //Serial.println(lock_t1);
    }

    
}

void ISR_T2A(){

    if(millis() - lastInterruptTime3 > debounce_t){
    digitalRead(T2_B) ? count_t2-- : count_t2++;
    lastInterruptTime3 = millis();

    //Serial.println(count_t2);
    }
    

}

void ISR_switch_T2(){

  if(millis() - lastInterruptTime4 > debounce){
  lock_t2 = !lock_t2;
  lastInterruptTime4 = millis();

  //Serial.println(lock_t2);
  }
}

void setup_turnimpuls(){
  pinMode (T1_A, INPUT_PULLUP);
  pinMode (T1_B, INPUT_PULLUP);
  pinMode (T1_Switch, INPUT_PULLUP);

  pinMode (T2_A, INPUT_PULLUP);
  pinMode (T2_B, INPUT_PULLUP);
  pinMode (T2_Switch, INPUT_PULLUP);

  count_t1 = 0;
  count_t2 = 0;

 

  attachInterrupt(digitalPinToInterrupt(T1_A),ISR_T1A,RISING);
  attachInterrupt(digitalPinToInterrupt(T1_Switch),ISR_switch_T1,FALLING);

  attachInterrupt(digitalPinToInterrupt(T2_A),ISR_T2A,RISING);
  attachInterrupt(digitalPinToInterrupt(T2_Switch),ISR_switch_T2,FALLING);

  lock_t1 = 0;
  lock_t2 = 0;


}

