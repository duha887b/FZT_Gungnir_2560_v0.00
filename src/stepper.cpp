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
const unsigned long int maxfrquenz = 100000;//Hz
const unsigned int microsStepp = 400; // steps per rotation (1.8° Stpep)

const unsigned int mmPerRotaion = 3;//mm pro umdrehung des Motors 1:3 

bool DIR = 0; // 0CW 1CCW


bool ENA = 1; //0ON 1OF for DM556T

unsigned int pDIR;
unsigned int pENA;
unsigned int pPUL;




int32_t Speed;//mm/s

volatile long double Position = 0;
volatile int64_t PostionCounter =0; // count relative steps from zero position

void pinSetup(unsigned int pena, unsigned int pdir,unsigned int ppuls){
    pDIR = pdir;
    pENA = pena;
    pPUL = ppuls;
    pinMode(pENA,OUTPUT);
    pinMode(pDIR,OUTPUT);
    pinMode(pPUL,OUTPUT);

    digitalWrite(pENA,ENA);
    digitalWrite(pDIR,DIR);
    digitalWrite(pPUL,LOW);

}

void step(){

                    //Crash dedection
    if(!ENA){
     digitalWrite(pPUL,HIGH);
     delayMicroseconds(minPulsHIGH);
     digitalWrite(pPUL,LOW);

     DIR ? PostionCounter-- : PostionCounter++;   
    }
    
    
}

void enableMotor(bool ena){
    ENA = ena;
    digitalWrite(pENA,ENA);
}

void setDir(bool dir){
    DIR = dir;
    digitalWrite(pDIR,DIR);
}

bool getDir(){
    return DIR;
}

//Timer 1 with intterupt for Wave genaration

void stepper_TimerInteruptModeSetup(){
    //Timer 1 16bit (overflow at 65536)

  noInterrupts(); //disable intterupts global
  // Clear registers
  TCCR1A = 0x00; //Timer/Counter 1 Control Register A auf NULL
  TCCR1B = 0x00; //Timer/Counter 1 Control Register B auf NULL
  TCNT1 = 0x0000; //Timer/Counter 1 

  

  // f=(16000000/((TOP+1)*N))

  OCR1A = 1; //TOP value
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 1024
  TCCR1B |= (1 << CS11); //N prescalar S157 Manuel
  
  interrupts();


}

float TOP[5]; //TOP S157 Manuel
int N[5] = {1,8,64,256,1024}; //Prescalar
long int NTop[2]; // final descision TOP & N

bool setFrequenz_Timer1(unsigned int f){

   if(f>maxfrquenz){return false;}                           // controller based max frequenz
    //Serial.begin(115200);

    for(int i = 0; i<(int)(sizeof(TOP)/sizeof(float));i++){  
    
        TOP[i] = ((float)F_CPU/((float)f*(float)N[i])-1);   // calc TOP for each combination 
        if (TOP[i]> 65535 ){                                // check overflow on 16bit Register 
            TOP[i] = -1;
        }
        //Serial.println(TOP[i]);
    }

    float tmp_1 = 1;
    
    for(int i = 0;i<(int)(sizeof(TOP)/sizeof(float));i++ ){ //find nearest matching frequenz
        float tmp;
        int flag = 0; //aufrunden

        if(TOP[i] == -1){continue;}

        tmp = TOP[i] - (long int)TOP[i];
        if(tmp>0.5){
            tmp = 1-tmp;
            flag = 1;
        }else{
            flag = 0;
        }

        if(tmp<tmp_1){
            NTop[0] = N[i];
            NTop[1] = (long int)TOP[i] + flag;
            tmp_1 = tmp;
            
        }
        if (tmp == tmp_1){
            if(NTop[1]>(long int)TOP[i] + flag){
                NTop[0] = N[i];
                NTop[1] = (long int)TOP[i] + flag;
            }
        }
        
    }
    //Serial.println(NTop[0]);
    //Serial.println(NTop[1]);


    OCR1A = NTop[1]; //set tTOP value
    TCCR1B &= 0xF8; // reset Timer source before rewriding

    switch (NTop[0]) // set prescalar
    {
    case 1:
        TCCR1B |= 0x01; //N prescalar S157 Manuel
        break;
    case 8:
        TCCR1B |= 0x02; //N prescalar S157 Manuel
        break;

    case 64:
        TCCR1B |= 0x03; //N prescalar S157 Manuel
        break;
    case 256:
        TCCR1B |= 0x04; //N prescalar S157 Manuel
        break;
    case 1024:
        TCCR1B |= 0x05; //N prescalar S157 Manuel
        break;
        
    default:
        return false;
    }

    return true;

}

void stepper_timerModeRun(){
    enableMotor(0);
    
    TIMSK1 |= (1 << OCIE1A); //Timer/Countern, Output Compare A Match Interrupt Enable
    
}

void stepper_timerModeStop(){
   
    TIMSK1 &= (0 << OCIE1A); //Timer/Countern, Output Compare A Match Interrupt Disable // Motor still powered
    Serial.println("tstopmmm");
}
long double updatePosition(){
    Position = (PostionCounter/microsStepp)*mmPerRotaion;
    return Position;
}

//workhorse 
ISR(TIMER1_COMPA_vect) {
    //Serial.println(1);
     //TCNT1 = 0x0000; //Timer/Counter 1
    step();
    DIR ? PostionCounter-- : PostionCounter++ ;
}

bool set_stepperSpeed(int speed){
    int32_t tmp_freq;
    Speed = speed;

    speed<0 ? setDir(DOWN) : setDir(UP); // set direction 

    tmp_freq  = (microsStepp / mmPerRotaion) * abs(speed) ;
    return setFrequenz_Timer1(tmp_freq);
}

bool moveRelative(float distance,unsigned int speed)// in mm (direction via distance)
{
    
    double long tmp_positionVictory = updatePosition() + distance; // calculate new position reletive to the current position
    
    distance>0 ? set_stepperSpeed(speed) : set_stepperSpeed(-(speed)); // set speed and direction
    
    stepper_timerModeRun();
    while(((distance>0) && (updatePosition()<=tmp_positionVictory)) || ((distance<0) && (updatePosition()>=tmp_positionVictory))){
        
        
    } 

    stepper_timerModeStop();
    return 1;

}


// TODO refractor
int homeing_speed = 20;
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

//TODO not tested 
bool goToPosition(float position)// in mm
{
  

    while (!get_limitBottom() && !get_limitTop())//quatsch
    {
        /* limit breack; */
        break;
    }
    

}


