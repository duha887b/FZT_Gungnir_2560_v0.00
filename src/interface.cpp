/*

interface.cpp - controle user Input
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

#include "interface.h"



char page;
char str[100];
int positionL;
int positionC;

bool refreschValues_L;
bool refreschValues_S;

volatile bool block_lock1;
volatile bool block_lock2;

float linSpeed = 0;
float spoolSpeed = 0;


// Array von Funktionspointern
    

void draw_speed();
void draw_jog(){
}
void draw_setZero();
void draw_home();


void main_Lin(){
    positionL = get_count_t1();

    if(positionL < 0){
    positionL = 0;
    reset_count_t1(0);
    }

    if(positionL > 4){
    positionL = 4;
    reset_count_t1(4);
    }

    draw_cursor(0,positionL);
}

void subLin_speed(){
  block_lock1 = true;
  

  //TODO auf bestehenden wert zurückgreifen
  linSpeed = ((get_count_t1()- positionL )*0.5);

  if(linSpeed<0){
    linSpeed=0;
    reset_count_t2(positionL);
  }

  probe_setSpeed(linSpeed);



  //TODO wert auf motor schreiben 
  if(!get_lockT1()){
    
    //set speed hard
    block_lock1 = false;
    reset_count_t1(positionL);
  }

}

void subLin_Zero(){

}

void subLin_Jog(){

}

void subLin_home(){

}

void subLin_START(){

}

void (*funktionsListe_LIN[])(void) = {main_Lin, subLin_speed, subLin_Zero,subLin_Jog,subLin_home,subLin_START};


 void main_spool(){

  positionC = get_count_t2();

  if(positionC < 0){
    positionC = 0;
    reset_count_t2(0);
  }

  
  if(positionC > 3){
    positionC = 3;
    reset_count_t2(3);
  }
  draw_cursor(1,positionC);
}

void subSpool_speed(){
   block_lock2 = true;
  

  //TODO auf bestehenden wert zurückgreifen
  spoolSpeed = ((get_count_t2()- positionC )*1);
  if(spoolSpeed<0){
    spoolSpeed=0;
    reset_count_t2(positionC);
  }
  spool_setSpeed(spoolSpeed);



  //TODO wert auf motor schreiben 
  if(!get_lockT2()){
    
    //set speed hard
    block_lock2 = false;
    reset_count_t2(positionC);
  }
}

void subSpool_diameter(){

}

void subSpool_Jog(){

}

void subSpool_START(){

}

void (*funktionsListe_SPOOL[])(void) = {main_spool, subSpool_speed, subSpool_diameter,subSpool_Jog,subSpool_START};

void start_menu(){

  probePage();
  block_lock1 = false;
  block_lock2 = false;
  positionC = 0;
  positionL = 0;

}

void refresh_interface(){

if(get_lockT1() || block_lock1){
  funktionsListe_LIN[positionL+1]();
  
}else{
  funktionsListe_LIN[0]();
}

if(get_lockT2() || block_lock2){
  funktionsListe_SPOOL[positionC+1]();
  
}else{
  funktionsListe_SPOOL[0]();
}

//Serial.print(get_lockT1() && !block_lock1);
//Serial.print(get_lockT2() && !block_lock2);
//Serial.println("");
  
}


