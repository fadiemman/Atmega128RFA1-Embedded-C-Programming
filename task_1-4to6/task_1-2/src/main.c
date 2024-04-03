#include <stdio.h>
#include <stdint.h>

//Program took an unsigned 8 bit integer and perform maximum tasks given in Task1_5

uint8_t bitVariable;

int main(void) {
    //Setting bit number 3
    bitVariable |= 1 << 3;
    printf("result of bit setting=%d\n", bitVariable);

    //Setting bit number 4 and 6 without shifting
    bitVariable |= (1 << 4 ) & (1 << 6);
    printf("result of bit setting=%d\n", bitVariable);

    //Clearing bit number 2
    bitVariable &= ~(1 << 2);

    //Clearing bit 2 and 7
    bitVariable &= ~(1 << 2) & ~(1 << 7);

    //Toggle bit 3
    bitVariable ^= 1 << 3;

    //Set bit 2 and Clear bit 5 and 7 at the same time
    bitVariable  = (bitVariable | (1 << 2)) & (~(1 << 5) & ~(1 << 7));

    return 0;
}
