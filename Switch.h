#ifndef SWITCH_H
#define SWITCH_H

class Switch
{
private:
    bool is_pressed;
    unsigned char switch_pin;
    unsigned long last_pressed_time;
    int debounce_time;
public:
    Switch(unsigned char);
    ~Switch();
    bool button_pressed();
};
#endif
