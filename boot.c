
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "boot.h"

#define  BOOT_SYNC_BYTE   0x55         /*1 Byte*/
#define  BOOT_DEV_SIG     0x001E950F   /*4 Byte*/
#define  BOOT_BAUD_RATE   38400        /*3 Byte*/

uint8_t  boot_config_packet[10];

void Boot_Relocate_Vector_Table_To_Bootloader(void){
  if((MCUCR & (1<<IVCE))==0){
    MCUCR = (1<<IVCE);
    MCUCR = (1<<IVSEL);
  }
}

void Boot_Relocate_Vector_Table_To_App(void){
  if(MCUCR & (1<<IVCE)){
    MCUCR = (1<<IVCE);
    MCUCR = 0;
  }
}

uint16_t Boot_Calcuate_CRC(uint16_t crc, uint8_t data){
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

uint16_t Boot_Calcuate_CRC_Block(uint8_t *buf, uint8_t len){
  uint16_t crc=0;
  for(uint8_t i=0;i<len;i++){
    crc=Boot_Calcuate_CRC(crc,buf[i]);
  }
  return crc;
}

void Boot_Flush_Config_Packet(void){
  for(uint8_t i=0;i<10;i++){
    boot_config_packet[i]=0;
  }
}

void Boot_Build_Config_Packet(void){
  boot_config_packet[0] = BOOT_SYNC_BYTE;
  boot_config_packet[1] = (BOOT_DEV_SIG>>24  ) & 0xFF;
  boot_config_packet[2] = (BOOT_DEV_SIG>>16  ) & 0xFF;
  boot_config_packet[3] = (BOOT_DEV_SIG>>8   ) & 0xFF;
  boot_config_packet[4] = (BOOT_DEV_SIG>>0   ) & 0xFF;
  boot_config_packet[5] = (BOOT_BAUD_RATE>>16) & 0xFF;
  boot_config_packet[6] = (BOOT_BAUD_RATE>>8 ) & 0xFF;
  boot_config_packet[7] = (BOOT_BAUD_RATE>>0 ) & 0xFF;
  uint16_t temp = Boot_Calcuate_CRC_Block(boot_config_packet, 8);
  boot_config_packet[8] = (temp>>8 ) & 0xFF;
  boot_config_packet[9] = (temp>>0 ) & 0xFF;
}

void Boot_Send_Config_Packet(void){
  for(uint8_t i=0;i<10;i++){
    UART_Transmit_Byte(boot_config_packet[i]);
  }
}


void Boot_Init(uint32_t Baud_Rate){
  Boot_Relocate_Vector_Table_To_Bootloader();
  Boot_Build_Config_Packet();
  UART_Init(Baud_Rate);
  Boot_Send_Config_Packet();
}