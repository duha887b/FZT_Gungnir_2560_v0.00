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

void draw_speed();
void draw_jog(){
}
void draw_setZero();
void draw_home();

void start_menu(){



  
    probePage();

   
}

void refresh_interface(){



  switch (get_lockT1())
  {
  case 0:
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
    
  

  
    break;
  
  case 1:
    value_shift_L(positionL);
    break;
  }

  switch (get_lockT2())
  {
  case 0:

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
  

    break;
  
  case 1:
    value_shift_S(positionC);
    break;
  }

  
 

  //Serial.print(get_lockT1()  );
  //Serial.print(  get_lockT2());
  //Serial.println("");

  if(refreschValues_L){
    refreschValues_L = false;
    reset_count_t1(positionL);
  }

  if(refreschValues_S){
    refreschValues_S = false;
    reset_count_t2(positionC);

  }
    
    

    
}

void value_shift_L(int pos){
  switch (pos)
  {
    case 0:
      break;

    case 1:
      break;

    case 2:
      break;

    case 3:
      break;

    case 4: 
      break;

    default:
      break;
    }

}

void value_shift_S(int pos){
  switch (pos)
  {
    case 0:
      break;

    case 1:
      break;

    case 3:
      break;

    default:
      break;


  }
  
}

