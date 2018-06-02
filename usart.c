/***************************************************************************
*
*
* usart.c
*
* Created by Shubham Gupta 
* Copyright (c) shubhamgupta.org  and/or its affiliates.
* All rights reserved.
***************************************************************************/

#include "usart.h"



volatile unsigned char xReceiveBuffer0[50];
volatile unsigned char xReceiveBuffer1[50];
volatile unsigned char xReceiveBuffer2[50];
volatile unsigned char xReceiveBuffer3[50];
volatile unsigned char testChar = ' ';
unsigned int recieveInc=0;




//////////////////////////// USART Communication ///////////////////////////////////////////////////////////////////////

void usartInit(unsigned int usart_port, unsigned baud_rate, unsigned char parity, unsigned char stop_bit, unsigned char bit_size, unsigned char isr_mode)
{

	
	if(usart_port==AB_USART_0)
	{
		UCSR0A |=(1<<U2X0);
		UCSR0B |= (1 << RXEN0) | (1<<TXEN0) | isr_mode;   // Turn on the transmission and reception circuitry
		UCSR0C |= ((parity|stop_bit|bit_size)|(UCSR0C&0x00));
		UBRR0H = (baud_rate >> 8); // Load upper 8-bits of the baud rate value..
		UBRR0L = baud_rate ;// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
		//sei();
		return;
	}
	
	else if(usart_port==AB_USART_1)
	{
		UCSR1A |=(1<<U2X1);
		UCSR1B |= (1 << RXEN1) | (1<<TXEN1) | isr_mode;   // Turn on the transmission and reception circuitry
		UCSR1C |= ((parity|stop_bit|bit_size)|(UCSR1C&0x00));
		UBRR1H = (baud_rate >> 8); // Load upper 8-bits of the baud rate value..
		UBRR1L = baud_rate ;// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
		//sei();
		return;
	}
	
	else if(usart_port==AB_USART_2)
	{
		UCSR2A |=(1<<U2X2);
		UCSR2B |= (1 << RXEN2) | (1<<TXEN2) | isr_mode;   // Turn on the transmission and reception circuitry
		UCSR2C |= ((parity|stop_bit|bit_size)|(UCSR2C&0x00));
		UBRR2H = (baud_rate >> 8); // Load upper 8-bits of the baud rate value..
		UBRR2L = baud_rate ;// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
		//sei();
		return;
	}
	
	else if(usart_port==AB_USART_3)
	{
		UCSR3A |=(1<<U2X3);
		UCSR3B |= (1 << RXEN3) | (1<<TXEN3) | isr_mode;   // Turn on the transmission and reception circuitry
		UCSR3C |= ((parity|stop_bit|bit_size)|(UCSR3C&0x00));
		UBRR3H = (baud_rate >> 8); // Load upper 8-bits of the baud rate value..
		UBRR3L = baud_rate ;// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
		//sei();
		return;
	}
	
}

// function to receive data
unsigned char uartRecieve(unsigned int usart_port)
{
	
	if(usart_port==AB_USART_0)
	{

		while((UCSR0A & (1<<RXC0)) == 0);                   // wait while data is being received
		return UDR0;                                   // return 8-bit data
		
	}
	
	else if(usart_port==AB_USART_1)
	{

		while((UCSR1A & (1<<RXC1)) == 0);                   // wait while data is being received
		return UDR1;                                   // return 8-bit data
		
	}
	
	else if(usart_port==AB_USART_2)
	{

		while((UCSR2A & (1<<RXC2)) == 0);                   // wait while data is being received
		return UDR2;                                   // return 8-bit data
		
	}
	
	else if(usart_port==AB_USART_3)
	{

		while((UCSR3A & (1<<RXC3)) == 0);                   // wait while data is being received
		return UDR3;                                   // return 8-bit data
		
	}
	
	return 0;
}


void sendCharacterUsart(unsigned int usart_port, unsigned char c_SendByte) 
{
	if(usart_port==AB_USART_0)
	{
		while ( !( UCSR0A & (1<<UDRE0))) ;
		UDR0 = c_SendByte;
	}
	
	else if(usart_port==AB_USART_1)
	{
		while ( !( UCSR1A & (1<<UDRE1))) ;
		UDR1 = c_SendByte;
	}
	
	else if(usart_port==AB_USART_2)
	{
		
		while ( !( UCSR2A & (1<<UDRE2))) ;
		UDR2 = c_SendByte;
	}
	
	else if(usart_port==AB_USART_3)
	{
		while ( !( UCSR3A & (1<<UDRE3))) ;
		UDR3 = c_SendByte;
	}
} 
// end of sendCharacter(unsigned char sendByte)

void sendUData(unsigned int usart_port, unsigned char *str,unsigned int length)
{
	while(length>0)
	{
		if(usart_port==AB_USART_0)
		{
			while ( !( UCSR0A & (1<<UDRE0))) ;
			UDR0 = *str;
			str++;length--;
		}
		
		else if(usart_port==AB_USART_1)
		{
			while ( !( UCSR1A & (1<<UDRE1))) ;
			UDR1 = *str;
			str++;length--;
		}
		
		else if(usart_port==AB_USART_2)
		{
			while ( !( UCSR2A & (1<<UDRE2))) ;
			UDR2 = *str;
			str++;length--;
		}
		
		else if(usart_port==AB_USART_3)
		{
			while ( !( UCSR3A & (1<<UDRE3))) ;
			UDR3 = *str;
			str++;length--;
		}
		
		
	}
}

void sendChar(unsigned int usart_port,unsigned char ch)
{
	sendCharacterUsart(usart_port,ch);
	
	sendCharacterUsart(usart_port,'\0');
	_delay_ms(4);
}


void sendString(unsigned int usart_port,char *str)
{
	while(*str!='\0')
	{
		sendCharacterUsart(usart_port,*str);
		_delay_ms(4);
		str++;
	}
	
}


void sendUInteger(unsigned int usart_port,unsigned int value)
{
	char ubuf[10];
	itoa(value,ubuf,10);
	sendString(usart_port,ubuf);

}


ISR(USART2_RX_vect)
{

	
	testChar = UDR2;
	
	if(testChar!='\0')
	{
		xReceiveBuffer2[recieveInc]=testChar;
		recieveInc++;
	}
	else
	{
	
	xReceiveBuffer2[recieveInc]='\0';
	recieveInc=0;
	}
	
	
 
}

ISR(USART0_RX_vect)
{
	
		testChar = UDR0;
	
		if(testChar!='\0')
		{
			xReceiveBuffer0[recieveInc]=testChar;
			recieveInc++;
		}
		else
		{
	
		xReceiveBuffer0[recieveInc]='\0';
		recieveInc=0;
		}
	
	
	
 
}

ISR(USART1_RX_vect)
{
	
		testChar = UDR1;
	
		if(testChar!='\0')
		{
			xReceiveBuffer1[recieveInc]=testChar;
			recieveInc++;
		}
		else
		{
	
		xReceiveBuffer1[recieveInc]='\0';
		recieveInc=0;
		}
	
	
	
 
}

ISR(USART3_RX_vect)
{
	
		testChar = UDR3;
	
		if(testChar!='\0')
		{
			xReceiveBuffer3[recieveInc]=testChar;
			recieveInc++;
		}
		else
		{
	
		xReceiveBuffer3[recieveInc]='\0';
		recieveInc=0;
		}
	
	
	
 
}


int stringCompare(unsigned int usart_port,char *strr, unsigned int str_length)
{
	int right_flag=0,j=0,i=0;

	while(i<str_length)
	{
		if(usart_port == AB_USART_0)
		{
		
			if(*strr==xReceiveBuffer0[j])
			{
				right_flag=1;
			}
			else
			{
				right_flag=0;
				break;
			}
		}
		
		else if(usart_port == AB_USART_1)
		{
		
			if(*strr==xReceiveBuffer1[j])
			{
				right_flag=1;
			}
			else
			{
				right_flag=0;
				break;
			}
		}
		
		else if(usart_port == AB_USART_2)
		{
		
			if(*strr==xReceiveBuffer2[j])
			{
				right_flag=1;
			}
			else
			{
				right_flag=0;
				break;
			}
		}
		j++;strr++;
		
		if(j>=50)
		{
			break;
		}
	}

	return right_flag;
}


// This Method Clears the Received Buffer

void clearBuffer2(unsigned int usart_port)
{
	unsigned int num;
	if(usart_port==AB_USART_0)
	{
		for(num = 0 ; num < 50 ; num++)
		{
			xReceiveBuffer0[num] = ' ';
		}
	}
	
	else if(usart_port==AB_USART_1)
	{
		for(num = 0 ; num < 50 ; num++)
		{
			xReceiveBuffer1[num] = ' ';
		}
	}
	
	if(usart_port==AB_USART_2)
	{
		for(num = 0 ; num < 50 ; num++)
		{
			xReceiveBuffer2[num] = ' ';
		}
	}
	recieveInc=0;
}


