#include <stdint.h>
#include <avr/io.h>

void wait(uint16_t millis);

// Program to toggle red Led after exactly 1 second
//calculation 1 sec = (65535*4*x)/16MHZ 
// x=60 

int main()
{

    DDRG |= 0x02;
    while (1)
    {
        wait(0xFFFF); // Max value possible to obtain delay of 1 sec
        PORTG ^= 0x02;
    }
    return 0;
}
void wait(uint16_t millis)
{
    uint8_t i;
    while (millis > 0)
    {
        for (i = 0x0F; i > 0; i--) //Each iteration for millis is followed by 15 iterations of i to execute 60 instructions 
        {
            //prevent code optimization by using inline assembler
            asm volatile("nop"); // one cycle with no operation
        }
        millis--;
    }
}

void manualDelay(void){
    uint16_t i;
    for(i=0x03ca; i>0; i--){
        asm volatile("nop");
    }
    
}