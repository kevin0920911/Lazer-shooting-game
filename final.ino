#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "Enermy.h"
#include "Fort.h"
#include "Switch.h"

#define VR_PIN 34

LiquidCrystal_I2C lcd(0x27, 16, 2);

HardwareSerial mySerial(1);
DFRobotDFPlayerMini myDFPlayer;
Enermy enermy[2] = {Enermy(36, 25, 26, 27, 2, 1000), Enermy(14, 5, 18, 33, 4, 3800)};
Fort fort(32, 19, 500);
Switch buttom(12, HIGH, true);

long int lcd_last_update_time = 0;

int score = 0;

void taskMP3(void* pvParameters);
TaskHandle_t taskHandleMP3 = NULL;

void setup() {
    Serial.begin(115200);
    MP3_setup();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press and\0");
    lcd.setCursor(0, 1);
    lcd.print("start the game!\0");
}

void loop() {
    if (buttom.check() == Switch::PRESSED || buttom.check() == Switch::LONG_PRESSED) {
        game_task();
    }
    delay(50);
}

void emermies_behaviour() {
    for (int i = 0; i < 2; i++) {
        if (enermy[i].is_shooted()) {
            score++;
            enermy[i].kill();
            enermy[i].last_shoot_time = millis();
        }

        if (millis() - enermy[i].last_shoot_time >= RECOVERY_TIME) {
            enermy[i].recovery();
        }

        if (millis() - enermy[i].last_turn_time >= MOVE_TIME) {
            enermy[i].last_turn_time = millis();
            enermy[i].last_turn_state = enermy[i].last_turn_state * -1;
            enermy[i].motor_direct(enermy[i].last_turn_state);
        }
    }
}

void MP3_setup() {
    mySerial.begin(9600, SERIAL_8N1, 17, 16);

    while (!myDFPlayer.begin(mySerial)) {
        Serial.println(F("Unable to begin"));
        delay(1000);
    }
    Serial.println(F("DFPlayer Mini online."));
    myDFPlayer.volume(30);
}

void fort_behaviour() {
    int angle = map(analogRead(VR_PIN), 0, 4095, 10, 160);

    fort.turn(angle);
    switch (buttom.check()) {
        case Switch::PRESSED:
        case Switch::LONG_PRESSED:
            fort.shoot();
            break;
    }
}

void taskMP3(void* pvParameters) {
    myDFPlayer.play(1);  // 播放第 1 首音樂
}

void game_task() {
    score = 0;
    long int game_start_time = millis();
    xTaskCreatePinnedToCore(taskMP3, "MP3 Task", 2048, NULL, 1, &taskHandleMP3, 1);
    while (millis() - game_start_time <= 30000) {
        fort_behaviour();
        emermies_behaviour();
        if (millis() - lcd_last_update_time >= 1000) {
            lcd_last_update_time = millis();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Score: " + String(score) + "\0");
            lcd.setCursor(0, 1);
            lcd.print("Time: " + String((30000 - (millis() - game_start_time)) / 1000) + "\0");
        }
    }
    for (int i = 0; i < 2; i++) {
        enermy[i].recovery();
        enermy[i].motor_direct(0);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: " + String(score) + "\0");
    lcd.setCursor(0, 1);
    lcd.print("Game Over!\0");

    if (taskHandleMP3 != NULL) {
        Serial.println("Deleting MP3 Task...");
        vTaskDelete(taskHandleMP3);  // 刪除任務
        taskHandleMP3 = NULL;        // 清空句柄
    }

    while (true) {
        if (buttom.check() == Switch::PRESSED or buttom.check() == Switch::LONG_PRESSED) {
            break;
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press and\0");
    lcd.setCursor(0, 1);
    lcd.print("start the game!\0");
    delay(1000);
}