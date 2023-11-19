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
#include "timer_controle.h"

// TODO Steuerung Spulenmotor
// FIXME Kommentierung


void setup(){

    

    Serial.begin(115200);
    Serial.println("Startet");

    setup_timer1();
    setupt_timer2();
    setup_timer3();


    setup_limits();
    setupPages();
    landingPage();
    setup_turnimpuls();

    reset_count_t1(0);
    reset_count_t2(0);


    start_menu();
    
    sei(); // globale interrupt freigabe; 
    start_timer1(); //interrupt freigabe
    

}

void loop(){
}


