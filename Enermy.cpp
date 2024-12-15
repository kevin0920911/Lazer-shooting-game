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