#include "switch.h"
#include <Arduino.h>

Switch::Switch(unsigned char pin):switch_pin(pin){
    this->is_pressed = false;
    this->last_pressed_time = 0;
    this->debounce_time = 1000;
}

Switch::~Switch(){}

bool Switch::button_pressed(){
    if (digitalRead(this->switch_pin) == LOW){
        if (!this->is_pressed){
            this->is_pressed = true;
            last_pressed_time = millis();
        }
    }
    else if(this->is_pressed && millis() - last_pressed_time > debounce_time){
        this->is_pressed = false;
        return true;
    }
    return false;
}