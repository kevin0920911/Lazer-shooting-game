#line 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\Fort.cpp"
#include "Fort.h"

Fort::Fort(
    unsigned char lazer_pin, 
    unsigned char servo_pin, 
    unsigned int CD):
    FORT_LAZER_PIN(lazer_pin), FORT_SERVO_PIN(servo_pin), CD(CD), last_shoot_time(0){

    pinMode(FORT_LAZER_PIN, OUTPUT);
    servo.attach(FORT_SERVO_PIN);
    servo.write(90);
}

Fort::~Fort(){servo.detach();}

void Fort::turn(int angle){
    servo.write(angle);
    
}


bool Fort::shoot(){
    unsigned long int current_time = millis();
    if (current_time - last_shoot_time >= CD){
        last_shoot_time = current_time;
        digitalWrite(FORT_LAZER_PIN, HIGH);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        digitalWrite(FORT_LAZER_PIN, LOW);
        return true;
    }
    return false;
}