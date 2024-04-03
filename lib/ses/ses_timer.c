/* INCLUDES ******************************************************************/
#include "ses_timer.h"
#include "ses_led.h"

/** DEFINES & MACROS **********************************************************/
#define TIMER1_CYC_FOR_5MILLISEC 0x04E2 //TODO
#define TIMER2_CYC_FOR_1MILLISEC 0xFA	//TODO

static volatile pTimerCallback timer2_Callback;
static volatile pTimerCallback timer1_Callback;

/*FUNCTION DEFINITION ********************************************************/
void timer2_setCallback(pTimerCallback cb)

{
	timer2_Callback = cb;
}

void timer2_start()
{

	// OOCR2A = 0;
	TCCR2A |= (1 << WGM21); // enables Clear Timer on Compare Match (CTC) mode operation

	OCR2A = TIMER2_CYC_FOR_1MILLISEC; // Sets the Top value for comparing with the counter

	TCCR2B |= (1 << CS22); // sets a prescaler of 64

	TIMSK2 |= (1 << OCIE2A); // Set interrupt mask register for Compare A

	TCNT2 = 0;

	TIFR2 = (1 << OCF2A); //Clear the interrupt flag by setting a 1 in flag register for Compare A.
}

void timer2_stop()
{
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20)); //No clock source (Timer/Counter2 stopped)
}

void timer1_setCallback(pTimerCallback cb)
{
	timer1_Callback = cb;
}

void timer1_start()
{
	TCCR1B |= (1 << WGM12); // enables Clear Timer on Compare Match (CTC) mode operation

	OCR1A = TIMER1_CYC_FOR_5MILLISEC; // Sets the Top value for comparing with the counter

	TCCR1B |= (1 << CS11) | (1 << CS10); // sets a prescaler of 64

	TIMSK1 |= 1 << OCIE1A; // Set interrupt mask register for Compare A

	//TIFR2 |= (1 << OCF2A); //Clear the interrupt flag by setting a 1 in flag register for Compare A.
}

void timer1_stop()
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS20)); // stops the the timer1
}

ISR(TIMER1_COMPA_vect)
{
	if (timer1_Callback != NULL)
	{
		timer1_Callback();
	}
}

ISR(TIMER2_COMPA_vect)
{
	// TODO
	if (timer2_Callback != NULL)
	{
		timer2_Callback();
	}
}
