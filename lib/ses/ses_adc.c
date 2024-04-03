/* INCLUDES ******************************************************************/

#include "ses_adc.h"
#include <avr/io.h>

/* DEFINES & MACROS **********************************************************/

#define TEMPERATURE_SENSOR_PORT PORTF

#define LIGHT_SENSOR_PORT PORTF

#define Josystick_PORT PORTF

#define ADC_VREF_SRC 0x3

/* FUNCTION DEFINITION *******************************************************/

void adc_init(void)
{
  // Configuring Temprature,light sensor and joustick ADC channels as input
  DDR_REGISTER(TEMPERATURE_SENSOR_PORT) &= ~(1 << ADC_TEMP_CH);
  DDR_REGISTER(LIGHT_SENSOR_PORT) &= ~(1 << ADC_LIGHT_CH);
  DDR_REGISTER(Josystick_PORT) &= ~(1 << ADC_JOYSTICK_CH);

  // Deactivates intenal pull-ups for Temprature,light sensor and joustick ADC channels
  TEMPERATURE_SENSOR_PORT &= ~(1 << ADC_TEMP_CH);
  LIGHT_SENSOR_PORT &= ~(1 << ADC_LIGHT_CH);
  Josystick_PORT &= ~(1 << ADC_JOYSTICK_CH);

  // Disable power reduction mode for ADC
  PRR0 &= ~(1 << PRADC);

  // Setting VREF = 1.6V
  ADMUX |= (ADC_VREF_SRC << REFS0);

  // Configured to have result right adjusted in Data Register
  ADMUX &= ~(1 << ADLAR);

  // Sets prescaler of 16 which makes ADC clock = 1MHZ from this calculation , CPu_clock/16 =  16MHz/16
  ADCSRA |= (ADC_PRESCALE << ADPS2);

  //Unselects the Auto trigerring
  ADCSRA &= ~(1 << ADATE);

  //Enables ADC
  ADCSRA |= (1 << ADEN);
}

uint16_t adc_read(uint8_t adc_channel)
{
  // Initially clearing Mux bits
  ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4));

  //Choosing ADC channel
  switch (adc_channel)
  {
  case ADC_MIC_NEG_CH:
    ADMUX = ADMUX;
    break;

  case ADC_MIC_POS_CH:
    ADMUX |= (1 << MUX0);
    break;

  case ADC_TEMP_CH:
    ADMUX |= (1 << MUX1);
    break;

  case ADC_RESERVED1_CH:
    ADMUX |= (1 << MUX0) | (1 << MUX1);

  case ADC_LIGHT_CH:
    ADMUX |= (1 << MUX2);
    break;

  case ADC_JOYSTICK_CH:
    ADMUX |= (1 << MUX0) | (1 << MUX2);
    break;

  case ADC_RESERVED2_CH:
    ADMUX |= (1 << MUX1) | (1 << MUX2);

  case ADC_RESERVED3_CH:
    ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2);

  default:
    return ADC_INVALID_CHANNEL;
  }

  // starts ADC conversion
  ADCSRA |= (1 << ADSC);

  while (ADCSRA & (1 << ADSC) == (1 << ADSC))
    ;

  return ADC;
}

int16_t adc_getTemperature()
{
  // Temprature range: 15C ,....., 40C
  int16_t adc = adc_read(ADC_TEMP_CH);

  const int ADC_TEMP_MAX = 313;  // Coressponds to 40C
  const int ADC_TEMP_MIN = 288;  // Coressponds to 15C
  uint8_t ADC_TEMP_FACTOR = 100; //upscale factor

  /** Manual calculation of thermister Resistance for these temprature thresholds by pluging it in 
   * thermister characteristics equation
      
      Rt1= 5930
      Rt2 = 14600

      after it comes calcaulation of ADC **/
  const int ADC_TEMP_RAW_MAX = 258;
  const int ADC_TEMP_RAW_MIN = 568;

  int16_t slope = 100 * (ADC_TEMP_MAX - ADC_TEMP_MIN) / (ADC_TEMP_RAW_MAX - ADC_TEMP_RAW_MIN);
  int16_t offset = 100 * ADC_TEMP_MIN; //(ADC_TEMP_RAW_MAX * slope);
  const int ADC_UPDATE = ((adc - ADC_TEMP_RAW_MIN) * slope);
  //int adcsum = adc * sum;
  //return adcsum / ADC_TEMP_FACTOR - 273;
  return ((ADC_UPDATE + offset) / 100 - 273);
}
