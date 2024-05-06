
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

#define  UART_DOUBLE_SPEED
#define  UART_ENABLE_TX
#define  UART_ENABLE_RX
#define  UART_ENABLE_RX_INT
#define  UART_BUF_SIZE      200

typedef struct uart_t{
  #ifdef UART_BUF_SIZE
  volatile uint8_t Buf[UART_BUF_SIZE];
  #else
  volatile uint8_t Buf[140];
  #endif
  volatile uint16_t BufIndex;
}uart_t;

uart_t UART;

void UART_Struct_Init(void){
  #ifdef UART_BUF_SIZE
  for(uint16_t i=0;i<UART_BUF_SIZE;i++){
    UART.Buf[i]=0;
  }
  #else
  for(uint16_t i=0;i<140;i++){
    UART.Buf[i]=0;
  }
  #endif
  UART.BufIndex=0;
}

void UART_Clear_Registers(void){
  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
  UBRR0H = 0;
  UBRR0L = 0;
}

void UART_Config_Transmitter(void){
  UCSR0B|= (1<<TXEN0);
  if((UCSR0C & 0x06) != 0x06){
    UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
  }
}

void UART_Config_Receiver(void){
  UCSR0B|=(1<<RXEN0);
  if((UCSR0C & 0x06) != 0x06){
    UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
  }
}

void UART_Config_Receiver_Interrupt(void){
  UCSR0B|=(1<<RXCIE0);
  sei();
}

void UART_Transmit_Byte(uint8_t val){
  UDR0=val;
  while((UCSR0A & (1<<UDRE0))==0);
}


uint8_t UART_Receive_Byte(void){
  return UDR0;
}


ISR(USART_RX_vect){
  UART.Buf[UART.BufIndex] = UART_Receive_Byte();
  UART.BufIndex++;
  #ifdef UART_BUF_SIZE
  if(UART.BufIndex>=UART_BUF_SIZE){
    UART.BufIndex=0;
  }
  #else
  if(UART.BufIndex>=140){
    UART.BufIndex=0;
  }
  #endif
}






uint16_t UART_Get_Buf_Index(void){
  return UART.BufIndex;
}


void UART_Init(uint32_t BAUD){
  cli();
  UART_Struct_Init();
  UART_Clear_Registers();
  
  #ifdef UART_DOUBLE_SPEED
  uint32_t UBRR_VAL = ((F_CPU/8)/BAUD)-1 ;
  UCSR0A|=(1<<U2X0);  
  #else
  UCSR0A&=~(1<<U2X0);  
  uint32_t UBRR_VAL = ((F_CPU/16)/BAUD)-1 ;
  #endif
  
  UBRR0H = (uint8_t)((UBRR_VAL >> 8) & 0xFF);
  UBRR0L = (uint8_t)(UBRR_VAL & 0xFF) ;
  
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