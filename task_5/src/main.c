//#include <avr/io.h>
//#include <util/delay.h>
#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_led.h"
#include "ses_button.h"
#include "ses_timer.h"
#include "ses_pwm.h"

uint8_t motorSpeed = 0xFF;
bool debounced_Button = true;

void motor_run();
void motor_stop()
{
    motorSpeed = 0;
    pwm_setDutyCycle(motorSpeed);
    button_setJoystickButtonCallback(motor_run);
}
void motor_run()
{
    motorSpeed = 170;
    pwm_setDutyCycle(motorSpeed);
    button_setJoystickButtonCallback(motor_stop);
    led_redOn();
}


int main(void)
{
    led_redInit();
    button_init(debounced_Button);
    button_setJoystickButtonCallback(motor_run);
    pwm_init();

    while (1)
    {
        button_checkState();

        _delay_ms(2000);
    }
    return 0;
}