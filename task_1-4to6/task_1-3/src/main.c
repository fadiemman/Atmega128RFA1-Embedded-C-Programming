#include <stdio.h>
#include <stdint.h>

uint8_t reversrBit(uint8_t num )
{
    uint8_t rev_num,size = 0 ; 
    size = sizeof(num)*8;
    for (int i = 0; i <size ; i++ )
    {
          rev_num |= (((num >> i) & 1) << (size-1-i));
    }
    return rev_num;
}

int main(void)
{
    uint8_t num,rev_number;
    printf("Enter the number: ");
     scanf("%ld",&num);
     rev_number=reversrBit(num);
     printf("rev_num= %ld\n",rev_number);

     return 0;

}