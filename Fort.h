#ifndef FORT_H
#define FORT_H


#include <Arduino.h>
#include <ESP32Servo.h>

class Fort{
private:
    unsigned long int last_shoot_time;
private:
    unsigned char FORT_LAZER_PIN;
    unsigned char FORT_SERVO_PIN;
    unsigned int CD;
    
    Servo servo;
public:
    Fort(unsigned char, unsigned char, unsigned int);
    ~Fort();
    /*
    * Turns the servo to the left or right
    */
    void turn(int);
    /*
    * Shoots the lazer if the cooldown time has passed
    * Return can shoot or not
    */
    bool shoot();
};

#endif