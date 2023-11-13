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

char16_t cursor_Page1;
char16_t cursor_Page2;

char page;
char str[100];

void draw_speed();
void draw_jog(){
  probe_setJog(true);
  while(get_lockT1()){
    if(get_count_t1()<0){
      moveRelative(-3,10);
      reset_count_t1();
      probe_setPosition(updatePosition());
      
      
    }

    if(get_count_t1()>0){
      moveRelative(3,10);
      reset_count_t1();
      probe_setPosition(updatePosition());
      
    }

    
  }
  probe_setJog(false);
}
void draw_setZero();
void draw_home();

void start_menu(){

    probePage();
    page = 0x01;
    cursor_Page1 = 0x01;
    
    while(true){
        if(page == 0x1){

          if(get_lockT1()){

            
            while (get_lockT1())
            {
              switch (cursor_Page1)
              {
              case 0x01:
                  //page PROBE
                  break;
              case 0x02:
                  //page RUN
                  break;
              case 0x04:
                  //draw_speed();
                  break;

              case 0x08:
                  draw_jog();
                  break;

              case 0x10:
                  //draw_setZero();
                  break;

              case 0x20:
                  //draw_home();
                  break;
              
              default:
                break;

              }
            }

            reset_count_t1();
            
          }
            
           if((get_count_t1() < 0)){
                if(cursor_Page1 != 0x20){

                cursor_Page1 = cursor_Page1 << 1;
                draw_cursor1(cursor_Page1);
                Serial.println((int)cursor_Page1);  
                }
                reset_count_t1();
           }
           if((get_count_t1()> 0 )){
                if(cursor_Page1 != 0x01){
                    
                cursor_Page1 = cursor_Page1 >> 1;
                draw_cursor1(cursor_Page1);
                Serial.println((int)cursor_Page1);
                }
                reset_count_t1();
           }
           
            
        }

    }

    

}

