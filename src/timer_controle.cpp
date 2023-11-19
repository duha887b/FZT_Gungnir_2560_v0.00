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

//TODO frquenzberechnung kerrekt machen und Vorteiler setzten lassen 
const int interruptfrequenzy_Timer1 = 15; //Hz

void setup_timer1(){

noInterrupts();
    
uint16_t compareValTimer1 = F_CPU/(2*64*interruptfrequenzy_Timer1) - 1; // Prescalar 64 muss gestzt werden 

// Timer 1 Register config
  TCCR1A = 0;                                           // Setzt die Timer-Steuerregister A auf 0 (normale Operation)
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);    // CTC-Modus, Prescalar auf 64
  OCR1A = compareValTimer1;                             // Setzt den Compare-Wert

  TIMSK1 |= (0 << OCIE1A);                              // Interrupt sperren 

  interrupts();
}
void start_timer1(){
  TIMSK1 |= (1 << OCIE1A);  
}

//ISR Timer 1 
ISR(TIMER1_COMPA_vect) { 
  refresh_interface();
  //Serial.println("timer1");
}

void setupt_timer2(){
    noInterrupts();

    TCCR2A = (1 << WGM21); // CTC-Modus
    TCCR2B = (1 << CS20);  // Prescalar 1
    
    OCR2A = F_CPU/(2*1*1) - 1;  // Frequenz setzen
    OCR2B = F_CPU/(2*1*2) - 1;  // Frequenz setzen

   // TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B); // A/B Vector Interrupt aktivieren

    interrupts();


}

// Timer 2 Compare-A Interrupt-Routine
ISR(TIMER2_COMPA_vect) {
  //Serial.println("A_INT");
}

// Timer 2 Compare-B Interrupt-Routine
ISR(TIMER2_COMPB_vect) {
 // Serial.println("B_INT");
}

void setup_timer3(){

  TCCR3A = 0;              // Setzt die Timer-Steuerregister A auf 0 (normale Operation)
  TCCR3B = (1 << WGM32) |   // CTC-Modus
           (1 << CS32) |    // Vorteiler 256 (andere Vorteiler sind auch möglich)
           (1 << CS30);     // Vorteiler 256 (andere Vorteiler sind auch möglich)

  // Berechne den Compare-Wert für eine Frequenz von 10 Hz
  // (Hier wird 16 MHz durch 2 * 256 * 10 geteilt für eine Timerfrequenz von 31250 Hz)
  uint16_t compareValue = 62500 / 1 - 1;

  // Setze den Compare-Wert
  OCR3A = compareValue;

  // Timer 3 Compare-A Interrupt aktivieren
  TIMSK3 |= (1 << OCIE3A);

  // Globale Interrupts aktivieren
  interrupts();
}

ISR(TIMER3_COMPA_vect) {
  Serial.println("timer3");
}