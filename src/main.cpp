#include <Arduino.h>
//#include <Arduino_FreeRTOS.h>
//#include <semphr.h>
#include "pages.h"
#include "stepper.h"

#define MOTOR_Y_ENABLE_PIN 49
#define MOTOR_Y_STEP_PIN 44
#define MOTOR_Y_DIR_PIN 51

/*
SemaphoreHandle_t xSerialSemaphore;

TaskHandle_t stepper,lcd;

void LCD_UPdate(void *pvParameters);
void stepperMove(void* pvParameters);

void setup() {

    setupPages();
    landingPage();
    runPage();

    pinSetup(MOTOR_Y_ENABLE_PIN,MOTOR_Y_DIR_PIN,MOTOR_Y_STEP_PIN);
    setSpeed(30);
    enableMotor(0);
    setDir(0);
    
    
    
    xTaskCreate(LCD_UPdate,"updateLCD",256,NULL,2,&lcd);
    xTaskCreate(stepperMove,"stepperMove",256,NULL,2,&stepper);
    
    vTaskStartScheduler();
    

}

void loop() {
    // NOTHIN
    
    }



void LCD_UPdate(void* pvParameters){

    TickType_t xFrequenz;
    xFrequenz = 1;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {   
        //vars_Step(NULL,sp0);
        vTaskDelayUntil(&xLastWakeTime,xFrequenz);
        startTouch();
        
        
    }
    
}




void stepperMove(void* pvParameters){

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    cycle(&xLastWakeTime);
    
}
*/



void setup(){
    Serial.begin(115200);
    Serial.println("Startet");

    setupPages();
    landingPage();
    runPage();

    pinSetup(MOTOR_Y_ENABLE_PIN,MOTOR_Y_DIR_PIN,MOTOR_Y_STEP_PIN);
    enableMotor(0);
    setDir(0);

    stepper_TimerInteruptModeSetup();
    stepper_timerModeStop();

}

void loop(){
    //main loop ....
    startTouch();
    setFrequenz_Timer1(9999);
    

}