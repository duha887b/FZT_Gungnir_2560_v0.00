#ifndef PAGES_HH
#define PAGES_HH

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Arduino.h>
#include <TouchScreen.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


 


void setupPages();
void landingPage();
void jogPage();
void setZeroPage();
void runPage();
void startTouch();
void vars_Step(bool DIR,int speed);


#endif