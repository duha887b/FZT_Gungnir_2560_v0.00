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

void draw_speed();
void draw_jog(){
}
void draw_setZero();
void draw_home();

void start_menu(){



  
    probePage();

   
}

void refresh_interface(){

  positionL = get_count_t1();
  positionC = get_count_t2();

  if(positionL < 0){
    positionL = 0;
    reset_count_t1(0);
  }

  if(positionC < 0){
    positionC = 0;
    reset_count_t2(0);
  }

  if(positionL > 4){
    positionL = 4;
    reset_count_t1(4);
  }

  if(positionC > 3){
    positionC = 3;
    reset_count_t2(3);
  }

  draw_cursor(0,positionL);
  draw_cursor(1,positionC);

  Serial.print(positionL  );
  Serial.print(  positionC);
  Serial.println("");
    
    

    
}

