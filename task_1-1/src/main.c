#include <avr/io.h>
#include <util/delay.h>

/**Toggles the red LED of the SES-board*/
int main(void) {
	DDRG |= 0x02;  //DDRG selects the direction of the Port G register

	while(1){
		_delay_ms(1000);
		PORTG ^= 0x02;  //It's a data register which contain data to be store in controller 
	}
	return 0;
}
