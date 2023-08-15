#ifndef STEPPER_HH
#define STEPPER_HH


#include <Arduino.h>


void pinSetup(unsigned int pena, unsigned int pdir,unsigned int ppuls);

void step();

void enableMotor(bool ena);
void setDir(bool dir);

void stepper_TimerInteruptModeSetup();

bool setFrequenz_Timer1(unsigned int f);

void stepper_timerModeRun();

void stepper_timerModeStop();



#endif