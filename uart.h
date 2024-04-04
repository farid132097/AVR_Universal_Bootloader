

void UART_Clear_Registers(void);
void UART_Config_Transmitter(void);
void UART_Config_Receiver(void);
void UART_Config_Receiver_Interrupt(void);
void UART_Init(uint32_t BAUD);
void UART_Transmit_Byte(uint8_t val);