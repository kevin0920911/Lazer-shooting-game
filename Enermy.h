#ifndef ENERMY_H
#define ENERMY_H

#include <Arduino.h>
#include <ESP32Servo.h>


// The threshold value for the LDR sensor
// The value need to modify!!
// Remember 1K ohm resistor is used
#define MOVE_TIME 7000
#define RECOVERY_TIME 1000
class Enermy
{
public:
    int last_shoot_time;
    short int last_turn_state;
    int last_turn_time;
    int Threshold;
private:
    short int LDR_PIN;
    short int MOTOR_PIN1;
    short int MOTOR_PIN2;
    short int MOTOR_PIN3;
    bool is_deaded;
    Servo servo;
public:
    Enermy(short int, short int, short int, short int, short int, int);
    ~Enermy();
public:
    void motor_direct(short);
    bool is_shooted();
    void kill();
    void recovery();
};

#endif