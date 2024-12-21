#include <Arduino.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "Enermy.h"
#include "Fort.h"
#include "Switch.h"

#define VR_PIN 34
#define MP3_PIN 17
#define LED 16


LiquidCrystal_I2C lcd(0x27, 16, 2);

Enermy enermy[2] = { Enermy(36, 25, 26, 27, 2, 1000), Enermy(14, 5, 18, 33, 4, 3800) };
Fort fort(32, 19, 1000);
Switch buttom(12, HIGH, true);

long int lcd_last_update_time = 0;
long int last_shoot_time = 0;

int score = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(MP3_PIN, OUTPUT);
    pinMode(LED, OUTPUT);   
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press and\0");
    lcd.setCursor(0, 1);
    lcd.print("start the game!\0");
    
}

void loop()
{
    switch (buttom.check())
    {
    case Switch::PRESSED:
    case Switch::LONG_PRESSED:
        game_task();
        break;
    }
}

void emermies_behaviour()
{
    for (int i = 0; i < 2; i++)
    {
        if (enermy[i].is_shooted())
        {
            score+=92;
            enermy[i].kill();
            enermy[i].last_shoot_time = millis();
        }

        if (millis() - enermy[i].last_shoot_time >= RECOVERY_TIME)
        {
            enermy[i].recovery();
        }

        if (millis() - enermy[i].last_turn_time >= MOVE_TIME)
        {
            enermy[i].last_turn_time = millis();
            enermy[i].last_turn_state = enermy[i].last_turn_state * -1;
            enermy[i].motor_direct(enermy[i].last_turn_state);
        }
    }
}

void fort_behaviour()
{
    int angle = map(analogRead(VR_PIN), 0, 4095, 10, 170);

    fort.turn(angle);
    switch (buttom.check())
    {
    case Switch::PRESSED:
    case Switch::LONG_PRESSED:
        digitalWrite(LED, LOW); 
        fort.shoot();
        break;
    }
    if (millis() - fort.last_shoot_time >= fort.CD)
    {
        digitalWrite(LED, HIGH);
    }
}

void game_task()
{
    score = 0;
    digitalWrite(MP3_PIN, LOW);
    enermy[0].last_turn_state = -enermy[1].last_turn_state;
    lcd.init();
    lcd.backlight();
    long int game_start_time = millis();
    while (millis() - game_start_time <= 60100)
    {
        fort_behaviour();
        emermies_behaviour();
        if (millis() - lcd_last_update_time >= 1000)
        {
            lcd_last_update_time = millis();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Score: " + String(score) + "\0");
            lcd.setCursor(0, 1);
            lcd.print("Time: " + String((60100 - (millis() - game_start_time)) / 1000) + "\0");
        }
    }
    
    for (int i = 0; i < 2; i++)
    {
        enermy[i].recovery();
        enermy[i].motor_direct(0);
    }
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: " + String(score) + "\0");
    lcd.setCursor(0, 1);
    lcd.print("Game Over!\0");
    digitalWrite(MP3_PIN, HIGH);
    digitalWrite(LED, LOW);
    while (true)
    {
        if (buttom.check() == Switch::PRESSED or buttom.check() == Switch::LONG_PRESSED)
        {
            break;
        }
    }
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press and\0");
    lcd.setCursor(0, 1);
    lcd.print("start the game!\0");
    delay(1000);
}