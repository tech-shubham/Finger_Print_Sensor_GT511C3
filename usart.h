/***************************************************************************
*
*
* usart.h
*
* Created by Shubham Gupta 
* Copyright (c) shubhamgupta.org  and/or its affiliates.
* All rights reserved.
***************************************************************************/

#ifndef __USART_HEADER_H__
#define __USART_HEADER_H__

#include "avrCommon.h"


/////////////////////////////////// For USART	////////////////////////////////////

#define AB_USART_0	0
#define AB_USART_1	1
#define AB_USART_2	2 
#define AB_USART_3	3 


////////// Parity /////////////////////
#define AB_NONE	0x00
#define AB_EVEN	0x20
#define AB_ODD		0x30

/////// Stop Bits ////////////////////////
#define AB_ONE_BIT	0X00
#define AB_TWO_BIT 0X08

/////// Character Size ///////////////

#define AB_FIVE_BIT	0x00
#define AB_SIX_BIT		0x01
#define	AB_SEVEN_BIT	0x02
#define	AB_EIGHT_BIT	0x03
#define AB_NINE_BIT	0x07

///////// Baud Rate	////////////////////

#define AB_B_2400		832
#define AB_B_4800		416
#define AB_B_9600		207
#define AB_B_14400		138
#define AB_B_19200		103
#define AB_B_28800		68
#define	AB_B_38400		51
#define AB_B_57600		34
#define	AB_B_76800		25
#define AB_B_115200		16
#define AB_B_230400		8
#define	AB_B_250000		7


#define AB_USART_INTERRUPT_ENABLE	0x80
#define AB_USART_INTERRUPT_DISABLE	0x00




void usartInit(unsigned int usart_port, unsigned baud_rate, unsigned char parity, unsigned char stop_bit, unsigned char bit_size, unsigned char isr_mode);
void sendCharacterUsart(unsigned int usart_port,unsigned char c_SendByte);
void sendChar(unsigned int usart_port,unsigned char ch);
void sendString(unsigned int usart_port,char *str);
int stringCompare(unsigned int usart_port,char *strr,unsigned int str_length);
void clearBuffer2(unsigned int usart_port);
void sendUData(unsigned int usart_port, unsigned char *str,unsigned int length);
unsigned char uartRecieve(unsigned int usart_port);
void sendUInteger(unsigned int usart_port,unsigned int value);




#endif //__USART_HEADER_H__