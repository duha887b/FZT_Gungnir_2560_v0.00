/*

stepper.h - interface for stepper.cpp
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

#ifndef STEPPER_HH
#define STEPPER_HH


#include <Arduino.h>
#include "limits.h"
#include "timer_controle.h"

#define MOTOR_S_ENABLE_PIN 49
#define MOTOR_S_STEP_PIN 44
#define MOTOR_S_DIR_PIN 51
#define MOTOR_S_MICROSTEPPING 400
#define MOTOR_S_MM_PER_U 314  // pi*diameter of spool // TODO update out of interface 

#define MOTOR_Y_ENABLE_PIN 30
#define MOTOR_Y_STEP_PIN 33
#define MOTOR_Y_DIR_PIN 32
#define MOTOR_Y_MICROSTEPPING 200
#define MOTOR_Y_MM_PER_U 3

#define DOWN 1
#define UP 0

void pinSetup();

void step(int step_pin);
bool get_ENA_Y();
bool get_ENA_S();
bool get_DIR_Y();
bool get_DIR_S();

void set_ENA_Y(bool s);
void set_ENA_S(bool s);
void set_DIR_Y(bool s);
void set_DIR_S(bool s);


void updatePosition(); //update positiion VAlues of S and Y

long get_positionY();
long get_positionS();

void set_positionCounterY(int64_t n);
void set_positionCounterS(int64_t n);

int umin_to_frequenz(float speed,float mmPerTurn, int mstep); // calculate frequenz for diffrent diameters

void set_speedY(float speed);
void set_speedS(float speed);

void run_MotorY(bool r);
void run_MotorS(bool r);


#endif