#include <Arduino.h>
#line 43 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void setup();
#line 58 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void loop();
#line 67 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void emermies_behaviour();
#line 88 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void MP3_setup();
#line 104 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void fort_behaviour();
#line 116 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void game_task();
#line 0 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
#line 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\Enermy.cpp"
#include "Enermy.h"

Enermy::Enermy(
    short int LDR_PIN, 
    short int MOTOR_PIN1, 
    short int MOTOR_PIN2, 
    short int MOTOR_PIN3,
    short int SERVO_PIN,
    int Threshold = 3900
    ){

    this -> LDR_PIN = LDR_PIN;
    this -> MOTOR_PIN1 = MOTOR_PIN1; 
    this -> MOTOR_PIN2 = MOTOR_PIN2;
    this->MOTOR_PIN3 = MOTOR_PIN3;
    this -> Threshold = Threshold;

    servo.attach(SERVO_PIN);
    pinMode(LDR_PIN, INPUT);
    pinMode(MOTOR_PIN1, OUTPUT);
    pinMode(MOTOR_PIN2, OUTPUT);
    pinMode(MOTOR_PIN3, OUTPUT);
    is_deaded = false;
    last_shoot_time = 0;
    last_turn_state = 1;
    last_turn_time = 0;
}

Enermy::~Enermy(){}

bool Enermy::is_shooted(){
    int LDR_value = analogRead(LDR_PIN);
    if (Threshold == 3900){
        printf("%d\n", LDR_value);
    }
    return LDR_value >= Threshold && !is_deaded;
}

void Enermy::motor_direct(short direction){
    switch (direction){
        case 0:
            digitalWrite(MOTOR_PIN3, HIGH);
            break;
        case 1:
            digitalWrite(MOTOR_PIN1, HIGH);
            digitalWrite(MOTOR_PIN2, HIGH);
            digitalWrite(MOTOR_PIN3, LOW);
            break;
        case -1:
            digitalWrite(MOTOR_PIN1, LOW);
            digitalWrite(MOTOR_PIN2, LOW);
            digitalWrite(MOTOR_PIN3, LOW);
            break;
        default:
            // The direction is invalid
            break;
    }
}

void Enermy::kill(){
    is_deaded = true;
    servo.write(0);
}

void Enermy::recovery(){
    is_deaded = false;
    servo.write(90);
}
#line 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
#include <Arduino.h>
#include "Enermy.h"
#include "Fort.h"
#include "Switch.h"
#include <ESP32Servo.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

//TODO: Define the pin number
#define VR_PIN 34

HardwareSerial mySerial(1);
DFRobotDFPlayerMini myDFPlayer;
Enermy enermy[2] = { Enermy(36, 25, 26, 27, 2, 1000), Enermy(14, 5, 18, 33, 4, 3800) };
Fort fort(32, 19, 1000);
Switch buttom(12, HIGH, true);

long int lcd_last_update_time = 0;
/*
* The information I need  
* 1. The Fort::Lazer pin number: 32
* 2. The Fort::Servo pin number: 19
* 3. The buttom pin: 35 
* 4. The VR pin number: 34
*/

int score = 0; 

/*
void fort_behaviour(){
    int angle = map(analogRead(DIRETION_PIN), 0, 4095, 10, 160);
    fort.turn(angle);
    switch (buttom.check()){
        case Switch::PRESSED:
        case Switch::LONG_PRESSED:
            fort.shoot();
            break;
    }
}
*/
void setup(){
    Serial.begin(115200);
    lcd.init(); 
    lcd.backlight();
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Press and\0");
    lcd.setCursor(0, 1);
    lcd.print("start the game!\0");
    MP3_setup();
    
}



void loop(){

    if (buttom.check() == Switch::PRESSED || buttom.check()  == Switch::LONG_PRESSED){
        game_task();
    }
    delay(50);
}


void emermies_behaviour(){
    for (int i=0; i<2; i++){
        if (enermy[i].is_shooted()){
            score++; 
            enermy[i].kill();
            enermy[i].last_shoot_time = millis();
        }

        if (millis() - enermy[i].last_shoot_time >= RECOVERY_TIME){
            enermy[i].recovery();
        }

        if (millis() - enermy[i].last_turn_time >= MOVE_TIME){
            enermy[i].last_turn_time = millis();
            enermy[i].last_turn_state = enermy[i].last_turn_state * -1;
            enermy[i].motor_direct(enermy[i].last_turn_state);
        }
    }
    
}

void MP3_setup(){
    mySerial.begin(9600, SERIAL_8N1, 17, 16);
    
    while (!myDFPlayer.begin(mySerial)) {
        Serial.println(F("Unable to begin"));
        delay(1000);
    }
    Serial.println(F("DFPlayer Mini online."));
    myDFPlayer.volume(20);

    /* 
    * The function is used to play music
    myDFPlayer.play(1);
    myDFPlayer.stop();
    */ 
}
void fort_behaviour(){
    int angle = map(analogRead(VR_PIN), 0, 4095, 10, 160);
    
    fort.turn(angle);
    switch (buttom.check()){
        case Switch::PRESSED:
        case Switch::LONG_PRESSED:
            fort.shoot();
            break;
    }
}

void game_task(){
    score = 0;
    long int game_start_time = millis(); 
    myDFPlayer.play(1);
    while (millis()- game_start_time <= 30000)
    {
        fort_behaviour();
        emermies_behaviour();
        if (millis() - lcd_last_update_time >= 1000){
            lcd_last_update_time = millis();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Score: "+ String(score)+"\0");
            lcd.setCursor(0, 1);
            lcd.print("Time: "+ String((30000 - (millis()-game_start_time))/1000)+"\0");
        }
    }
    for(int i=0; i<2; i++){
        enermy[i].recovery();
        enermy[i].motor_direct(0);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: "+ String(score)+"\0");
    lcd.setCursor(0, 1);
    lcd.print("Game Over!\0");
    myDFPlayer.stop();
    while (true){
        if (buttom.check() == Switch::PRESSED or buttom.check() == Switch::LONG_PRESSED){
            break;
        }
    }
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Press and\0");
    lcd.setCursor(0, 1);
    lcd.print("start the game!\0");
    delay(1000);
}
