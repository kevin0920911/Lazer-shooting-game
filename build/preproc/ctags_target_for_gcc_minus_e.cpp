# 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\Enermy.cpp"
# 2 "c:\\Users\\Kevin\\Desktop\\ES_final\\Enermy.cpp" 2

Enermy::Enermy(
    unsigned char LDR_PIN,
    unsigned char MOTOR_PIN1,
    unsigned char MOTOR_PIN2,
    unsigned char MOTOR_PIN3
    ):LDR_PIN(LDR_PIN), MOTOR_PIN1(MOTOR_PIN1), MOTOR_PIN2(MOTOR_PIN2), MOTOR_PIN3(MOTOR_PIN3){

    pinMode(LDR_PIN, 0x01);
    pinMode(MOTOR_PIN1, 0x02);
    pinMode(MOTOR_PIN2, 0x02);
    pinMode(MOTOR_PIN3, 0x02);
}

Enermy::~Enermy(){}

bool Enermy::is_shooted(){
    int LDR_value = analogRead(LDR_PIN);
    return LDR_value > 6958;
}

void Enermy::motor_direct(short direction){
    switch (direction){
        case 0:
            digitalWrite(MOTOR_PIN3, 0x0);
            break;
        case 1:
            digitalWrite(MOTOR_PIN1, 0x1);
            digitalWrite(MOTOR_PIN2, 0x1);
            digitalWrite(MOTOR_PIN3, 0x1);
            break;
        case -1:
            digitalWrite(MOTOR_PIN1, 0x0);
            digitalWrite(MOTOR_PIN2, 0x0);
            digitalWrite(MOTOR_PIN3, 0x1);
            break;
        default:
            // The direction is invalid
            break;
    }
}
# 1 "c:\\Users\\Kevin\\Desktop\\ES_final\\final.ino"
void setup()
{

}

void loop()
{

}
