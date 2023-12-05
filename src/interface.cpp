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
float zero_positionY;

bool refreschValues_L;
bool refreschValues_S;

volatile bool block_lock1;
volatile bool block_lock2;

float linSpeed = 0.025;
float spoolSpeed = 2;
float spoolDiameter = 100;
float linJogDistance = 0;

float tmp_count_s = 0 ;
float tmp_count_y = 0;

bool lin_jumpBack = false;
bool spool_jumpBack = false;

float tmp_position_y = 0;

int count_delay = 0;


// Array von Funktionspointern
    

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
  


  if(linSpeed<0){
    linSpeed=0;
    reset_count_t1(positionL);
  }

  if(get_count_t1() != tmp_count_y){
    linSpeed += ((get_count_t1()- positionL - tmp_count_y)*0.005);
    tmp_count_y = get_count_t1();
    probe_setSpeed(linSpeed);
  }

  



  //TODO wert auf motor schreiben 
  if(!get_lockT1()){
    
    //set speed hard
    block_lock1 = false;
    reset_count_t1(positionL);
    tmp_count_y = 0 ;
  }

}

void subLin_Zero(){
    zero_positionY = (float)get_positionY();
    probe_setZero(zero_positionY);
    set_positionCounterY(0);
    updatePosition();
    probe_setPosition((float)get_positionY());

    block_lock1 = false;
    set_lockT1(false);
    reset_count_t1(positionL);
}

void subLin_Jog(){
  
}

void subLin_home(){
  block_lock1 =  true;

  if(!lin_jumpBack){
    set_speedY((HOME_SPEED));
    run_MotorY(true);
  }

  if(get_limitTop()){
    run_MotorY(false);
    set_positionCounterY(0);
    updatePosition();
    probe_setPosition(get_positionY());
    block_lock1 = false;
    set_lockT1(false);
    
  }




}

void subLin_START(){
  block_lock1 = true;

  if(!lin_jumpBack){
    probe_setStart(true);
    set_speedY(-1 * linSpeed);
    run_MotorY(true);
    probe_setSpeed(linSpeed);
    tmp_count_y = positionL;

    lin_jumpBack = true;
  }

  if(get_count_t1() != tmp_count_y){
    linSpeed += ((get_count_t1()- positionL - tmp_count_y)*0.005);
    tmp_count_y = get_count_t1();
    probe_setSpeed(linSpeed);

  }
  

    

  

  if((!get_lockT1() && get_count_t1() == positionL) || get_limitBottom()){
    
    
    block_lock1 = false;
    set_lockT1(false);
    run_MotorY(false);
    probe_setStart(false);
    reset_count_t1(positionL);
    lin_jumpBack = false;
    tmp_count_y = 0 ;


  }


  


      
  

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
  
  
  
  
  
  if(spoolSpeed<0){
    spoolSpeed=0;
    reset_count_t2(positionC);
  }

  if(get_count_t2() != tmp_count_s){
    spoolSpeed += ((get_count_t2()- positionC- tmp_count_s )*0.5);
    tmp_count_s = get_count_t2();
    spool_setSpeed(spoolSpeed);
  }
  



  //TODO wert auf motor schreiben 
  if(!get_lockT2()){
    
    //set speed hard
    block_lock2 = false;
    reset_count_t2(positionC);
    tmp_count_s = 0;
  }
}

void subSpool_diameter(){
  block_lock2 = true;

  

  if(spoolDiameter<0){
    spoolDiameter=0;
    reset_count_t2(positionC);
  }

  if(get_count_t2() != tmp_count_s){
    spoolDiameter += ((get_count_t2()- positionC- tmp_count_s )*1);
    tmp_count_s = get_count_t2();
    spool_setDiameter(spoolDiameter);
  }
  

  if(!get_lockT2()){
    block_lock2 = false;
    reset_count_t2(positionC);
    tmp_count_s = 0;
  }

}

void subSpool_Jog(){

}

void subSpool_START(){
  block_lock2 = true;

  if(!spool_jumpBack){
    set_positionCounterS(0);
    updatePosition();
    spool_setPosition(get_positionS());
    set_speedS(spoolSpeed);
    run_MotorS(true);
    spool_setStart(true);
    spool_jumpBack = true;
  }

  if(!get_lockT2()){
    reset_count_t2(positionC);
    block_lock2 = false;
    run_MotorS(false);
    spool_jumpBack = false;
    tmp_count_s = 0;
  }

}



void (*funktionsListe_SPOOL[])(void) = {main_spool, subSpool_speed, subSpool_diameter,subSpool_Jog,subSpool_START};

void start_menu(){

  probePage();
  block_lock1 = false;
  block_lock2 = false;
  positionC = 0;
  positionL = 0;
  tmp_count_s = 0;
  tmp_count_y = 0;

  spool_setDiameter(spoolDiameter);
  spool_setSpeed(spoolSpeed);
  probe_setSpeed(linSpeed);

  

}

int tmp_t = 40;
int tmp_b = 0;
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

if(tmp_b == tmp_t){
  probe_setPosition((float)get_positionY());
  
  
}

if(tmp_b == 2*tmp_t){
  spool_setPosition((float)get_positionS());
  tmp_b = 0;
  
}
tmp_b++;

updatePosition();
//probe_setPosition(get_positionY());
//Serial.println((float)get_positionY());



//Serial.print(get_lockT1() && !block_lock1);
//Serial.print(get_lockT2() && !block_lock2);
//Serial.println("");
  
}


