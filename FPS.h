/***************************************************************************
*
*
* FPS.h
*
* Created by Shubham Gupta 
* Copyright (c) shubhamgupta.org  and/or its affiliates.
* All rights reserved.
***************************************************************************/

#ifndef __FPS_HEADER_H__
#define __FPS_HEADER_H__

#include "usart.h"
#include "pins.h"


//// finger print sensors 

#define SET_AND_SUMADD(idx,val) sendbuf[idx]=((unsigned char)(val));sum += sendbuf[idx]


		#define  CMD_Open  0x01 // 01 Open Initialization 
        #define  CMD_Close  0x02 // 02 Close Termination 
        #define  CMD_UsbInternalCheck  0x03 // 03 UsbInternalCheck Check if the connected USB device is valid 
        #define  CMD_ChangeBaudrate  0x04 // 04 ChangeBaudrate Change UART baud rate 
        #define  CMD_SetIAPMode  0x05 // 05 SetIAPMode Enter IAP Mode In this mode FW Upgrade is available 
        #define  CMD_CmosLed  0x12 // 12 CmosLed Control CMOS LED 
        #define  CMD_GetEnrollCount  0x20 // 20 GetEnrollCount Get enrolled fingerprint count 
        #define  CMD_CheckEnrolled  0x21 // 21 CheckEnrolled Check whether the specified ID is already enrolled 
        #define  CMD_EnrollStart  0x22 // 22 EnrollStart Start an enrollment 
        #define  CMD_Enroll1  0x23 // 23 Enroll1 Make 1st template for an enrollment 
        #define  CMD_Enroll2  0x24 // 24 Enroll2 Make 2nd template for an enrollment 
        #define  CMD_Enroll3  0x25 // 25 Enroll3  Make 3rd template for an enrollment merge three templates into one template save merged template to the database 
        #define  CMD_IsPressFinger  0x26 // 26 IsPressFinger Check if a finger is placed on the sensor 
        #define  CMD_DeleteID  0x40 // 40 DeleteID Delete the fingerprint with the specified ID 
        #define  CMD_DeleteAll  0x41 // 41 DeleteAll Delete all fingerprints from the database 
        #define  CMD_Verify  0x50 // 50 Verify 1:1 Verification of the capture fingerprint image with the specified ID 
        #define  CMD_Identify  0x51 // 51 Identify 1:N Identification of the capture fingerprint image with the database 
        #define  CMD_VerifyTemplate  0x52 // 52 VerifyTemplate 1:1 Verification of a fingerprint template with the specified ID 
        #define  CMD_IdentifyTemplate  0x53 // 53 IdentifyTemplate 1:N Identification of a fingerprint template with the database 
        #define  CMD_CaptureFinger  0x60 // 60 CaptureFinger Capture a fingerprint image(256x256) from the sensor 
        #define  CMD_MakeTemplate  0x61 // 61 MakeTemplate Make template for transmission
        #define  CMD_GetImage  0x62 // 62 GetImage Download the captured fingerprint image(256x256) 
        #define  CMD_GetRawImage  0x63 // 63 GetRawImage Capture & Download raw fingerprint image(320x240) 
        #define  CMD_GetTemplate  0x70 // 70 GetTemplate Download the template of the specified ID 
        #define  CMD_SetTemplate  0x71 // 71 SetTemplate Upload the template of the specified ID 
        #define  CMD_GetDatabaseStart  0x72 // 72 GetDatabaseStart Start database download obsolete 
        #define  CMD_GetDatabaseEnd  0x73 // 73 GetDatabaseEnd End database download obsolete 
        #define  CMD_UpgradeFirmware  0x80 // 80 UpgradeFirmware Not supported 
        #define  CMD_UpgradeISOCDImage  0x81 // 81 UpgradeISOCDImage Not supported 
        #define  CMD_Ack  0x30 // 30 Ack Acknowledge. 
        #define  CMD_Nack  0x31 // 31 Nack Non-acknowledge.
  
        #define NACK_TIMEOUT  0x1001 // NACK_TIMEOUT 0x1001 Obsolete capture timeout 
        #define NACK_INVALID_BAUDRATE  0x1002 // NACK_INVALID_BAUDRATE 0x1002 Obsolete Invalid serial baud rate 
        #define NACK_INVALID_POS  0x1003 // NACK_INVALID_POS 0x1003 The specified ID is not between 0~199 
        #define NACK_IS_NOT_USED  0x1004 // NACK_IS_NOT_USED 0x1004 The specified ID is not used 
        #define NACK_IS_ALREADY_USED  0x1005 // NACK_IS_ALREADY_USED 0x1005 The specified ID is already used 
        #define NACK_COMM_ERR  0x1006 // NACK_COMM_ERR 0x1006 Communication Error 
        #define NACK_VERIFY_FAILED  0x1007 // NACK_VERIFY_FAILED 0x1007 1:1 Verification Failure 
        #define NACK_IDENTIFY_FAILED  0x1008 // NACK_IDENTIFY_FAILED 0x1008 1:N Identification Failure 
        #define NACK_DB_IS_FULL  0x1009 // NACK_DB_IS_FULL 0x1009 The database is full 
        #define NACK_DB_IS_EMPTY  0x100A // NACK_DB_IS_EMPTY 0x100A The database is empty 
        #define NACK_TURN_ERR  0x100B // NACK_TURN_ERR 0x100B Obsolete Invalid order of the enrollment (The order was not as: EnrollStart -> Enroll1 -> Enroll2 -> Enroll3) 
        #define NACK_BAD_FINGER  0x100C // NACK_BAD_FINGER 0x100C Too bad fingerprint 
        #define NACK_ENROLL_FAILED  0x100D // NACK_ENROLL_FAILED 0x100D Enrollment Failure 
        #define NACK_IS_NOT_SUPPORTED  0x100E // NACK_IS_NOT_SUPPORTED 0x100E The specified command is not supported 
        #define NACK_DEV_ERR  0x100F // NACK_DEV_ERR 0x100F Device Error especially if Crypto-Chip is trouble 
        #define NACK_CAPTURE_CANCELED  0x1010 // NACK_CAPTURE_CANCELED 0x1010 Obsolete The capturing is canceled 
        #define NACK_INVALID_PARAM  0x1011 // NACK_INVALID_PARAM 0x1011 Invalid parameter 
        #define NACK_FINGER_IS_NOT_PRESSED  0x1012 // NACK_FINGER_IS_NOT_PRESSED 0x1012 Finger is not pressed 
        #define NACK_IO_ERR  0xF000   // ORIGINAL ERROR CODE. Serial line error.





////////////////////// Finger Print Sensor ////////////////////////////////////////

void AB_STEM_init_FPS(void);
int SendCommand(unsigned long Parameter,unsigned short Command);
void clear_sendbuf(void);
int RecvResponse(unsigned long *Parameter,unsigned short *Response);
int RecvData(unsigned char *data,unsigned long size);
int SendRecv(unsigned short Command,unsigned long *Parameter,unsigned short *Response);
int ClearLine(void);
int Open(void);
int WaitPress(int press);
int CmosLed(int onoff);
int IsPress(void);
int Capture(int best);
int Enroll_N(int N);
int Identify(void);
int Enrol_count(void);
int Verify(int N);
int Enroll(int ID);
int CheckEnrolled(int ID);
int DeleteID(int ID);
int AB_STEM_check_ID(void);
void AB_STEM_ENROLL(int id);











#endif //__FPS_HEADER_H__