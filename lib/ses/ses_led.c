/* INCLUDES ******************************************************************/

#include "ses_common.h"
#include "ses_led.h"

/* DEFINES & MACROS **********************************************************/

// LED wiring on SES board
#define LED_RED_PORT PORTG
#define LED_RED_PIN 1

#define LED_YELLOW_PORT PORTF
#define LED_YELLOW_PIN 7

#define LED_GREEN_PORT PORTF
#define LED_GREEN_PIN 6

/* FUNCTION DEFINITION *******************************************************/

void led_redInit(void)
{
    DDR_REGISTER(LED_RED_PORT) |= (1 << LED_RED_PIN); // Initializes red led
    LED_RED_PORT |= 1 << LED_RED_PIN;                 // Turns Off Red led at first place
}

void led_redToggle(void)
{
    LED_RED_PORT ^= 1 << LED_RED_PIN; // toggles red led on board
}

void led_redOn(void)
{

    LED_RED_PORT &= ~(1 << LED_RED_PIN); // Turns the red led on
}

void led_redOff(void)
{

    LED_RED_PORT |= 1 << LED_RED_PIN; // Turns the red led off
}

void led_yellowInit(void)
{

    DDR_REGISTER(LED_YELLOW_PORT) |= (1 << LED_YELLOW_PIN); //Initializes yellow led
    LED_YELLOW_PORT |= (1 << LED_YELLOW_PIN);               // Turns Off Yellow led at first place
}

void led_yellowToggle(void)
{

    LED_YELLOW_PORT ^= (1 << LED_YELLOW_PIN); //Toggles yellow led
}

void led_yellowOn(void)
{

    LED_YELLOW_PORT &= ~(1 << LED_YELLOW_PIN); //Turns yellow led on
}

void led_yellowOff(void)
{

    LED_YELLOW_PORT |= (1 << LED_YELLOW_PIN); //Turns off yellow led
}

void led_greenInit(void)
{

    DDR_REGISTER(LED_GREEN_PORT) |= (1 << LED_GREEN_PIN); //Initializes green led
    LED_GREEN_PORT |= (1 << LED_GREEN_PIN);               //Turns off yellow led at First place
}

void led_greenToggle(void)
{

    LED_GREEN_PORT ^= (1 << LED_GREEN_PIN); //Toggles green led
}

void led_greenOn(void)
{

    LED_GREEN_PORT &= ~(1 << LED_GREEN_PIN); //Turns on green led
}

void led_greenOff(void)
{

    LED_GREEN_PORT |= (1 << LED_GREEN_PIN); //Turns off green led
}
