/* INCLUDES ******************************************************************/

#include "ses_common.h"
#include "ses_button.h"
#include "ses_led.h"

/* DEFINES & MACROS **********************************************************/
#define BUTTON_NUM_DEBOUNCE_CHECKS 5
#define BUTTON_DEBOUNCE_PERIOD 6
#define BUTTON_DEBOUNCE_POS_JOYSTICK 0x02
#define BUTTON_DEBOUNCE_POS_ROTARY 0x01

#define BUTTON_ROTARY_PORT PORTB
#define BUTTON_ROTARY_PIN 6

#define BUTTON_JOYSTICK_PORT PORTB
#define BUTTON_JOYSTICK_PIN 7

/* variables **********************************************************/

static volatile pButtonCallback rotary_pointer;
static volatile pButtonCallback joystick_pointer;

/* FUNCTION DEFINITION *******************************************************/

void button_init(bool debouncing)
{

    DDR_REGISTER(BUTTON_ROTARY_PORT) &= ~(1 << BUTTON_ROTARY_PIN); //Makes it as Input pin
    DDR_REGISTER(BUTTON_JOYSTICK_PORT) &= ~(1 << BUTTON_JOYSTICK_PIN);

    BUTTON_ROTARY_PORT |= (1 << BUTTON_ROTARY_PIN); // Activates internal pull-ups
    BUTTON_JOYSTICK_PORT |= (1 << BUTTON_JOYSTICK_PIN);

    if (debouncing) // checks whether debounced button are used?
    {
        timer1_setCallback(button_checkState);
    }
    else
    {
        PCICR |= (1 << PCIE0);
        PCMSK0 |= (1 << BUTTON_JOYSTICK_PIN) | (1 << BUTTON_ROTARY_PIN);
    }
}

bool button_isJoystickPressed(void)
{
    if ((PIN_REGISTER(BUTTON_JOYSTICK_PORT) & (1 << BUTTON_JOYSTICK_PIN)) == (1 << BUTTON_JOYSTICK_PIN))
    {
        return false;
    }
    else
    {
        return true; // When button is pressed pin is active low, it returns button status
    }
}

bool button_isRotaryPressed(void)
{

    if ((PIN_REGISTER(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN)) == (1 << BUTTON_ROTARY_PIN))
    {
        return false;
    }
    else
    {
        return true; // When button is pressed pin is active low, it returns button status
    }
}

void button_setRotaryButtonCallback(pButtonCallback callback)
{
    /* Make sure that a button callback is only executed if a valid callback was set and the mask register
contains a 1*/
    if (callback != NULL)
    {
        rotary_pointer = callback;
    }
}

void button_setJoystickButtonCallback(pButtonCallback callback)
{
    /* Make sure that a button callback is only executed if a valid callback was set and the mask register
contains a 1*/
    if (callback != NULL)
    {
        joystick_pointer = callback;
    }
}

void button_checkState()
{
    static uint8_t state[BUTTON_NUM_DEBOUNCE_CHECKS] = {};
    static uint8_t index = 0;
    static uint8_t debouncedState = 0;
    uint8_t lastDebouncedState = debouncedState;
    static uint8_t count = 0;
    // each bit in every state byte represents one button
    state[index] = 0;
    if (button_isJoystickPressed())
    {
        state[index] |= BUTTON_DEBOUNCE_POS_JOYSTICK;
    }
    if (button_isRotaryPressed())
    {
        state[index] |= BUTTON_DEBOUNCE_POS_ROTARY;
    }
    index++;
    if (index == BUTTON_NUM_DEBOUNCE_CHECKS)
    {
        index = 0;
    }
    // init compare value and compare with ALL reads, only if
    // we read BUTTON_NUM_DEBOUNCE_CHECKS consistent "1's" in the state
    // array, the button at this position is considered pressed
    uint8_t j = 0xFF;
    for (uint8_t i = 0; i < BUTTON_NUM_DEBOUNCE_CHECKS; i++)
    {
        j = j & state[i];
    }
    debouncedState = j;

    while ((debouncedState && lastDebouncedState) != 0)
    {
        count++;

        if (count == BUTTON_DEBOUNCE_PERIOD)
        {
            if (debouncedState == BUTTON_DEBOUNCE_POS_ROTARY)
            {
                rotary_pointer();
                count = 0;
                debouncedState = 0;
            }
            if (debouncedState == BUTTON_DEBOUNCE_POS_JOYSTICK)
            {
                joystick_pointer();
                count = 0;
                debouncedState = 0;
            }
        }
    }
   
}

ISR(PCINT0_vect)
{
    // Checks if rotary button values is changed
    if ((PIN_REGISTER(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN)) == 0)
    {
        rotary_pointer();
    }

    // Checks if joystick button values is changed
    if ((PIN_REGISTER(BUTTON_JOYSTICK_PORT) & (1 << BUTTON_JOYSTICK_PIN)) == 0)
    {
        joystick_pointer();
    }
}