#include <avr/io.h>
#include <util/delay.h>
#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_led.h"
#include "ses_adc.h"
#include "ses_button.h"
#include "ses_timer.h"
#include "ses_scheduler.h"

bool debounced_Button = true;
static taskDescriptor task1,task2,task3,task4;
static uint16_t count,start,seconds = 0;


 typedef enum colors {
    YELLOW,
    GREEN,
    RED
};

const uint8_t ledColor = GREEN;

void ledtoggle(void* led_Color)
{
    switch (*(uint8_t *)led_Color)
    {
    case YELLOW:
        led_yellowToggle();
        break;

    case GREEN:
        led_greenToggle();
        break;

    case RED:
        led_redToggle();
        break;
    }
}
void revert_stopwatch_state()
{
    start = ~ start;  // takes 2's complement of start and changes the state if rotary button is pressed
}
void start_stopwatch()
{
    if (start == 0 )  //it stops and starts counting , while checking state of rotary button.
    {
        count++;
    }
    if(count > 10) // after 10th of second increment 1 secoond
    {
        seconds++;
        count = 0;
    }
    lcd_setCursor(5, 2);
	fprintf(lcdout, "%d:%d", seconds, count);

}
void yellowLed_button_off();

// it sets a callback to joystick button
void yellowLed_button_on()
{
    led_yellowOn();

    button_setJoystickButtonCallback(yellowLed_button_off); // if joystick is pressed again then call yellowLed_button_off

   // setup task3
    task3.task = yellowLed_button_off;  
    task3.param = NULL;
    task3.period = 0;
    task3.expire = 5000;
    scheduler_add(&task3);

}
void yellowLed_button_off()
{
    led_yellowOff();


    scheduler_remove(&task3); // remove task ,if yellow led is on and button is pressed again within 5 seconds

     button_setJoystickButtonCallback(yellowLed_button_on); // enabling led on for next joystick pressed


}

int main(void)
{
    
    button_init(debounced_Button);
    scheduler_init();
    led_greenInit();
    led_yellowInit();
    led_yellowOff();
    lcd_init();
    button_setJoystickButtonCallback(yellowLed_button_on); //sets the joystickcallback 
    button_setRotaryButtonCallback(revert_stopwatch_state);

    
     //  setup task1  for toggling green led after 2 seconds 
    task1.task = ledtoggle;
    task1.param = (void*)&ledColor;
    task1.expire = 200;
    task1.period = 2000;

    scheduler_add(&task1);

    //  setup task2  for checking debounced button after eevery 5 ms
    task2.task = button_checkState;
    task2.param= NULL;
    task2.expire = 100;
    task2.period = 5;

    scheduler_add(&task2);

    //setup task4  for stopwatch control
    task4.task = start_stopwatch;
    task4.param = NULL;
    task4.period = 100;
    task4.expire = 50; 

    scheduler_add(&task4);

    sei();          // enables global interupts

    scheduler_run(); //

    return 0;
}