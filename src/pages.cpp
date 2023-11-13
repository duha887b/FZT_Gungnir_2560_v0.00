/*

pages.cpp - display read/write
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


const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 806, TS_RT = 163, TS_TOP = 958, TS_BOT = 168;
enum page{zero,jog,run}currentPage;



MCUFRIEND_kbv tft;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button zero_btn, jog_btn,run_btn;

int pixel_x, pixel_y;
//Touch_getXY() updates global vars
/*
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

*/

void setupPages(){
    // Reading TFT ID:
    uint16_t ID=tft.readID();
                                //Initializing TFT display:
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);

    tft.setRotation(1);
    tft.fillScreen(BLACK);
    

}

void landingPage(){
    tft.setCursor(165,160);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("Gungnir_00.0");
    delay(500);
    tft.fillScreen(BLACK);
    tft.drawLine(0,0,180,0,WHITE);
    tft.drawLine(180,0,180,50,WHITE);
    tft.drawLine(180,50,MAX_X,50,WHITE);
    tft.drawLine(0,0,0,MAX_Y,WHITE);
    tft.drawLine(MAX_X,50,MAX_X,MAX_Y,WHITE);
    tft.drawLine(0,MAX_Y,MAX_X,MAX_Y,WHITE);
    tft.drawLine(MAX_X/2,50,MAX_X/2,MAX_Y,WHITE);
    //tft.drawLine(0,50,MAX_X,50,WHITE);

    tft.setCursor(10,15);
    tft.setTextSize(3);
    tft.print("PROBE RUN");

    tft.setCursor(400,15);
    tft.setTextSize(2);
    tft.print("v00.0");

    


}

const unsigned int hight = 22;
const unsigned int width = 78;
unsigned int cursor_x= 146;
unsigned int cursor_y=126 - 50;

//TODO refrector so it can be used for all pages and sides 
void draw_cursor1(char16_t c){      //curosr animatin 
    
    cursor_x= 146;
    cursor_y= 126;

    

    char16_t tmp = 0x04;
    for (int i = 0; i<5 ; i++){

        tft.drawRect(cursor_x,cursor_y,width,hight,BLACK);
        if(c <= 0x02){
            return;}

        if(tmp == c){
            tft.drawRect(cursor_x,cursor_y,width,hight,CYAN);
            Serial.println((int)tmp);
        }

        tmp = tmp <<1;
        cursor_y += 50;
    }
}


void probePage(){
    tft.drawRect(5,10,96,31,GREEN); 
    tft.drawRect(4,9,98,33,GREEN);

    tft.setCursor(10,55);
    tft.setTextSize(1);
    tft.print("z-Achse");

    tft.setTextSize(2);

    tft.setCursor(10,80);
    tft.print("Z-Postion");

    tft.setCursor(10,130);
    tft.print("Speed");
    
    tft.setCursor(10,180);
    tft.print("Z-Jog");

    tft.setCursor(10,230);
    tft.print("Set-Zero");

    tft.setCursor(150,280);
    tft.print("Home");

    tft.setCursor(150,80);
    tft.print("000,00");

    tft.setCursor(150,130);
    tft.print("00,0");
    
    tft.setCursor(150,180);
    tft.print("Stop");

    tft.setCursor(150,230);
    tft.print("000,00");

    
} 

void probe_setPosition(double pos){


    tft.setTextSize(2);
    tft.setCursor(150,80);
    tft.fillRect(150,80,70,17,BLACK);

    tft.setCursor(150,80);
    tft.setTextSize(2);
    tft.print(pos,2);
}

void probe_setSpeed(char sp[10]){
    tft.setCursor(150,150);
    tft.setTextSize(2);
    tft.print(sp);
}

void probe_setJog(bool jog){
    
    tft.setTextSize(2);
    tft.setCursor(150,180);
    tft.fillRect(150,180,70,17,BLACK);

    jog ? tft.print("Run") : tft.print("Stop");
        
        
    
}

void probe_setZero(char zero[10]){

    

    tft.setCursor(150,250);
    tft.setTextSize(2);
    tft.print(zero);
}

void req(){
    tft.fillRect(0,65,320,415,BLACK);
    tft.drawRect(0,65,320,415,WHITE);
    

}

void vars_Step(bool DIR,int sp){
        tft.setTextSize(1);
        tft.setCursor(120,120);
        tft.print(sp);
    }