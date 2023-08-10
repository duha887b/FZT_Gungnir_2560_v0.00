
#include "pages.h"


const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 806, TS_RT = 163, TS_TOP = 958, TS_BOT = 168;
enum page{zero,jog,run}currentPage;



MCUFRIEND_kbv tft;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button zero_btn, jog_btn,run_btn;

int pixel_x, pixel_y;
//Touch_getXY() updates global vars
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



void setupPages(){
    // Reading TFT ID:
    uint16_t ID=tft.readID();
                                //Initializing TFT display:
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);

    tft.setRotation(0);
    tft.fillScreen(BLACK);
    Serial.begin(9600);
    Serial.println("PagesStart");

}

void landingPage(){
    tft.setCursor(80,240);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("Gungnir_00.0");
    delay(500);
    tft.fillScreen(BLACK);

    tft.setCursor(230,25);
    tft.print("v0.00");
    tft.drawLine(65,0,65,64,WHITE);
    tft.drawLine(129,0,129,64,WHITE);
    tft.drawLine(193,0,193,64,WHITE);
    tft.drawLine(0,0,0,64,WHITE);

    zero_btn.initButton(&tft,33,32,63,63,BLACK,BLACK,WHITE,"Zero", 2);
    zero_btn.drawButton(false);

    jog_btn.initButton(&tft,97,32,63,63,BLACK,BLACK,WHITE,"Jog", 2);
    jog_btn.drawButton(false);

    run_btn.initButton(&tft,161,32,63,63,BLACK,BLACK,WHITE,"Run", 2);
    run_btn.drawButton(false);

}
void req(){
    tft.fillRect(0,65,320,415,BLACK);
    tft.drawRect(0,65,320,415,WHITE);
    

}
Adafruit_GFX_Button btn_autoZero;
void setZeroPage(){
    currentPage = zero;
    req();
    tft.drawLine(1,65,64,65,BLACK);
    btn_autoZero.initButton(&tft,160,200,160,100,WHITE,BLACK,WHITE,"Z-Achse", 2);
    btn_autoZero.drawButton(false);
    
    
}

Adafruit_GFX_Button btn_01, btn_1,btn_10, btn_100;
Adafruit_GFX_Button btn_top, btn_bot, btn_zero;
void jogPage(){
    currentPage = jog;
    req();
    tft.drawLine(66,65,128,65,BLACK);
    tft.drawLine(0,271,320,271,WHITE);

    tft.setTextSize(1);
    tft.setCursor(10,70);
    tft.print("Z-Achse");
        
    tft.setTextSize(1);
    tft.setCursor(10,90);
    tft.print("Weg pro Rastung");
    btn_01.initButton(&tft,42,120,77,40,WHITE,BLACK,WHITE,"0,1mm",2);
    btn_01.drawButton(false);
    btn_1.initButton(&tft,120,120,77,40,WHITE,BLACK,WHITE,"1mm",2);
    btn_1.drawButton(false);
    btn_10.initButton(&tft,198,120,77,40,WHITE,BLACK,WHITE,"10mm",2);
    btn_10.drawButton(false);
    btn_100.initButton(&tft,276,120,77,40,WHITE,BLACK,WHITE,"100mm",2);
    btn_100.drawButton(false);

    tft.setTextSize(1);
    tft.setCursor(10,150);
    tft.print("Home");
    btn_top.initButton(&tft,55,212,100,100,WHITE,BLACK,WHITE,"Top",2);
    btn_top.drawButton(false);
    btn_bot.initButton(&tft,160,212,100,100,WHITE,BLACK,WHITE,"Bot",2);
    btn_bot.drawButton(false);
    btn_zero.initButton(&tft,265,212,100,100,WHITE,BLACK,WHITE,"Zero",2);
    btn_zero.drawButton(false);
    
    tft.setTextSize(1);
    tft.setCursor(10,276);
    tft.print("Spule");

}
void runPage(){
    currentPage = run;
    req();
    tft.drawLine(129,65,192,65,BLACK);
    tft.drawLine(0,271,320,271,WHITE);

    tft.setTextSize(1);
    tft.setCursor(10,70);
    tft.print("Z-Achse");

    tft.setTextSize(1);
    tft.setCursor(10,276);
    tft.print("Spule");
}

void startTouch(){

    bool down = Touch_getXY();
    if(!down){return;}

    /*if (down){
        Serial.println(pixel_x);
        Serial.println(pixel_y);
        delay(500);
    }*/
    zero_btn.press(down && zero_btn.contains(pixel_x, pixel_y));
    jog_btn.press(down && jog_btn.contains(pixel_x, pixel_y));
    run_btn.press(down && run_btn.contains(pixel_x, pixel_y));

    if (zero_btn.justReleased()){
        setZeroPage();
    }
        
    if (jog_btn.justReleased()){
        jogPage(); 
    }

    if (run_btn.justReleased()){
        runPage();       
    }

    switch (currentPage)
    {
    case zero:
        btn_autoZero.press(down && btn_autoZero.contains(pixel_x, pixel_y));
        if(btn_autoZero.justPressed()){
            btn_autoZero.drawButton(true);
        }
        break;
    case jog:
        break;
    case run:
        break;
    default:
        break;
    }
  

}

void vars_Step(bool DIR,int sp){
        tft.setTextSize(1);
        tft.setCursor(120,120);
        tft.print(sp);
    }