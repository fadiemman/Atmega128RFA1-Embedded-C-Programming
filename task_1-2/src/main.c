#include <stdint.h>
#include <avr/io.h>

//Total number of cycles = 4*ldi + 256*nop + 256*sbiw + 255*brne + 1*ret
//Total number of cycles = 4*1 + 256*1 + 256*2 + 255*1 + 1*1
// Total number of cycles = 1028

void shortDelay (void)
{
uint16_t i; // 16 bit unsigned integer
for (i = 0x0100; i > 0 ; i--) {
//prevent code optimization by using inline assembler
asm volatile ( "nop" ); // one cycle with no operation
}
}
int main(){

 shortDelay();
 return 0;

}
