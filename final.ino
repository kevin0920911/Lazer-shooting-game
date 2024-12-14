#include "Enermy.h"
#include "Fort.h"
#include "Switch.h"

//TODO: Define the pins
#define DIRETION_PIN 34 

//TODO: Define the buttoms
Switch buttom;

void emermies_behaviour(Enermy &enermy){
    if (enermy.is_shooted()){
        enermy.kill();
        enermy.last_shoot_time = millis();
    }

    if (millis() - enermy.last_shoot_time >= RECOVERY_TIME){
        enermy.recovery();
    }

    if (millis() - enermy.last_turn_time >= MOVE_TIME){
        enermy.last_turn_time = millis();
        enermy.last_turn_state = !enermy.last_turn_state;
        enermy.motor_direct(enermy.last_turn_state);
    }
}

void fort_behaviour(Fort &fort){
    int angle = map(analogRead(DIRETION_PIN), 0, 4095, 10, 160);
    fort.turn(angle);
    switch (buttom.check()){
        case Switch::PRESSED:
        case Switch::LONG_PRESSED:
            fort.shoot();
            break;
    }
}

void setup(){

}
void loop(){
    
}