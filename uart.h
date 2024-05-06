
void     UART_Struct_Init(void);
void     UART_Clear_Registers(void);
void     UART_Config_Transmitter(void);
void     UART_Config_Receiver(void);
void     UART_Config_Receiver_Interrupt(void);
void     UART_Transmit_Byte(uint8_t val);
uint8_t  UART_Receive_Byte(void);


uint16_t UART_Get_Buf_Index(void);

void     UART_Init(uint32_t BAUD);