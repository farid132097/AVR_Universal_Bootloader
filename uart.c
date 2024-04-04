
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

#define  UART_DOUBLE_SPEED
#define  UART_ENABLE_TX
#define  UART_ENABLE_RX
#define  UART_ENABLE_RX_INT

void UART_Clear_Registers(void){
  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
  UBRR0H = 0;
  UBRR0L = 0;
  UDR0   = 0;
}

void UART_Config_Transmitter(void){
  UCSR0B|=(1<<TXEN0);
  UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
}

void UART_Config_Receiver(void){
  UCSR0B|=(1<<RXEN0);
  UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
}

void UART_Config_Receiver_Interrupt(void){
  UCSR0B|=(1<<RXCIE0);
  sei();
}

void UART_Init(uint32_t BAUD){
  UART_Clear_Registers();
  
  #ifdef UART_DOUBLE_SPEED
  uint32_t UBRR_VAL = ((F_CPU/8)/BAUD)-1 ;
  UCSR0A|=(1<<U2X0);  
  #else
  UCSR0A&=~(1<<U2X0);  
  uint32_t UBRR_VAL = ((F_CPU/16)/BAUD)-1 ;
  #endif
  UBRR0H = (UBRR_VAL >> 8) & 0xFF;
  UBRR0L = UBRR_VAL & 0xFF ;
  
  #ifdef UART_ENABLE_TX
    UART_Config_Transmitter();
  #endif
  
  #ifdef UART_ENABLE_RX
    UART_Config_Receiver();
  #endif
  
  #ifdef UART_ENABLE_RX_INT
    UART_Config_Receiver_Interrupt();
  #endif
}

void UART_Transmit_Byte(uint8_t val){
  UDR0=val;
  while((UCSR0A & (1<<UDRE0))==0);
}