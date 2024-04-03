#include <avr/io.h>
#include <util/delay.h>
#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_led.h"
#include "ses_adc.h"
#include "ses_button.h"
#include <time.h>
#include "ses_timer.h"

 uint16_t count = 0;
bool debounced_Button = true;

// typedef enum color{
//     RED,
//     GREEN,
// };

// void softwareTimer(void)
//  {
//      count ++;

//     if(count >= 200)
//     {
//     led_yellowToggle();
//      count = 0;
//     }
      
//  }

int main(void)
{
    

    //// Task 3.1 ///
    /*  sei();
    button_init();
    led_redOff();
    led_greenInit();
    led_yellowInit();
    led_yellowOff();
    led_greenOff();
    button_setJoystickButtonCallback(led_yellowToggle);
	button_setRotyaryButtonCallback(led_greenToggle);
*/

    /// Taks 3.2 ///////
   /* led_yellowInit();
    timer2_start();
    sei();
    timer2_setCallback(softwareTimer);*/

    // led_yellowInit();
    // timer1_start();
    // sei();
    // timer1_setCallback(softwareTimer);


    // while (1)
    // {
       
    // }


    //Task  3.3 /////////////
   
    led_greenInit();
    led_yellowInit();
    led_redInit();
    led_redOff();
    led_greenOff();
    led_yellowOff();
    button_setJoystickButtonCallback(led_greenToggle);
    button_setRotaryButtonCallback(led_yellowToggle);
    timer1_start();
    sei();
    button_init(debounced_Button);
 while (1)
    {
       
    }
}