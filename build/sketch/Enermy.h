#line 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\Enermy.h"
#ifndef ENERMY_H
#define ENERMY_H

#include <Arduino.h>
#include <ESP32Servo.h>


// The threshold value for the LDR sensor
// The value need to modify!!
// Remember 1K ohm resistor is used
#define Threshold 3900
class Enermy
{
private:
    unsigned char LDR_PIN;
    unsigned char MOTOR_PIN1;
    unsigned char MOTOR_PIN2;
    unsigned char MOTOR_PIN3;
    Servo servo;
public:
    Enermy(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
    ~Enermy();
public:
    void motor_direct(short);
    bool is_shooted();
    void kill();
    void recovery();
};

#endif