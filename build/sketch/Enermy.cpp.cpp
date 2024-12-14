#include <Arduino.h>
#line 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void setup();
#line 6 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void loop();
#line 0 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
#line 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\Enermy.cpp"
#include "Enermy.h"

Enermy::Enermy(
    unsigned char LDR_PIN, 
    unsigned char MOTOR_PIN1, 
    unsigned char MOTOR_PIN2, 
    unsigned char MOTOR_PIN3
    ):LDR_PIN(LDR_PIN), MOTOR_PIN1(MOTOR_PIN1), MOTOR_PIN2(MOTOR_PIN2), MOTOR_PIN3(MOTOR_PIN3){
    
    pinMode(LDR_PIN, INPUT);
    pinMode(MOTOR_PIN1, OUTPUT);
    pinMode(MOTOR_PIN2, OUTPUT);
    pinMode(MOTOR_PIN3, OUTPUT);
}

Enermy::~Enermy(){}

bool Enermy::is_shooted(){
    int LDR_value = analogRead(LDR_PIN);
    return LDR_value > Threshold;
}

void Enermy::motor_direct(short direction){
    switch (direction){
        case 0:
            digitalWrite(MOTOR_PIN3, LOW);
            break;
        case 1:
            digitalWrite(MOTOR_PIN1, HIGH);
            digitalWrite(MOTOR_PIN2, HIGH);
            digitalWrite(MOTOR_PIN3, HIGH);
            break;
        case -1:
            digitalWrite(MOTOR_PIN1, LOW);
            digitalWrite(MOTOR_PIN2, LOW);
            digitalWrite(MOTOR_PIN3, HIGH);
            break;
        default:
            // The direction is invalid
            break;
    }
}
#line 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void setup()
{
	
}

void loop()
{
	
}

