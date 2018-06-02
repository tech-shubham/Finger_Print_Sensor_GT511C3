/***************************************************************************
*
*
* FPS.c
*
* Created by Shubham Gupta 
* Copyright (c) shubhamgupta.org  and/or its affiliates.
* All rights reserved.
***************************************************************************/

#include "FPS.h"



////////////////////////////////////// FINGER PRINT ///////////////////////////// 

unsigned char sendbuf[12];
unsigned long LastError;
unsigned long FirmwareVersion; 
unsigned long IsoAreaMaxSize; 
unsigned char DeviceSerialNumber[16];
	
void AB_STEM_init_FPS() 	// fixed USART pin P0 and p1
{
	setDirection(AB_P0,AB_INPUT);
	setDirection(AB_P1,AB_OUTPUT);
	usartInit(AB_USART_2, AB_B_9600, AB_NONE, AB_ONE_BIT, AB_EIGHT_BIT,AB_USART_INTERRUPT_DISABLE);
	Open();
	CmosLed(1);
	
}
int SendCommand(unsigned long Parameter,unsigned short Command)
{
    unsigned short sum = 0;
    int idx = 0;
    int i;
 
    SET_AND_SUMADD(idx,0x55); idx++;
    SET_AND_SUMADD(idx,0xAA); idx++;
    SET_AND_SUMADD(idx,0x01); idx++;
    SET_AND_SUMADD(idx,0x00); idx++;
    SET_AND_SUMADD(idx,Parameter & 0xff); idx++;
    SET_AND_SUMADD(idx,(Parameter >> 8) & 0xff); idx++;
    SET_AND_SUMADD(idx,(Parameter >> 16) & 0xff); idx++;
    SET_AND_SUMADD(idx,(Parameter >> 24) & 0xff); idx++;
    SET_AND_SUMADD(idx,Command & 0xff); idx++;
    SET_AND_SUMADD(idx,(Command >> 8) & 0xff); idx++;
    sendbuf[idx] = sum & 0xff; idx++;
    sendbuf[idx] = (sum >> 8) & 0xff; idx++;
	
	sendUData(AB_USART_2,sendbuf,12);
    return 0;
}

void clear_sendbuf()
{
	for(int i=0;i<12;i++)
	{
		sendbuf[i]=' ';
	}
}

int RecvResponse(unsigned long *Parameter,unsigned short *Response)
{	
    const unsigned char fixedbuf[4] = { 0x55,0xAA,0x01,0x00 };
    unsigned char buf[12];
    unsigned short sum = 0;
    int i;
 
    *Parameter = 0;
    *Response = CMD_Nack;
 
    for(i = 0;i < sizeof(buf);i++)
	{
        while((UCSR2A & (1<<RXC2)) == 0);                   // wait while data is being received
        buf[i] = UDR2;
        if(i < 9)
		{
            sum += buf[i];
        }
        if(i < 4)
		{
            if(buf[i] != fixedbuf[i])
			{
                return -1;
            }
        }
    }
    if(buf[10] != (sum & 0xff))
        return -2;
    if(buf[11] != ((sum >> 8) & 0xff))
        return -2;
 
    *Parameter = (*Parameter << 8) | buf[7];
    *Parameter = (*Parameter << 8) | buf[6];
    *Parameter = (*Parameter << 8) | buf[5];
    *Parameter = (*Parameter << 8) | buf[4];
 
    *Response = buf[9];
    *Response = (*Response << 8) | buf[8];
	
	for(int i=0;i<12;i++)
	{
		//fn_SendCharacter_USART(USART_0, buf[i]);
	}
	
    return 0;
}

int RecvData(unsigned char *data,unsigned long size)
{
    const unsigned char fixedbuf[4] = { 0x5A,0xA5,0x01,0x00 };
    unsigned short sum = 0;
    int i;
 
    for(i = 0;i < size;i++){
        while((UCSR2A & (1<<RXC2)) == 0);                   // wait while data is being received
        *(data + i) = UDR2;
        if(i < (size-2)){
            sum += *(data + i);
        }
        if(i < 4){
            if(*(data + i) != fixedbuf[i]){
                return -1;
            }
        }
    }
    if(*(data + size - 2) != (sum & 0xff))
        return -2;
    if(*(data + size - 1) != ((sum >> 8) & 0xff))
        return -2;
    return 0;
}


int SendRecv(unsigned short Command,unsigned long *Parameter,unsigned short *Response)
{
    int sts;
    if((sts = SendCommand(*Parameter,Command)) == 0){
        *Parameter = 0;
        if((sts = RecvResponse(Parameter,Response)) != 0){
            *Response = CMD_Nack;
            *Parameter = NACK_IO_ERR;
        }
		
		
    }
    if(*Response == CMD_Nack){
        LastError = *Parameter;
    }
    return sts;
}


int ClearLine(void)
{
    while((UCSR2A) & (1<<RXC2));                   // wait while data is being received
    {
        UDR2;
    }
    return 0;
}


int Open(void)
{
    unsigned long Parameter = 1;
    unsigned short Response = 0;
    unsigned char buf[4+sizeof(FirmwareVersion)+sizeof(IsoAreaMaxSize)+sizeof(DeviceSerialNumber)+2];
    int sts = 0;
 
 
    sts = SendRecv(CMD_Open,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack)){
        return -1;
    }

    if((sts = RecvData(buf,sizeof(buf))) == 0){
        memcpy(&FirmwareVersion,&buf[4+0],sizeof(FirmwareVersion));
        memcpy(&IsoAreaMaxSize,&buf[4+sizeof(FirmwareVersion)],sizeof(IsoAreaMaxSize));
        memcpy(DeviceSerialNumber,&buf[4+sizeof(FirmwareVersion)+sizeof(IsoAreaMaxSize)],sizeof(DeviceSerialNumber));
    }
    return sts;
}

int WaitPress(int press)
{
    while(IsPress() != press);
    return 0;
}

int CmosLed(int onoff)
{
    unsigned long Parameter = onoff & 1;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_CmosLed,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack)){
        return -1;
    }
    return 0;
}


int IsPress(void)
{
    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;
    sts = SendRecv(CMD_IsPressFinger,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return 0;
    if(Parameter != 0)
        return 0;
    return 1;
}


int Capture(int best)
{
    unsigned long Parameter = best;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_CaptureFinger,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return -1;
    return 0;
}

int Enroll_N(int N)
{
    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;
	unsigned short cmd;
	
	
    switch(N){
        default:
        case 1: cmd = CMD_Enroll1; break;
        case 2: cmd = CMD_Enroll2; break;
        case 3: cmd = CMD_Enroll3; break;
    }
    sts = SendRecv(cmd,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return -1;
    return 0;
}


int Identify(void)
{
    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_Identify,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return -1;
    return Parameter;
}

int Enrol_count(void)
{
    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_GetEnrollCount,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return -1;
    return Parameter;
}

int Verify(int N)
{

    unsigned long Parameter = N;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_Verify,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return -1;
    return sts;
}

 
int Enroll(int ID)
{
    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;
 
    CmosLed(1);
 
    while(1){
        
        Parameter = ID;
        sts = SendRecv(CMD_EnrollStart,&Parameter,&Response);
        if(sts != 0)
            return sts;
        if(Response != CMD_Ack)
            return -100;
 
        
        CmosLed(0);
        WaitPress(0);
        CmosLed(1);
            
        while(1){
            
            WaitPress(1);
            if(Capture(1) == 0)
                break;
        }
 
        
        CmosLed(0);
        
        if(Enroll_N(1) != 0)
            continue;
        
        WaitPress(0);
        CmosLed(1);
            
        while(1){
          
            WaitPress(1);
            if(Capture(1) == 0)
                break;
        }
 
        CmosLed(0);
        if(Enroll_N(2) != 0)
            continue;
        WaitPress(0);
        CmosLed(1);
        while(1){
            
            CmosLed(1);
            WaitPress(1);
            if(Capture(1) == 0)
                break;
        }
 
        CmosLed(0);
        if(Enroll_N(3) != 0)
            continue;
        WaitPress(0);
 
        break;
    }
    return 0;
}


int CheckEnrolled(int ID)
{
    unsigned long Parameter = ID;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_CheckEnrolled,&Parameter,&Response);
    if((sts == 0) && (Response == CMD_Ack))
        return 0;  //This ID is enrolled 
    return -1;
}


int DeleteID(int ID)
{
    unsigned long Parameter = ID;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_DeleteID,&Parameter,&Response);
    if((sts == 0) && (Response == CMD_Ack))
        return 0;
    return -1;
}

int AB_STEM_check_ID()
{
	int temp;
	if( IsPress() == 1)
	{
		Capture(0);
		temp = Identify();
		return temp;
	}
	return -2;
}
void AB_STEM_ENROLL(int id)
{
	DeleteID(id);
	Enroll(id);
}





int main()
{
	int fpsID=0;
	AB_STEM_init_FPS();
	
	AB_STEM_ENROLL(10);		// Put ID in arguement which you want to enroll
	
	while(1)
	{
		fpsID=AB_STEM_check_ID()	;	///// Returns you the ID of the finger Placed
	}

		return 0;
}

