
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "boot.h"


int main(void){

  UART_Init(9600);
  UART_Transmit_Byte(0x38);
  
  while(1){
	
	_delay_ms(200);
  }
}