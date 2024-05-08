

void     Boot_Struct_init(void);
void     Boot_Flush_Buf(void);

void     Boot_RVT_Bootloader(void);
void     Boot_RVT_App(void);
uint16_t Boot_CRC16(uint16_t crc, uint8_t data);
uint16_t Boot_CRC16_Block(uint8_t *buf, uint8_t len);
void     Boot_Config_Add_Field(uint32_t val, uint8_t len);
void     Boot_Build_Config_Packet(void);
void     Boot_Send_Config_Packet(void);
void     Boot_Init(void);