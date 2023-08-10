#ifndef STEPPER_HH
#define STEPPER_HH

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>


void pinSetup(unsigned int pena, unsigned int pdir,unsigned int ppuls);

void step();


void setSpeed(signed int speed);

void cycle(TickType_t* xLast);

void enableMotor(bool ena);
void setDir(bool dir);

float get_position();

//begin variant2

void stepper_TimerInteruptModeSetup();

bool setFrequenz_Timer1(int f);

void stepper_timerModeRun();


void stepper_timerModeStop();

#endif