#include <avr/io.h>
#include <util/delay.h>
#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_led.h"
#include "ses_adc.h"
#include "ses_button.h"
#include <time.h>

int main(void)
{
    uint8_t  adc_channel = ADC_TEMP_CH;
    uint16_t  adc,adc_raw = 0;
    uart_init(57600);
  /*  led_redInit();
    led_greenInit();
    led_redOff();
    led_greenOff();
    button_init();*/
    adc_init();

    while (1)
    { 
       // adc_raw = adc_read(adc_channel);
        adc= adc_getTemperature();
        //fprintf(uartout, "ADC value %d\n",adc_raw);
        fprintf(uartout, "TempC= %d\n", adc);
        _delay_ms(2500);
        //fprintf(uartout, "TempC= %d", adc++);
        
       /*if(button_isRotaryPressed())
        {
            led_redOn();
        }
        else
        {
            led_redOff();
        }
         if(button_isJoystickPressed())
        {
            led_greenOn();
        }
        else
        {
            led_greenOff();
        }*/
        
    }
    return 0;
    /*uart_init(57600);
   
    while (1)
    {
        _delay_ms(1000);
        fprintf(uartout, "START\n");
        fprintf(lcdout, "START");
    }
    return 0;*/
}