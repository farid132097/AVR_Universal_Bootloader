

void     Boot_Relocate_Vector_Table_To_Bootloader(void);
void     Boot_Relocate_Vector_Table_To_App(void);
uint16_t Boot_Calcuate_CRC(uint16_t crc, uint8_t data);
uint16_t Boot_Calcuate_CRC_Block(uint8_t *buf, uint8_t len);
void     Boot_Flush_Config_Packet(void);
void     Boot_Build_Config_Packet(void);
void     Boot_Send_Config_Packet(void);
void     Boot_Init(void);