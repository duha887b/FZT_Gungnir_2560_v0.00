/*

stepper.cpp - controle bipolar stepper motors with puls controled controlers
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

#include "stepper.h"


const unsigned int minPulsLOW = 2.5; //us
const unsigned int minPulsHIGH = 2.5;//us

bool DIR_Y = 0; // 0CW 1CCW
bool ENA_Y = 1; //0ON 1OF for DM556T

bool DIR_S = 0; // 0CW 1CCW
bool ENA_S = 1; //0ON 1OF for DM556T

int32_t Speed_Y;//mm/s
int32_t Speed_S;//mm/s


volatile float Position_Y = 0;
volatile int64_t PostionCounter_Y =0; // count relative steps from zero position

volatile float Position_S = 0;
volatile int64_t PostionCounter_S =0;

void pinSetup(){

    pinMode(MOTOR_Y_DIR_PIN,OUTPUT);
    pinMode(MOTOR_Y_ENABLE_PIN,OUTPUT);
    pinMode(MOTOR_Y_STEP_PIN,OUTPUT);

    pinMode(MOTOR_S_DIR_PIN,OUTPUT);
    pinMode(MOTOR_S_ENABLE_PIN,OUTPUT);
    pinMode(MOTOR_S_STEP_PIN,OUTPUT);

    digitalWrite(MOTOR_Y_DIR_PIN,1);
    digitalWrite(MOTOR_Y_ENABLE_PIN,1);
    digitalWrite(MOTOR_Y_STEP_PIN,0);

    digitalWrite(MOTOR_S_DIR_PIN,1);
    digitalWrite(MOTOR_S_ENABLE_PIN,1);
    digitalWrite(MOTOR_S_STEP_PIN,0);

    


}

void step_y(){

        if(ENA_Y){return;}  // crash detection 
        else if ((DIR_Y == UP) && get_limitTop() )
        {
            return;
        }else if ((DIR_Y == DOWN) && get_limitBottom())
        {
            return;
        }

        digitalWrite(MOTOR_Y_STEP_PIN,HIGH);
        delayMicroseconds(minPulsHIGH);
        digitalWrite(MOTOR_Y_STEP_PIN,LOW);

        DIR_Y ? PostionCounter_Y-- : PostionCounter_Y++;
        //Serial.println((int)PostionCounter_Y);
    
    
     
}

void step_s(){
        if(ENA_S){return;}

        digitalWrite(MOTOR_S_STEP_PIN,HIGH);
        delayMicroseconds(minPulsHIGH);
        digitalWrite(MOTOR_S_STEP_PIN,LOW);

        DIR_S ? PostionCounter_S-- : PostionCounter_S++;
    
    
     
}

//getter setter ENA/DIR

bool get_ENA_Y(){
    return ENA_Y;
}
bool get_ENA_S(){
    return ENA_S;
}
bool get_DIR_Y(){
    return DIR_Y;
}
bool get_DIR_S(){
    return DIR_S;
}

void set_ENA_Y(bool s){
    ENA_Y = s;
    digitalWrite(MOTOR_Y_ENABLE_PIN,s);
}
void set_ENA_S(bool s){
    ENA_S = s;
    digitalWrite(MOTOR_S_ENABLE_PIN,s);
}
void set_DIR_Y(bool s){
    DIR_Y = s;
    digitalWrite(MOTOR_Y_DIR_PIN,s);
}
void set_DIR_S(bool s){
    DIR_S = s;
    digitalWrite(MOTOR_S_DIR_PIN,s);
}

void updatePosition(){
    Position_Y = ((float)PostionCounter_Y/MOTOR_Y_MICROSTEPPING)*MOTOR_Y_MM_PER_U;
    Position_S = ((float)PostionCounter_S/MOTOR_S_MICROSTEPPING)*MOTOR_S_MM_PER_U;
    //Serial.println(Position_S);
}

long double get_positionY(){
    return Position_Y;
}
long double get_positionS(){
    return Position_S;
}

void set_positionCounterY(int64_t n){
    PostionCounter_Y=n;
}
void set_positionCounterS(int64_t n){
    PostionCounter_S=n;
}

int64_t get_positionCounterY(){
    return PostionCounter_Y;
}
int64_t get_positionCounterS(){
    return PostionCounter_S;
}

int umin_to_frequenz(float speed,float mmPerTurn, int mstep){
    return (mstep/mmPerTurn)*abs(speed); //quantisierungsfehler = 1Hz
}

void set_speedY(float speed){
    speed < 0 ? set_DIR_Y(DOWN): set_DIR_Y(UP);
    set_timer2(umin_to_frequenz(speed,MOTOR_Y_MM_PER_U,MOTOR_Y_MICROSTEPPING)); 
}
void set_speedS(float speed){
    speed < 0 ? set_DIR_S(DOWN) : set_DIR_S(UP);
    set_timer3(umin_to_frequenz(speed,MOTOR_S_MM_PER_U,MOTOR_S_MICROSTEPPING));
}


void run_MotorY(bool r){
    start_timer2(r);
    set_ENA_Y(!r);
}
void run_MotorS(bool r){
    start_timer3(r);
    set_ENA_S(!r);
}






/*
bool moveRelative_MotorY(float distance,float speed)// in mm (direction via distance)
{
    
    double long tmp_positionVictory = updatePosition() + distance; // calculate new position reletive to the current position
    
    distance>0 ? set_stepperSpeed(speed) : set_stepperSpeed(-(speed)); // set speed and direction
    
    stepper_timerModeRun();
    while(((distance>0) && (updatePosition()<=tmp_positionVictory)) || ((distance<0) && (updatePosition()>=tmp_positionVictory))){

        
    } 
        
    stepper_timerModeStop();

   
    return 1;

}*/


// TODO refractor
/*int homeing_speed = 20;
bool home()// home axis and get zero ; calibrate position 
{
   
    set_stepperSpeed(-(homeing_speed)); //go to bottom limit 
    stepper_timerModeRun();
    
    for(;;){
        
        if(!get_limitBottom()){
            break;
        }
    }

    moveRelative(3,1);
    Serial.println("move rel point");
   
    moveRelative(-9,1); // slow down

    Position = 0;
    PostionCounter = 0;

    set_stepperSpeed((homeing_speed)); //go to upper limit
    stepper_timerModeRun();

    while (get_limitTop());
    moveRelative(-3,1);
    moveRelative(9,1);
    

    Serial.println("done");

    return true;
}
*/
//TODO not tested ans implemented
/*bool goToPosition(float position)// in mm
{
  

    while (!get_limitBottom() && !get_limitTop())//quatsch
    {
        
        break;
    }
    

}*/

bool moveRelative_MotorY(float distance,float currentPosition){

    
    if(distance < 0){
        if((currentPosition + distance) < (float)get_positionY()){
                // stepping over interrupt , implementet in timer_controle
             return true;   
             Serial.println((currentPosition + distance) < (float)get_positionY());
                
        }
        
        
    }else{
        if((currentPosition + distance) > (float)get_positionY()){
                // stepping over interrupt , implementet in timer_controle
                Serial.println("wait up");
        }
        Serial.println((currentPosition + distance) > (float)get_positionY());
        return true;

    }
    
    return false;
}

//bool moveRelative_MotorY(float distance,float speed){}


