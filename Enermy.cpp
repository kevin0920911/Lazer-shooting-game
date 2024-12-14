#include "Enermy.h"

Enermy::Enermy(
    unsigned char LDR_PIN, 
    unsigned char MOTOR_PIN1, 
    unsigned char MOTOR_PIN2, 
    unsigned char MOTOR_PIN3,
    unsigned char SERVO_PIN
    ):LDR_PIN(LDR_PIN), MOTOR_PIN1(MOTOR_PIN1), MOTOR_PIN2(MOTOR_PIN2), MOTOR_PIN3(MOTOR_PIN3){
    
    servo.attach(SERVO_PIN);
    pinMode(LDR_PIN, INPUT);
    pinMode(MOTOR_PIN1, OUTPUT);
    pinMode(MOTOR_PIN2, OUTPUT);
    pinMode(MOTOR_PIN3, OUTPUT);
    last_shoot_time = 0;
    last_turn_state = false;
    last_turn_time = 0;
}

Enermy::~Enermy(){}

bool Enermy::is_shooted(){
    int LDR_value = analogRead(LDR_PIN);
    printf("LDR value: %d\n", LDR_value);
    return LDR_value >= Threshold;
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

void Enermy::kill(){
    servo.write(180);
}

void Enermy::recovery(){
    servo.write(90);
}