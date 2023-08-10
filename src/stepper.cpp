#include "stepper.h"

const unsigned int minPulsLOW = 2.5; //us
const unsigned int minPulsHIGH = 2.5;//us
const unsigned int microsStepp = 400; // steps per rotation (1.8° Stpep)

const unsigned int mmPerRotaion = 3;//mm pro umdrehung des Motors 1:3 

bool DIR = 0; // 0CW 1CCW
bool ENA = 1; //0ON 1OF for DM556T

float sPS;
float ThalfWave;

unsigned int pDIR;
unsigned int pENA;
unsigned int pPUL;

signed int Speed;//mm/s

float position;



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
    digitalWrite(pPUL,HIGH);
    delayMicroseconds(minPulsHIGH);
    digitalWrite(pPUL,LOW);
}

void enableMotor(bool ena){
    ENA = ena;
    digitalWrite(pENA,ENA);
}

void setDir(bool dir){
    DIR = dir;
    digitalWrite(pDIR,DIR);
}


void cycle(TickType_t* xLast){

    TickType_t xFrequenz;
    xFrequenz = 3;
    //Serial.begin(115200);
    for(;;){
    
    //vTaskDelayUntil(xLast,xFrequenz);
    //Serial.println(xTaskGetTickCount());
    delayMicroseconds(ThalfWave);
    step();

    if(!DIR){
        position++;
    }else{position--;}

    
    }
}

void stepPerSecond(){
    sPS = microsStepp/mmPerRotaion * Speed;
    ThalfWave = ((1/sPS) * 1000000) - minPulsHIGH;  
    

}

void setSpeed(signed int speed){
    Speed = speed;
    stepPerSecond();
    
}

float get_position(){
    return (position/microsStepp)*mmPerRotaion;
}

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

bool setFrequenz_Timer1(int f){
    if(f>65536){return 0;}
    OCR1A = f;
    return 1;

}

void stepper_timerModeRun(){
    enableMotor(0);
    
    TIMSK1 |= (1 << OCIE1A); //Timer/Countern, Output Compare A Match Interrupt Enable
    
}

void stepper_timerModeStop(){
    enableMotor(1);
    TIMSK1 &= (0 << OCIE1A); //Timer/Countern, Output Compare A Match Interrupt Disable
    
}

ISR(TIMER1_COMPA_vect) {
    //Serial.println(1);
    step();
}