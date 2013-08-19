#include "modbus_rtu_crc16.h"

//The same as CRC calculation in Modbus RTU: 
//http://www.protocessor.com/techsupport/Free_Modbus_RTU_Source_Code.asp

//Thanks to Max Power: http://www.control.com/thread/1026204601

#define SEED 0xFFFF  //initialization for CRC16
#define GP   0xA001  //generating polynomial

//for standard CRC16
//(remainder of division)
//to start a new CRC, set CRC16 = SEED
//then for each byte call Calc_CRC(byte, &CRC16);
//CRC16 will contain the result
//(if you calculate all of the incoming data
//INCLUDING the CRC, the result should be 0x0000
//and if you are sending the CRC be sure to
//send the bytes in the correct order)

void Calc_CRC(unsigned char b8, unsigned int* CRC)
{
	unsigned int carry;
	int i;

	CRC[0] ^= b8 & 0xFF;
	for (i=0; i<8; i++)
	{
		carry = CRC[0] & 0x0001;
		CRC[0]>>=1;
		if (carry) CRC[0] ^= GP;
	}
}

/* return: CRC16 code of stream */
unsigned int modbus_rtu_crc16(unsigned char* encoded_message,int length)
{
	unsigned char *ptr = encoded_message;
	unsigned int CRC16;
	int i;

	CRC16 = SEED;
	for(i = 0; i < length; i++)
	{
		Calc_CRC(ptr[i], &CRC16);
	}

	return(CRC16);
}
