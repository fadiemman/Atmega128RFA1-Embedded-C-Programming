/* INCLUDES ******************************************************************/
#include "ses_common.h"
#include "ses_pwm.h"

/* DEFINES & MACROS **********************************************************/
#define PWM_PORT PORTG
#define PWM_PIN 5

#define DISABLE_PRESCALAR 0x01

/* FUNCTION DEFINITION *******************************************************/


// Intializes PWM from TIMER0 
void pwm_init(void)
{
    PRR0 &= ~(1 << PRTIM0); // enables Timer0

    DDR_REGISTER(PWM_PORT) |= (1 << PWM_PIN); // Initializes Pwm pin as output

    TCCR0A |= (1 << COM0B0)| (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Selects Inverted-Fast PWM Mode , Top = 0xFF and generates OCOB at pin

    TCCR0B |= (DISABLE_PRESCALAR << CS00); // Disables prescalaing

    OCR0B = 0xFF; // Stops Motor initially
}

// Sets the DutyCycles for PWM wave
void pwm_setDutyCycle(uint8_t dutyCycle)
{
    OCR0B = dutyCycle;
}