
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "boot.h"


int main(void){

  Boot_Init();
  
  if(UART_Get_Buf_Index()==12){
    DDRD |= (1<<4);
	PORTD|= (1<<4);
  }else{
    DDRD |= (1<<4);
	PORTD&=~(1<<4);
  }
  UART_Transmit_Byte(UART_Get_Buf_Index());
  
  while(1){
	
	_delay_ms(200);
  }
}