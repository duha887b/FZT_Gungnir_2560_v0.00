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

#define MOTOR_Y_ENABLE_PIN 49
#define MOTOR_Y_STEP_PIN 44
#define MOTOR_Y_DIR_PIN 51

void pinSetup(unsigned int pena, unsigned int pdir,unsigned int ppuls);

void step();

void enableMotor(bool ena);
void setDir(bool dir);

void stepper_TimerInteruptModeSetup();

bool setFrequenz_Timer1(unsigned int f);

void stepper_timerModeRun();

void stepper_timerModeStop();

bool set_stepperSpeed(int speed);

bool home();// home axis and get zero ; calibrate position 

bool goToPosition(float position);// in mm

bool moveRelative(float distance,unsigned int speed);// in mm


#endif