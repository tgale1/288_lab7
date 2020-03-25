/*
*
*   uart.c
*
*
*
*   @author
*   @date
*/

#include "uart.h"
#include <stdint.h>


void uart_init(void){
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0b000010;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0b000010 ) == 0) {};
  while ((SYSCTL_PRUART_R & 0b000010 ) == 0) {};

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0b00000011;

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0b00000011;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R &= 0x00000000;
  GPIO_PORTB_PCTL_R |= 0x00000011;

  //calculate baud rate com1
  //uint16_t iBRD = 0b01101000; //use equations
  //uint16_t fBRD = 0b001011; //use equations

  //wifi 16
  uint16_t iBRD = 8;//0b00001000; //use equations
  uint16_t fBRD = 44;//0b00101100; //use equations

  //wifi 8
  //uint16_t iBRD = 0b00010001; //use equations
  //uint16_t fBRD = 0b00010111; //use equations

  //turn off UART1 while setting it up
  UART1_CTL_R &= 0;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R = iBRD;
  UART1_FBRD_R = fBRD;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = 0b01100000;

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  UART1_CTL_R = 0b01100000001;

}

void uart_sendChar(char data){
	while(UART1_FR_R & 0x20){}
	UART1_DR_R = data;
}

char uart_receive(void){
	char data = 0;

	while(UART1_FR_R & UART_FR_RXFE){}

	data = (char)(UART1_DR_R & 0XFF);

	return data;
}

void uart_sendStr(const char *data){
    /*
    char[20] words;
    int counter = 0;

    while(*data != '\0')
    {
        char[counter] = data;
        counter++
        data++;
    }
    */
}
