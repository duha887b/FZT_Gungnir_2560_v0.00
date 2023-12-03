/*

timer_controle.cpp - setup all timers and handling coresponding interrupts
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


#include "timer_controle.h"



float TOP[5]; //TOP S157 Manuel (compare Value)
int N[5] = {1,8,64,256,1024}; //Prescalar


long * setupt_timerValuesFrequenz(unsigned int f){
  static long NTop[2]; 

  if (f>MAX_F){
    return NULL; // Fehler
  }

  for(int i = 0; i<(int)(sizeof(TOP)/sizeof(float));i++){  
        
        TOP[i] = ((float)F_CPU/((float)f*(float)N[i])-1);   // calc TOP for each combination 
        //Serial.println(TOP[i]);
        if ((TOP[i]> 65535) || (TOP[i]<1)){                                // check overflow on 16bit Register 
            TOP[i] = -1;
        }
        //Serial.println(TOP[i]);
    }

    float tmp_1 = 1;
    
    for(int i = 0;i<(int)(sizeof(TOP)/sizeof(float));i++ ){ //find nearest matching frequenz
        float tmp;
        int flag = 0; //aufrunden

        if(TOP[i] == -1){continue;}

        tmp = TOP[i] - (long int)TOP[i];
        if(tmp>0.5){
            tmp = 1-tmp;
            flag = 1;
        }else{
            flag = 0;
        }

        if(tmp<tmp_1){
            NTop[0] = N[i];
            NTop[1] = (long int)TOP[i] + flag;
            tmp_1 = tmp;
            
        }
        if (tmp == tmp_1){
            if(NTop[1]>(long int)TOP[i] + flag){
                NTop[0] = N[i];
                NTop[1] = (long int)TOP[i] + flag;
            }
        }
        
    }



  Serial.println(NTop[0]);
  Serial.println(NTop[1]);

  return NTop;

}

//TODO frquenzberechnung kerrekt machen und Vorteiler setzten lassen 
const int interruptfrequenzy_Timer1 = 35; //Hz

//use  Timer/Counter 1 
void setup_timer1(){

noInterrupts();
    
uint16_t compareValTimer1 = F_CPU/(64*interruptfrequenzy_Timer1) - 1; // Prescalar 64 muss gestzt werden 

// Timer 1 Register config
  TCCR1A = 0;                                           // Setzt die Timer-Steuerregister A auf 0 (normale Operation)
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);    // CTC-Modus, Prescalar auf 64
  OCR1A = compareValTimer1;                             // Setzt den Compare-Wert

  TIMSK1 &= (0 << OCIE1A);                              // Interrupt sperren 

  interrupts();
}
void start_timer1(bool c){
  c ? TIMSK1 |= (1 << OCIE1A) : TIMSK1 &= (0 << OCIE1A);  
}

//ISR Timer 1 
ISR(TIMER1_COMPA_vect) { 
  update_turnpuls1();
  update_turnpuls2();
  refresh_interface();
  //Serial.println(get_limitTop());
}

// Timer/Counter 4
void setupt_timer2(){
    noInterrupts();

    TCCR4A = 0; 
    TCCR4B |= (1 << CS40);  // Prescalar 1 default
    TCCR4B |= (1 << WGM42); // CTC-Modus
  
    TIMSK4 &= (0 << OCIE4A); // Interrupt of

    interrupts();


}

void start_timer2(bool c){
  c ? TIMSK4 |= (1 << OCIE4A) : TIMSK4 &= (0 << OCIE4A);  
}

// Timer 2 Compare-A Interrupt-Routine
ISR(TIMER4_COMPA_vect) {
  step_y();

}

void set_timer2(unsigned int f){
  long * tmp = setupt_timerValuesFrequenz(f);
  long i = 1;

  OCR4A = *(tmp+i);

  TCCR4B &= 0xF8;  //zero bit 0-2

  switch (*tmp) // set prescalar
    {
    case 1:
        TCCR4B |= 0x01; //N prescalar S157 Manuel
        break;
    case 8:
        TCCR4B |= 0x02; //N prescalar S157 Manuel
        break;

    case 64:
        TCCR4B |= 0x03; //N prescalar S157 Manuel
        break;
    case 256:
        TCCR4B |= 0x04; //N prescalar S157 Manuel
        break;
    case 1024:
        TCCR4B |= 0x05; //N prescalar S157 Manuel
        break;
    }

}

void setup_timer3(){

  TCCR3A = 0;              // Setzt die Timer-Steuerregister A auf 0 (normale Operation)
  TCCR3B |= (1 << CS30);  // Prescalar 1 default
  TCCR3B |= (1 << WGM32); // CTC-Modus
  
  TIMSK3 &= (0 << OCIE3A); // Interrupt of       


  // Globale Interrupts aktivieren
  interrupts();
}

void start_timer3(bool c){
  c ? TIMSK3 |= (1 << OCIE3A) : TIMSK3 &= (0 << OCIE3A);  
}

void set_timer3(unsigned int f){
  long * tmp = setupt_timerValuesFrequenz(f);
  long i = 1;

  OCR3A = *(tmp+i);

  TCCR3B &= 0xF8;  //zero bit 0-2

  switch (*tmp) // set prescalar
    {
    case 1:
        TCCR3B |= 0x01; //N prescalar S157 Manuel
        break;
    case 8:
        TCCR3B |= 0x02; //N prescalar S157 Manuel
        break;

    case 64:
        TCCR3B |= 0x03; //N prescalar S157 Manuel
        break;
    case 256:
        TCCR3B |= 0x04; //N prescalar S157 Manuel
        break;
    case 1024:
        TCCR3B |= 0x05; //N prescalar S157 Manuel
        break;
    }
}
ISR(TIMER3_COMPA_vect) { 
  step_s();
}