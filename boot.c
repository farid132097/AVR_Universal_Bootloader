
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "boot.h"

#define  BOOT_BUF_SIZE    140          //Max buf len
#define  BOOT_SYNC_BYTE   0x55AA       //2 Byte
#define  BOOT_DEV_SIG     0x001E950F   //4 Byte
#define  BOOT_BAUD_RATE   38400        //4 Byte

uint8_t  boot_buf[140];

typedef struct boot_t{
  volatile uint8_t Buf[BOOT_BUF_SIZE];
  volatile uint8_t BufIndex;
  volatile uint8_t PageIndex;
  volatile uint8_t SubPageIndex;
}boot_t;

boot_t Boot;


void Boot_Struct_init(void){
  Boot_Flush_Buf();
  Boot.PageIndex=0;
  Boot.SubPageIndex=0;
}

void Boot_Flush_Buf(void){
  for(uint16_t i=0;i<BOOT_BUF_SIZE;i++){
    Boot.Buf[i]=0;
  }
  Boot.BufIndex=0;
}


void Boot_RVT_Bootloader(void){
  if((MCUCR & (1<<IVCE))==0){
    MCUCR = (1<<IVCE);
    MCUCR = (1<<IVSEL);
  }
}

void Boot_RVT_App(void){
  if(MCUCR & (1<<IVCE)){
    MCUCR = (1<<IVCE);
    MCUCR = 0;
  }
}

uint16_t Boot_CRC16(uint16_t crc, uint8_t data){
  crc=crc^((uint16_t)data<<8);
  for(uint8_t i=0;i<8;i++){
    if(crc & 0x8000){
	  crc=(crc<<1)^0x1021;
	}
    else{
	  crc<<=1;
	}
  }
  return crc;
}

uint16_t Boot_CRC16_Block(uint8_t *buf, uint8_t len){
  uint16_t crc=0;
  for(uint8_t i=0;i<len;i++){
    crc=Boot_CRC16(crc,buf[i]);
  }
  return crc;
}

void Boot_Config_Add_Field(uint32_t val, uint8_t len){
  for(uint8_t i=len;i>0;i--){
    Boot.Buf[Boot.BufIndex+i-1]= val & 0xFF;
	val>>=8;
  }
  Boot.BufIndex+=len;
}

void Boot_Build_Config_Packet(void){
  Boot_Flush_Buf();
  Boot_Config_Add_Field(BOOT_SYNC_BYTE,2);
  Boot_Config_Add_Field(BOOT_DEV_SIG,  4);
  Boot_Config_Add_Field(BOOT_BAUD_RATE,4);
  uint16_t temp = Boot_CRC16_Block(Boot.Buf, Boot.BufIndex);
  Boot_Config_Add_Field(temp,2);
}

void Boot_Send_Config_Packet(void){
  for(uint8_t i=0;i<Boot.BufIndex;i++){
    UART_Transmit_Byte(Boot.Buf[i]);
  }
}


void Boot_Init(void){
  Boot_RVT_Bootloader();
  Boot_Build_Config_Packet();
  UART_Init(9600);
  Boot_Send_Config_Packet();
  _delay_ms(5);
  UART_Init(BOOT_BAUD_RATE);
  _delay_ms(30);
}