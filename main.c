
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "boot.h"


int main(void){

  Boot_Init();
  
  
  
  while(1){
	
	_delay_ms(200);
  }
}