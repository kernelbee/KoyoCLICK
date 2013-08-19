#include "clickplc_exe_codes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//registers encoding:   2 bytes{REGTYPE,index}
//BITS
#define EXE_CODE_REGTYPE_BOOL_X				0xc000	//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);
								//1-16:		0xc000-0xc00f;
								//101-116:	0xc020-0xc02f;
								//201-216:	0xc040-0xc04f;
								//301-316:	0xc060-0xc06f;
								//401-416:	0xc080-0xc08f;
								//501-516:	0xc0a0-0xc0af;
								//601-616:	0xc0c0-0xc0cf;
								//701-716:	0xc0e0-0xc0ef;
								//801-816:	0xc100-0xc10f;

#define EXE_CODE_REGTYPE_BOOL_Y				0xc400	//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);
								//1-16:		0xc400-0xc40f;
								//101-116:	0xc420-0xc42f;
								//201-216:	0xc440-0xc44f;
								//301-316:	0xc460-0xc46f;
								//401-416:	0xc480-0xc48f;
								//501-516:	0xc4a0-0xc4af;
								//601-616:	0xc4c0-0xc4cf;
								//701-716:	0xc4e0-0xc4ef;
								//801-816:	0xc500-0xc50f;

//b.addr. last addr.																b.addr.
#define EXE_CODE_REGTYPE_BOOL_T				0xc800	//...0xc9f3	index = (number - 1); number: 1...500, so totally: 500;			1100100000000000
#define EXE_CODE_REGTYPE_BOOL_CT			0xcc00	//...0xccf9	index = (number - 1); number: 1...250, so totally: 250;			1100110000000000		
#define EXE_CODE_REGTYPE_BOOL_C				0xd000	//...0xd7cf	index = (number - 1); number: 1...2000, so totally: 2000;		1101000000000000
#define EXE_CODE_REGTYPE_BOOL_SC			0xf000	//...0xf3e8	index = (number - 1); number: 1...1000, so totally: 1000;		1111000000000000
//DATA
#define EXE_CODE_REGTYPE_INT16_DS			0x0000	//...0x1193	index = (number - 1); number: 1...4500, so totally: 4500;		0000000000000000
#define EXE_CODE_REGTYPE_INT32_DD			0x1194	//...0x1962	index = (number - 1)x2; number: 1...1000, so totally: 1000;		0001000110010100
#define EXE_CODE_REGTYPE_HEX16_DH			0x1964	//...0x1b57	index = (number - 1); number: 1...500, so totally: 500;			0001100101100100
#define EXE_CODE_REGTYPE_FLT32_DF			0x1b58	//...0x1f3e	index = (number - 1)x2; number: 1...500, so totally: 500;		0001101101011000
#define EXE_CODE_REGTYPE_INT16_SD			0x5000	//...0x53e7	index = (number - 1); number: 1...1000, so totally: 1000;		0101000000000000
#define EXE_CODE_REGTYPE_INT16_TD			0x6000	//...0x61f3	index = (number - 1); number: 1...500, so totally: 500;			0110000000000000		
#define EXE_CODE_REGTYPE_INT32_CTD			0x6800	//...0x69f2	index = (number - 1)x2; number: 1...250, so totally: 250;		0110100000000000
#define EXE_CODE_REGTYPE_ASCII_TXT			0x7000	//...0x73e7	index = (number - 1); number: 1...1000, so totally: 1000;		0111000000000000
#define EXE_CODE_REGTYPE_HEX16_XD			0x7c00	//...0x7c08	index==number: 0...8, so totally: 9 regs;						0111110000000000
#define EXE_CODE_REGTYPE_HEX16_YD			0x7c40	//...0x7c48	index==number: 0...8, so totally: 9 regs;						0111110001000000

#define EXE_CODE_REGTYPE_DUMMY				0xffff	
#define EXE_CODE_REGTYPE_DUMMY2				0xaaaa	//see Drum


/*

Range encoding:
______________________________ 
2 bytes{first register},
2 bytes{0xff, 0xff},
2 bytes{last register}
______________________________

*/

/*		
	return: encoded size, ok=2, error=0.
*/
int clickplc_int16_enc(	unsigned short number,unsigned char* stream2bytes)
{
	int retval = 2;
	stream2bytes[0] = (unsigned char)(number>>8);
	stream2bytes[1] = (unsigned char)(number&0x00ff);
	return(retval);
}
int clickplc_int16_dec(	unsigned char* stream2bytes,unsigned short* pnumber)
{
	int retval = 2;
	*pnumber  = ((unsigned short)stream2bytes[0]<<8)|(unsigned short)stream2bytes[1];	
	return(retval);
}
/*		
	return: encoded size, ok=4, error=0.
*/
int clickplc_int32_enc(	unsigned int number,			//IN
					   unsigned char* stream4bytes		//OUT
					   )
{
	int retval = 4;
	stream4bytes[0] = (unsigned char)( number>>24);
	stream4bytes[1] = (unsigned char)((number>>16)&0x000000ff);
	stream4bytes[2] = (unsigned char)((number>>8 )&0x000000ff);
	stream4bytes[3] = (unsigned char)( number     &0x000000ff);
	return(retval);
}

int clickplc_int32_dec(	unsigned char* stream4bytes,	//IN
					   unsigned int* pnumber			//OUT
					   )
{
	int retval = 4;
	*pnumber  =	 ((unsigned int)stream4bytes[0]<<24)
				|((unsigned int)stream4bytes[1]<<16)
				|((unsigned int)stream4bytes[2]<<8)
				| (unsigned int)stream4bytes[3];
	return(retval);
}

/*		
	return: ok=1, error=0.
*/
int clickplc_reg_check(	EXE_Register* reg  )
{
	int retval = 1;//OK by default
	
	if(		(reg->reg_id >= CLICK_EXE_Register_X) 
		&&	(reg->reg_id <= CLICK_EXE_Register_YD))
	{
		CLICK_EXE_RegMetrics *limits;
		
		limits = &CLICK_EXE_RegDataBase[reg->reg_id];
		
		if (   (reg->number >= limits->n_min) 
			&& (reg->number <= limits->n_max) )
		{			
			switch (reg->reg_id)
			{
			case CLICK_EXE_Register_X:
				//X
				//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);
				//1-16:		0xc000-0xc00f;
				//101-116:	0xc020-0xc02f;
				//201-216:	0xc040-0xc04f;
				//301-316:	0xc060-0xc06f;
				//401-416:	0xc080-0xc08f;
				//501-516:	0xc0a0-0xc0af;
				//601-616:	0xc0c0-0xc0cf;
				//701-716:	0xc0e0-0xc0ef;
				//801-816:	0xc100-0xc10f;
			case CLICK_EXE_Register_Y:
				//Y
				//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);
				//1-16:		0xc400-0xc40f;
				//101-116:	0xc420-0xc42f;
				//201-216:	0xc440-0xc44f;
				//301-316:	0xc460-0xc46f;
				//401-416:	0xc480-0xc48f;
				//501-516:	0xc4a0-0xc4af;
				//601-616:	0xc4c0-0xc4cf;
				//701-716:	0xc4e0-0xc4ef;
				//801-816:	0xc500-0xc50f;
				
				if ( (reg->number % 100) > 16 )
				{
					retval = 0;//wrong number for X,Y
				}
				//else OK!
				break;
				
			default:
				break;//OK!
			}
		}
		else
		{
			retval = 0;//number is out of range
		}
	}
	else
	{
		retval = 0;	//wrong ID
	}	
	return(retval);
}

/*		
	return: 2 (= encoded size of register)
*/
int clickplc_reg_enc(	EXE_Register* reg,				//IN
						unsigned char* stream2bytes		//OUT
						)
{	
	unsigned int	index;
	unsigned short	encreg;
	
	switch (reg->reg_id)
	{
	case CLICK_EXE_Register_X:	//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);		
	case CLICK_EXE_Register_Y:	//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);
		//index = (reg->number%100) - 1 + (reg->number/100)*0x0020;
		index = (reg->number%100) - 1 + ((reg->number/100)<<5);
		break;
		
	case CLICK_EXE_Register_T:	//index = (number - 1);
	case CLICK_EXE_Register_CT:	//index = (number - 1);
	case CLICK_EXE_Register_C:	//index = (number - 1);
	case CLICK_EXE_Register_SC:	//index = (number - 1);
	case CLICK_EXE_Register_DS:	//index = (number - 1);	
	case CLICK_EXE_Register_DH: //index = (number - 1);
	case CLICK_EXE_Register_SD: //index = (number - 1);
	case CLICK_EXE_Register_TD: //index = (number - 1);	
	case CLICK_EXE_Register_TXT://index = (number - 1);
		index = reg->number - 1;
		break;
		
	case CLICK_EXE_Register_DD:	//index = (number - 1)*2;
	case CLICK_EXE_Register_DF: //index = (number - 1)*2;
	case CLICK_EXE_Register_CTD://index = (number - 1)*2;
		//index = (reg->number - 1)*2;
		index = (reg->number - 1)<<1;
		break;
		
	case CLICK_EXE_Register_XD: //index = (number - 1);
	case CLICK_EXE_Register_YD: //index = (number - 1);
		index = reg->number;
		break;		
	}
	
	index += CLICK_EXE_RegDataBase[reg->reg_id].baddr;
	encreg = (unsigned short)index;
	
	stream2bytes[0] = (unsigned char)((encreg&0xff00)>>8);
	stream2bytes[1] = (unsigned char)(encreg&0xff);
	
	return(2);
}

/*		
return: 2 (= encoded size of register)
*/
int clickplc_reg_dec(	unsigned char* stream2bytes,	//IN
						EXE_Register* reg				//OUT
						)
{
	unsigned int encreg;
	int reg_id;
	int is_found;
	unsigned int baddr;	
	int retval = 2;//OK by default
		
	encreg = ((unsigned int)stream2bytes[0]<<8)|(unsigned int)stream2bytes[1];
	
	is_found = 0;
	
	/*
	TODO: 

	1. sort registers by baddr to make array CLICK_EXE_RegDataBase[reg_id] sorted.
	2. change to binary search!!!
	
	*/
	for(reg_id = CLICK_EXE_Register_X; reg_id <= CLICK_EXE_Register_YD; reg_id++)
	{
		unsigned int maxenc;
		
		baddr = CLICK_EXE_RegDataBase[reg_id].baddr;
		maxenc= CLICK_EXE_RegDataBase[reg_id].enc_max;
		
		if (encreg >= baddr)
		{
			if (encreg <= maxenc)
			{
				is_found = 1;
				break;
			}
		}
	}
	
	if (is_found)
	{
		int number;
		int index;	
		
		index = encreg - baddr;
		
		switch (reg_id)
		{
		case CLICK_EXE_Register_X:	//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);		
		case CLICK_EXE_Register_Y:	//index = (number%100) - 1 + (number/100)*0x0020; totally: 16x9 = 144 (bits);
			
			index += 1;
			//number = 100*( index / 0x0020 ) + (index % 0x0020);
			number = 100*( index >> 5  ) + (index & 0x0000001f);
			break;
			
		case CLICK_EXE_Register_T:	//index = (number - 1);
		case CLICK_EXE_Register_CT:	//index = (number - 1);
		case CLICK_EXE_Register_C:	//index = (number - 1);
		case CLICK_EXE_Register_SC:	//index = (number - 1);
		case CLICK_EXE_Register_DS:	//index = (number - 1);	
		case CLICK_EXE_Register_DH: //index = (number - 1);
		case CLICK_EXE_Register_SD: //index = (number - 1);
		case CLICK_EXE_Register_TD: //index = (number - 1);	
		case CLICK_EXE_Register_TXT://index = (number - 1);
			
			number = index + 1;
			break;
			
		case CLICK_EXE_Register_DD:	//index = (number - 1)*2;
		case CLICK_EXE_Register_DF: //index = (number - 1)*2;
		case CLICK_EXE_Register_CTD://index = (number - 1)*2;
			if ( !(index&0x1) )
			{
				//number = (index/2) + 1;
				number = (index>>1) + 1;
			}
			else
			{
				retval = 0;//wrong value
				goto __exit_point;
			}
			break;
			
		case CLICK_EXE_Register_XD: //index = (number - 1);
		case CLICK_EXE_Register_YD: //index = (number - 1);
			number = index;
			break;		
		}
		//result
		reg->reg_id = reg_id;
		reg->number = number;
	}
	else
	{
		retval = 0;//wrong value
	}
__exit_point:		
	return(retval);
}
//return: encoded size of register, error=0.	
int clickplc_reg_pascal_enc(EXE_Register* reg,unsigned char* pascal_string)
{
	int encsize;
	int strsize,i;
	char text[CKP_REG_TEXT_MAX_SIZE];
		
	strsize = sprintf(text,CLICK_EXE_RegDataBase[reg->reg_id].txtformat,CLICK_EXE_RegDataBase[reg->reg_id].txtID,reg->number);	
	
	encsize = strsize*2+1;
	pascal_string[0] = encsize;
	for (i = 0; i < strsize; i++)
	{
		pascal_string[1+2*i+0] = text[i];
		pascal_string[1+2*i+1] = 0x00;
	}
	pascal_string[encsize++] = 0x00;
	
	return(encsize);
}

/*
{0xc000,			1,		816,	0xc10f,	"X"},		//0	X
{0xc400,			1,		816,	0xc50f,	"Y"},		//1	Y
{0xc800,			1,		500,	0xc9f3,	"T"},		//2 T
{0xcc00,			1,		250,	0xccf9,	"CT"},		//3 CT
{0xd000,			1,		2000,	0xd7cf,	"C"},		//4 C
{0xf000,			1,		1000,	0xf3e7,	"SC"},		//5 SC
{0x0000,			1,		4500,	0x1193,	"DS"},		//6 DS
{0x1194,			1,		1000,	0x1962,	"DD"},		//7 DD
{0x1964,			1,		500,	0x1b57,	"DH"},		//8 DH
{0x1b58,			1,		500,	0x1f3e,	"DF"},		//9 DF
{0x5000,			1,		1000,	0x53e7,	"SD"},		//10 SD
{0x6000,			1,		500,	0x61f3,	"TD"},		//11 TD
{0x6800,			1,		250,	0x69f2,	"CTD"},		//12 CTD
{0x7000,			1,		1000,	0x73e7,	"TXT"},		//13 TXT
{0x7c00,			0,		8,		0x7c08,	"XD"},		//14 XD
{0x7c40,			0,		8,		0x7c48,	"YD"}		//15 YD	
*/

int clickplc_reg_pascal_dec(unsigned char* pascal_string,EXE_Register* reg)
{
	int encsize;
	int strsize,i;
	char text[CKP_REG_TEXT_MAX_SIZE];

	strsize = (pascal_string[0] - 1)/2;
	for (i = 0; i < strsize; i++)
	{
		text[i] = pascal_string[1+2*i+0];
	}
	text[i] = 0;

	if(clickplc_reg_cstr_dec(text,reg)==1)
	{		
		encsize = pascal_string[0] + 1;
	}
	else
	{
		encsize = 0;
	}

	return(encsize);
}

int clickplc_regid_cstr_dec(char* cstr_string,int* preg_id)
{
	int retval = 1;//ok by default
	char* text = cstr_string;	
	int reg_id = -1;
	
	switch(text[0])
	{
	case 'X':
		if (text[1]!='D')
		{
			reg_id = CLICK_EXE_Register_X;
		}
		else
		{
			reg_id = CLICK_EXE_Register_XD;			
		}
		break;

	case 'Y':
		if (text[1]!='D')
		{
			reg_id = CLICK_EXE_Register_Y;
		}
		else
		{
			reg_id = CLICK_EXE_Register_YD;			
		}
		break;

	case 'T':
		if (text[1]!='D')
		{
			if (text[1]!='X')
			{
				reg_id = CLICK_EXE_Register_T;
			}
			else
			{				
				if (text[2]=='T')
				{
					reg_id = CLICK_EXE_Register_TXT;					
				}				
				else
				{
					retval = 0;
					goto __exit_point;
				}
			}			
		}
		else
		{
			reg_id = CLICK_EXE_Register_TD;			
		}
		break;

	case 'C':
		if (text[1]!='T')
		{
			reg_id = CLICK_EXE_Register_C;
		}
		else
		{
			if (text[2]!='D')
			{
				reg_id = CLICK_EXE_Register_CT;				
			}
			else
			{
				reg_id = CLICK_EXE_Register_CTD;				
			}			
		}
		break;

	case 'S':
		if (text[1]=='C')
		{
			reg_id = CLICK_EXE_Register_SC;			
		}
		else
		{
			if (text[1]=='D')
			{
				reg_id = CLICK_EXE_Register_SD;				
			}
			else
			{
				retval = 0;
				goto __exit_point;
			}			
		}
		break;

	case 'D':		
		switch (text[1])
		{
		case 'S':
			reg_id = CLICK_EXE_Register_DS;
			break;
		case 'D':
			reg_id = CLICK_EXE_Register_DD;
			break;
		case 'H':
			reg_id = CLICK_EXE_Register_DH;
			break;
		case 'F':
			reg_id = CLICK_EXE_Register_DF;
			break;
		default:
			printf("clickplc_regid_cstr_dec(): Unknown REG D %02x\n",text[1]);
			retval = 0;
			goto __exit_point;
		}
		break;
	default:
		printf("clickplc_regid_cstr_dec(): Unknown REG %02x\n",text[0]);
		retval = 0;
		goto __exit_point;
	}
	if(preg_id) *preg_id = reg_id;
__exit_point:
	return(retval);
}

int clickplc_reg_cstr_dec(char* cstr_string,EXE_Register* reg)
{
	int retval = 0;//NG by default
	char* text = cstr_string;
	int reg_id = -1;

	if (clickplc_regid_cstr_dec(text,&reg_id))
	{
		int number_offset;		
		int number = 0;

		number_offset = CLICK_EXE_RegDataBase[reg_id].txtsize;

		if (sscanf(text+number_offset,"%d",&number)!=1)
		{
			printf("Can't decode REG-number from: %s\n",text+number_offset);			
		}
		else
		{
			reg->reg_id = reg_id;
			reg->number = number;
			retval = 1;
		}
	}

	return(retval);
}

int clickplc_regbyptr_cstr_dec(char* cstr_string,EXE_Register* reg,EXE_Register* ptrreg)
{
	int retval = 0;//NG by default
	char* text = cstr_string;
	int reg_id = -1;

	if (clickplc_regid_cstr_dec(text,&reg_id))
	{
		int ptr_offset;		
		int number = 0;

		ptr_offset = CLICK_EXE_RegDataBase[reg_id].txtsize;

		if (text[ptr_offset] == '[')
		{
			ptr_offset++;
			
			if(clickplc_reg_cstr_dec(text+ptr_offset,ptrreg))
			{
				reg->reg_id = reg_id;
				reg->number = 0;

				retval = 1;
			}
		}
	}

	return(retval);
}

//return: encoded size, error=0.	
int clickplc_regbyptr_pascal_enc(EXE_Register* reg,EXE_Register* ptrreg,unsigned char* pascal_string)
{
	int encsize;
	int strsize,i;
	int strsize_t;
	char text[2*CKP_REG_TEXT_MAX_SIZE+1];
	char textptr[CKP_REG_TEXT_MAX_SIZE+1];
		
	strsize_t = sprintf(textptr,CLICK_EXE_RegDataBase[ptrreg->reg_id].txtformat,
						CLICK_EXE_RegDataBase[ptrreg->reg_id].txtID,
						ptrreg->number);

	strsize = sprintf(	text,"%s[%s]",
						CLICK_EXE_RegDataBase[reg->reg_id].txtID,textptr);

	encsize = strsize*2+1;
	pascal_string[0] = encsize;
	for (i = 0; i < strsize; i++)
	{
		pascal_string[1+2*i+0] = text[i];
		pascal_string[1+2*i+1] = 0x00;
	}
	pascal_string[encsize++] = 0x00;

	return(encsize);
}

/*

#########################################################
Timer:

An ON Delay Timer measures the time duration that begins with a transition of the enable rung from OFF to ON. 
Beyond this transition point, the Timer increases the Current Value, when it reaches the Set Point, the Timer Bit is turned ON.

An OFF Delay Timer measures the time duration that begins with a transition of the enable rung from ON to OFF. 
Beyond this transition point, the Timer increases the Current Value, when it reaches the Set Point, the Timer Bit is turned OFF.

1 byte{0x05}
1 byte{time unit|flags}
2 bytes{timer register}
2 bytes{0xff,0xff}
2 bytes{time in units}
____________________
=8 bytes

05 21 c8 00 ff ff 00 01		day 	ON Delay	Not Retained	0010 0001	day = 0010 0001 = 0x21

01 00 d0 00 reset
05 1d c8 01 ff ff 00 01 	hour	ON Delay	Retained		0001 1101	hour= 0001 1001 = 0x19

05 13 c8 02 ff ff 00 01 	min	OFF Delay	Not Retained		0001 0011	min = 0001 0001	= 0x11		OFF Delay = 0000 0010 = 0x02

05 09 c8 03 ff ff 00 01		sec 	ON Delay	Not Retained	0000 1001	sec = 0000 1001 = 0x09

05 03 c8 04 ff ff 00 01 	ms	OFF Delay	Not Retained		0000 0011	ms  = 0000 0001 = 0x01

01 00 d0 00 reset
05 0d c8 05 ff ff 00 01 	sec	ON Delay	Retained			0000 1101	sec = 0000 1001			Retained = 0000 0100 = 0x04

01 02 c0 01 enable
01 00 c0 00 reset
05 05 c8 06 ff ff 00 64 	ms	ON Delay	Retained			0000 0101	

01 02 c0 02 enable
05 01 c8 07 ff ff 00 10 	ms	ON Delay	Not Retained		0000 0001	

01 03 c0 02 enable
05 03 c8 08 ff ff 00 64 	ms	OFF Delay	Not Retained		0000 0011	

*/

/*		
return: encoded size !=0, or error=0.
*/	
int clickplc_exe_Timer_enc(void* obj,unsigned char* stream)
{
	int encsize = 8;
	EXE_Timer *timer = (EXE_Timer*)obj;
	
	stream[0] = EXE_CODE_INSTRUCTION_TIMER;
	stream[1] = timer->units|timer->flags|timer->is_sp_const_number;
	clickplc_reg_enc(&timer->timer_bit_ref,stream+2);
	stream[4] = 0xff;
	stream[5] = 0xff;

	if (timer->is_sp_const_number == 0)
	{
		clickplc_reg_enc(&timer->sp_reg_ref,stream+6);
	}
	else
	{
		clickplc_int16_enc((unsigned short)timer->sp_const_number,stream+6);
	}	
	
	return(encsize);
}

int clickplc_exe_Timer_dec(unsigned char* stream,void* obj)
{
	int encsize = 8;
	EXE_Timer *timer = (EXE_Timer*)obj;
	
	timer->units				= stream[1]&TIMER_UNITS_MASK;
	timer->flags				= stream[1]&(TIMER_FLAGS_RETAINED|TIMER_FLAGS_OFFDELAY);
	timer->is_sp_const_number	= stream[1]&TIMER_SPTYPE_MASK;
	
	clickplc_reg_dec(stream+2,&timer->timer_bit_ref);

	if (timer->is_sp_const_number == 0)
	{
		clickplc_reg_dec(stream+6,&timer->sp_reg_ref);
	}
	else
	{
		clickplc_int16_dec(stream+6,(unsigned short*)&timer->sp_const_number);
	}
	
	return(encsize);
}


/*
##################################################
Counter:

When enabled, a Counter instruction counts up or down (depending on user settings) until it reaches the Set Point. 
The Counter counts in response to the transition from OFF to ON of the enabling rung. 
If the user selects Count Up & Down, the Counter will have two enabling rungs, one for up counts and one for down counts.

The current count is held in the register shown in the Current Value field. 
When the Current Value reaches the Set Point, the Completion Bit is turned ON. 
The Completion Bit is turned OFF when the Reset rung is enabled.

1 byte{0x06}
1 byte{type: up 0x00, down 0x02, up&down 0x04}
2 bytes{complete bit register}
2 bytes{0xff,0xff}
2 bytes{set point int16/32 register}

Total = 8 bytes.

Examples:

01 00 d0 01 up
01 00 d0 00 reset
06 00 cc 00 ff ff 00 00	up

01 00 d0 02 down
01 00 d0 00 reset
06 02 cc 01 ff ff 00 00	down

01 00 d0 00 up
01 00 d0 01 down
01 00 d0 02 reset
06 04 cc 02 ff ff 00 00	up&down

01 00 d0 01 up
01 00 d0 00 reset
06 00 cc 03 ff ff 11 94	up

01 00 d0 00 down
01 00 d0 01 reset
06 02 cc 04 ff ff 11 94	down

01 00 d0 00 up
01 00 d0 01 down
01 00 d0 02 reset
06 04 cc 05 ff ff 11 94	up&down


COUNTER	06 01 cc 00 00 00 30 39		up CT1, sp=12345, 16-bit
COUNTER	06 03 cc 01 00 01 e2 40		down CT2, sp=123456, 32-bit
COUNTER	06 04 cc 02 ff ff 00 00 
COUNTER	06 00 cc 03 ff ff 11 94 
COUNTER	06 02 cc 04 ff ff 11 94 

*/

#define COUNTER_COUNT_SP_CONST		0x01
/*		
	return: encoded size !=0, or error=0.
*/
int clickplc_exe_Counter_enc(void* obj,unsigned char* stream)
{
	int encsize = 8;
	EXE_Counter* counter = (EXE_Counter*)obj;
	
	stream[0] = EXE_CODE_INSTRUCTION_COUNTER;
	stream[1] = counter->count_type|counter->is_sp_const_number;

	clickplc_reg_enc(&counter->out_complete_bit_ref,	&stream[2]);//2,3

	if (counter->is_sp_const_number)
	{
		clickplc_int32_enc((unsigned int)counter->sp_const_number,	&stream[4]);//4,5,6,7
	}
	else
	{
		stream[4] = 0xff;
		stream[5] = 0xff;
		clickplc_reg_enc(&counter->sp_reg_ref,			&stream[6]);//6,7
	}
	
	return(encsize);
}

int clickplc_exe_Counter_dec(unsigned char* stream,void* obj)
{
	int encsize = 8;
	EXE_Counter* counter = (EXE_Counter*)obj;
	
	counter->count_type			= stream[1]&0xfe;
	counter->is_sp_const_number = stream[1]&0x01;

	clickplc_reg_dec(stream+2,&counter->out_complete_bit_ref);
	
	if (counter->is_sp_const_number)
	{
		clickplc_int32_dec(&stream[4],(unsigned int*)&counter->sp_const_number);//4,5,6,7
	}
	else
	{
		clickplc_reg_dec(&stream[6],&counter->sp_reg_ref);//6,7
	}
	
	return(encsize);
}


/*

#################################################
Shift register:

The Shift Register instruction Shifts a range of control bits with each OFF-to-ON transition of  the Clock pulse. 
If the Starting Address is lower than the Ending Address, the Shift register will Shift from the Starting Address to the Ending Address.  
If the Ending Address is lower than Starting Address then Shift Register will Shift from Ending Address to the Starting Address.


1 byte{0x04}
1 byte{0x30}
2 bytes{start bit register, C-regs only}
2 bytes{0xff,0xff}
2 bytes{end bit register}

Total = 8 bytes

01 04 c0 00 	Data
01 04 c0 01 	Clock
01 04 c0 02 	Reset

04 30 d0 00 ff ff d0 c8 	c1 - c201: d0c8 = d0|c8(200)

01 01 c0 00		Data
01 01 c0 01		Clock
01 01 c0 02		Reset

04 30 d0 01 ff ff d2 bb 	c2 - c700: d2bb = d0|2bb(699)

*/

/*		
return: encoded size !=0, or error=0.
*/
int clickplc_exe_ShiftReg_enc(void* obj,unsigned char* stream)
{
	int encsize = 8;
	EXE_ShiftRegister* shiftreg = (EXE_ShiftRegister*)obj;
	stream[0] = EXE_CODE_INSTRUCTION_COIL;
	stream[1] = EXE_CODE_INSTRUCTION_SHIFTREG_SECONDBYTE;
	clickplc_reg_enc(&shiftreg->start_ref,	&stream[2]);//2,3
	stream[4] = 0xff;
	stream[5] = 0xff;
	clickplc_reg_enc(&shiftreg->end_ref,	&stream[6]);//6,7
	return(encsize);
}

int clickplc_exe_ShiftReg_dec(unsigned char* stream,void* obj)
{
	int encsize = 8;
	EXE_ShiftRegister* shiftreg = (EXE_ShiftRegister*)obj;
	clickplc_reg_dec(&stream[2],&shiftreg->start_ref);	//2,3
	clickplc_reg_dec(&stream[6],&shiftreg->end_ref);	//6/7
	return(encsize);
}

/*
FOR loop:

for const int16 = 12345
###	00 20 00 03 00 00 30 39 
###	00 21 00 03 00 00 30 39 	1s

for const int32 = 123456
###	00 20 00 03 00 01 e2 40 
###	00 21 00 03 00 01 e2 40 	1s

for var int16 = ds100
###	00 20 00 01 ff ff 00 64 
###	00 21 00 01 ff ff 00 64 	1s

for var int32 = dd1
###	00 20 00 01 ff ff 11 94 
###	00 21 00 01 ff ff 11 94 	1s
*/

/*		
	return: encoded size !=0, or error=0.
*/	
#define EXE_CODE_CNTRL_FOR_LOOP_CONST		0x03
#define EXE_CODE_CNTRL_FOR_LOOP_VARIABLE	0x01

int clickplc_exe_For_enc(void* obj,unsigned char* stream)
{
	int encsize = 8;
	EXE_For* loopfor = (EXE_For*)obj;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_FOR_LOOP|loopfor->is_one_shot;
	stream[2] = 0x00;
	if (loopfor->is_variable==0)
	{
		stream[3] = EXE_CODE_CNTRL_FOR_LOOP_CONST;
		clickplc_int32_enc(loopfor->const_cycles,stream+4);
	}
	else
	{
		stream[3] = EXE_CODE_CNTRL_FOR_LOOP_VARIABLE;
		stream[4] = 0xff;
		stream[5] = 0xff;
		clickplc_reg_enc(&loopfor->variable_cycles_ref,	stream+6);
	}	
	return(encsize);
}

int clickplc_exe_For_dec(unsigned char* stream,void* obj)
{
	int encsize = 8;
	EXE_For* loopfor = (EXE_For*)obj;
	loopfor->is_one_shot = stream[1]&0x01;
	if (stream[3] == EXE_CODE_CNTRL_FOR_LOOP_CONST)
	{
		loopfor->is_variable = 0;
		clickplc_int32_dec(stream+4,&loopfor->const_cycles);
	}
	else
	{//EXE_CODE_CNTRL_FOR_LOOP_VARIABLE		
		loopfor->is_variable = 1;
		clickplc_reg_dec  (stream+6,&loopfor->variable_cycles_ref);
	}	
	return(encsize);
}

//00 22 0xff 0xff
int clickplc_exe_Next_enc				(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_NEXT;
	stream[2] = 0xff;
	stream[3] = 0xff;
	return(encsize);
}
int clickplc_exe_Next_dec				(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}

//0x00,0x80,0xff,0xff
int clickplc_exe_EndOfMain_enc			(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_MAIN_END;
	stream[2] = 0xff;
	stream[3] = 0xff;	
	return(encsize);
}
int clickplc_exe_EndOfMain_dec			(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}

//0x00,0x81,0xff,0xff
int clickplc_exe_EndOfSubProgramm_enc				(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_SUBPROGRAMM_END;
	stream[2] = 0xff;
	stream[3] = 0xff;
	return(encsize);
}
int clickplc_exe_EndOfSubProgramm_dec				(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}
//0x00,0x82,0xff,0xff
int clickplc_exe_EndOfInterrupt_enc	(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_INTERRUPT_END;
	stream[2] = 0xff;
	stream[3] = 0xff;
	return(encsize);
}
int clickplc_exe_EndOfInterrupt_dec	(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}

//0x00,0x10,0xNN,0xNN	- subprogram #N call
/*		
	return: encoded size !=0, or error=0.	
*/	
int clickplc_exe_Call_enc(void* obj,unsigned char* stream)
{
	int encsize = 4;
	EXE_Call* call = (EXE_Call*)obj;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_SUBPROGRAMM_CALL;
	clickplc_int16_enc(call->subprogramm_no,stream+2);
	return(encsize);
}

int clickplc_exe_Call_dec(unsigned char* stream,void* obj)
{
	int encsize = 4;
	EXE_Call* call = (EXE_Call*)obj;
	clickplc_int16_dec(stream+2,&call->subprogramm_no);
	return(encsize);
}

//0x00,0x11,0xNN,0xNN	- subprogram #N start
/*		
	return: encoded size !=0, or error=0.
*/	
int clickplc_exe_SubProgrammStart_enc(void* obj,unsigned char* stream)
{
	int encsize = 4;
	EXE_SubProgrammStart* start = (EXE_SubProgrammStart*)obj;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_SUBPROGRAMM_START;
	clickplc_int16_enc(start->subprogramm_no,stream+2);
	return(encsize);
}

int clickplc_exe_SubProgrammStart_dec(unsigned char* stream,void* obj)
{
	int encsize = 4;
	EXE_SubProgrammStart* start = (EXE_SubProgrammStart*)obj;
	clickplc_int16_dec(stream+2,&start->subprogramm_no);
	return(encsize);
}

#define INTERRUPT_BASE_NUMBER 0x03db

int clickplc_exe_InterruptStart_enc(void* obj,unsigned char* stream)
{
	int encsize = 4;
	EXE_InterruptStart* start = (EXE_InterruptStart*)obj;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_INTERRUPT_START;
	clickplc_int16_enc(INTERRUPT_BASE_NUMBER+start->interrupt_no,stream+2);
	return(encsize);
}

int clickplc_exe_InterruptStart_dec(unsigned char* stream,void* obj)
{
	int encsize = 4;
	EXE_InterruptStart* start = (EXE_InterruptStart*)obj;
	clickplc_int16_dec(stream+2,&start->interrupt_no);
	start->interrupt_no -= INTERRUPT_BASE_NUMBER; 
	return(encsize);
}

/*
#########################################################
MATH

Decimal
The Math instruction solves a user-defined formula during the execution of the Ladder Program. 
The formula is developed on the Math dialog using the on-screen keypad, the computer keyboard, and Address Picker. 
Two sets of mathematical operators are available. 
One set is appropriate for use with decimal values, and the other is for use with hexadecimal values. 
Also see Math (Hex). Parenthetical expressions can be nested up to eight levels deep. 
If the Floating Point Data Type is used in any operation, then all operations will be based on Floating Point math. 
The solution will be stored in the data format selected for the Result.

Hexadecimal
The Math instruction solves a user-defined formula during the execution of the Ladder Program. 
The formula is developed on the Math dialog using the onscreen keypad, the computer keyboard, and Address Picker. 
Two sets of mathematical operators are available. 
One set is appropriate for use with decimal values, and the other is for use with hexadecimal values. 
Also see Math (Decimal). Parenthetical expressions can be nested up to eight levels deep. 
If the Floating Point Data Type is used in any operation, then all operations will be based on Floating Point math. 
The solution will be stored in the data format selected for the Result.
______________________________________________________________________________________________________________________

Math encoding:
______________________________
6 bytes{0x00 0x40 0xff 0xff 0xff 0xff} if dec
or
6 bytes{0x00 0x41 0xff 0xff 0xff 0xff}	if dec + one shot
or
6 bytes{0x00 0x42 0xff 0xff 0xff 0xff}	if hex
or
6 bytes{0x00 0x43 0xff 0xff 0xff 0xff}	if hex + one shot

2 bytes{dest.register},
2 bytes{=N, string size, excluding ending null},
N bytes{string},
+padding nuls

Examples:

math: df1 = 1+2.343567
00 40 ff ff ff ff 1b 58 00 0a 31 2b 32 2e 33 34 33 35 36 37 

math: dd1 = 1+2+3
00 40 ff ff ff ff 11 94 00 05 31 2b 32 2b 33 00 


1) N = 5, M = 0:

00 40 ff ff ff ff            
11 94 
00 05 
44 44 31 2b 31 
00

2) N = 27, M = 2:

00 40 ff ff ff ff                                                                                                       
11 94 
00 1b 
44 44 31 2b 
53 49 4e 28 
44 44 32 29 
2b 37 2b 53 
51 52 54 28 
53 44 31 29 
2f 50 49 
00 
00 00 

*/

/*		
return: encoded size !=0, or error=0.
*/
int clickplc_exe_Math_enc(void* obj,unsigned char* stream)
{
	int encsize;
	EXE_Math* math = (EXE_Math*)obj;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_MATH_CALL_DEC|math->is_one_shot|(math->is_hex<<1);
	stream[2] = 0xff;
	stream[3] = 0xff;
	stream[4] = 0xff;
	stream[5] = 0xff;
	clickplc_reg_enc(&math->result_reg_ref,	stream+6);
	clickplc_int16_enc(math->formula_size,	stream+8);
	memcpy(stream+10,math->formula,math->formula_size);
	encsize = math->formula_size + 10;	
	switch (encsize & 0x03)
	{
	case 0://already aligned
		break;
	case 1://+3
		stream[encsize + 0] = 0x00; stream[encsize + 1] = 0x00;	stream[encsize + 2] = 0x00;
		encsize += 3;
		break;
	case 2://+2
		stream[encsize + 0] = 0x00; stream[encsize + 1] = 0x00;
		encsize += 2;
		break;
	case 3://+1
		stream[encsize + 0] = 0x00;
		encsize++;
		break;
	}	
	return(encsize);
}

int clickplc_exe_Math_dec(unsigned char* stream,void* obj)
{
	int encsize;
	EXE_Math* math = (EXE_Math*)obj;
	math->is_one_shot	=  stream[1]&0x01;
	math->is_hex		= (stream[1]&0x02)>>1;
	clickplc_reg_dec(stream+6,&math->result_reg_ref);
	clickplc_int16_dec(stream+8,&math->formula_size);
	memcpy(math->formula,stream+10,math->formula_size);
	encsize = math->formula_size + 10;
	if (encsize & 0x03)
	{
		encsize += 4-(encsize & 0x03);
	}
	return(encsize);
}

/*
#########################################################
COPY

The Single Copy instruction is used to copy a data or text value from its Source location to a specified Destination register. 
The Source location of the data or text can be another register, identified by its Memory Address, or it can be a constant value typed directly 
into the Source field on the dialog.  The Single Copy instruction allows you to copy numerical text values to one or more data registers as integer values.

The Block Copy instruction is used to copy data or text from its Source registers to specified sequential Destination registers. 
The Source location of the data or text is identified by its beginning and ending Memory Addresses, and the Destination registers are identified 
by the Memory Address of the first register in the sequence.

The Pack Copy mode of the Copy Instruction combines the status of up to 16 Source Bit Memory Addresses (X, Y, C, T, CT or SC) 
and copy the combined status into a Destination Data Register (DH or YD).

The Unpack Copy mode of the Copy Instruction copies the data in a Source Data Register (DH only) to up to 16 Destination Bit Memory Addresses ( Y or C).

The Fill mode of the Copy Instruction is used to copy a data or text value from its Source location to a specified range of Destination registers. 
The Source location of the data or text can be another register, identified by its Memory Address, or it can be a constant value typed directly 
into the Source field on the dialog. Constant data or text values must follow the typing conventions for that Data Type.


Copy encoding:

00		1 byte{0xa0}
01		1 bytes{copy type: single(0x00), pack(0x06), block(0x02), unpack(0x08), fill(0x04) | is_one_shot(0x01)}
02		1 byte{0x00-single, 0x11-pack,block; 0x01-unpack,fill;}
03		1 byte{src type: 0x11 - register; 0x71 - const string, 0x21 - const dec 16-bit, 0x31 - const dec 32-bit, 0x41 - const hex}
04		2 bytes{
single copy:
direct addressing: 0xff,0xff
by pointers [DSn]: dst.reg
other copy:
0xff,0xff
}
06		2 bytes{	
single copy:
direct addressing: dest.register
by pointers [DSn]: dst.pointer reg DSn
other copy:
dest.register first;
}
08		1 byte{0xff}
09		1 byte{0xff or 0xfe - if not suppress zero(single int2txt)/exponent form(single flt2txt)}
10		2 bytes{dest.register last: unpack,fill; or 0xff,0xff}
12		2 bytes	{

if src type=0x11: 
single copy:
direct addressing: 0xff,0xff;
by pointers [DSn]: src.reg
other copy:	0xff,0xff;

if src type=0x21,0x41:	0xff,0xff;

if src type=0x31: int32&0xff000000 >>3,int32&0x00ff0000 >>2,
if src type=0x71: N-string size
}
14		2 bytes {
if src type=0x11: src reg first
single copy:
direct addressing: src.reg;
by pointers [DSn]: src.pointer reg [DSn]
other copy:
src reg first;
if src 0x21,0x41: int16&0xff00 >>1,int16&0x00ff,
if src type=0x31: int32&0x0000ff00 >>1,int32&0x000000ff,
if src type=0x71: string...
}
######################## end for "single","fill","unpack" = 16 bytes, "single string->..."=14 bytes min;

### block,pack only: ###
16		2 bytes{0xff,0xff}
18		2 bytes {src reg last}
######################## end for "block","pack": 20 bytes.
_____________________

single: ds3->ds4 
0a 00 00 11 ff ff 00 03 ff ff ff ff ff ff 00 02 
0a 01 00 11 ff ff 00 03 ff ff ff ff ff ff 00 02  	1s

Currently, only the Copy instruction supports Pointer Addressing in the Single Copy mode.  
The Pointer Addressing can be used for the Source and/or Destination.

single copy by pointers: dd[ds4] -> dd[ds8]
0a 00 00 11 11 94 00 07 ff ff ff ff 11 94 00 03 
single copy by pointers: dec 16-bit 12345 -> dd[ds8]
0a 00 00 21 11 94 00 07 ff ff ff ff ff ff 30 39
single copy by pointers: dec 32-bit 1234567 -> dd[ds8]
0a 00 00 31 11 94 00 07 ff ff ff ff 00 12 d6 87
single copy by pointers: ascii string "ncvlkekemewm" -> txt[ds8]
0a 00 00 71 70 00 00 07 ff ff ff ff 00 0c 6e 63 76 6c 6b 65 6b 65 6d 65 77 6d 00 00
single copy: ctd1 -> dd1
0a 00 00 11 ff ff 11 94 ff ff ff ff ff ff 68 00
single copy: xd0 -> yd0
0a 00 00 11 ff ff 7c 40 ff ff ff ff ff ff 7c 00
single copy: ctd2 -> df2
0a 00 00 11 ff ff 1b 5a ff ff ff ff ff ff 68 02
single copy: x101 -> y801
0a 00 00 11 ff ff c5 00 ff ff ff ff ff ff c0 20

pack: x001...x016 -> dh10
0a 06 11 11 ff ff 19 6d ff ff ff ff ff ff c0 00 ff ff c0 0f 
0a 07 11 11 ff ff 19 6d ff ff ff ff ff ff c0 00 ff ff c0 0f		1s

pack: c1...c16 -> dh101 
0a 06 11 11 ff ff 19 c8 ff ff ff ff ff ff d0 00 ff ff d0 0f 
0a 07 11 11 ff ff 19 c8 ff ff ff ff ff ff d0 00 ff ff d0 0f		1s

pack: sc1...sc16 -> dh201
0a 06 11 11 ff ff 1a 2c ff ff ff ff ff ff f0 00 ff ff f0 0f 
0a 07 11 11 ff ff 1a 2c ff ff ff ff ff ff f0 00 ff ff f0 0f		1s

block: x001...x016 -> c601...c616
0a 02 11 11 ff ff d2 58 ff ff ff ff ff ff c0 00 ff ff c0 0f 
0a 03 11 11 ff ff d2 58 ff ff ff ff ff ff c0 00 ff ff c0 0f		1s

block: ds501...ds601 -> dd1..dd101 
0a 02 11 11 ff ff 11 94 ff ff ff ff ff ff 01 f4 ff ff 02 58 
0a 03 11 11 ff ff 11 94 ff ff ff ff ff ff 01 f4 ff ff 02 58		1s

block: dd701...dd801 -> df1...df101
0a 02 11 11 ff ff 1b 58 ff ff ff ff ff ff 17 0c ff ff 17 d4 
0a 03 11 11 ff ff 1b 58 ff ff ff ff ff ff 17 0c ff ff 17 d4		1s

unpack: dh401 -> c1001...c1016
0a 08 01 11 ff ff d3 e8 ff ff d3 f7 ff ff 1a f4 
0a 09 01 11 ff ff d3 e8 ff ff d3 f7 ff ff 1a f4		1s

fill: ds1001 -> ds3001...ds3020
0a 04 01 11 ff ff 0b b8 ff ff 0b cb ff ff 03 e8 
0a 05 01 11 ff ff 0b b8 ff ff 0b cb ff ff 03 e8		1s

fill: ds501 -> dd401..d3407 
0a 04 01 11 ff ff 14 b4 ff ff 14 c0 ff ff 01 f4 
0a 05 01 11 ff ff 14 b4 ff ff 14 c0 ff ff 01 f4		1s

fill: df401 -> df201...df213    
0a 04 01 11 ff ff 1c e8 ff ff 1d 00 ff ff 1e 78 
0a 05 01 11 ff ff 1c e8 ff ff 1d 00 ff ff 1e 78		1s

single: txt101 -> txt101
0a 00 00 11 ff ff 70 64 ff ff ff ff ff ff 70 64 
0a 01 00 11 ff ff 70 64 ff ff ff ff ff ff 70 64		1s

single one shot: txt401 -> txt101
0a 01 00 11 ff ff 70 64 ff ff ff ff ff ff 71 90 	1s
0a 00 00 11 ff ff 70 64 ff ff ff ff ff ff 71 90

single: "string..." -> 	txt301...313
0a 00 00 71 ff ff 71 2c ff ff ff ff 00 0d 61 62 2c 6d 64 6e 66 76 6e 65 6f 6e 65 00 

single: 12345 -> ds5
0a 00 00 21 ff ff 00 04 ff ff ff ff ff ff 30 39 

single: 123456 -> dd2
0a 00 00 31 ff ff 11 96 ff ff ff ff 00 01 e2 40 

single: 1A35 hex -> ds3001
0a 00 00 41 ff ff 0b b8 ff ff ff ff ff ff 1a 35 

single: 54e7 hex -> dd401
0a 00 00 41 ff ff 14 b4 ff ff ff ff ff ff 54 e7 

single: 567f hex -> df1
0a 00 00 41 ff ff 1b 58 ff ff ff ff ff ff 56 7f 

single suppress zero: 321f hex -> txt3
0a 00 00 41 ff ff 70 02 ff ff ff ff ff ff 32 1f 

single not suppress: 327f hex -> txt3
0a 00 00 41 ff ff 70 02 ff fe ff ff ff ff 32 7f 

single suppress: dd4 -> txt7
0a 00 00 11 ff ff 70 06 ff ff ff ff ff ff 11 9a 

single not suppress: dd4 -> txt7
0a 00 00 11 ff ff 70 06 ff fe ff ff ff ff 11 9a 

single real number: df7 -> txt5
0a 00 00 11 ff ff 70 04 ff ff ff ff ff ff 1b 64 

single exponent form: df7 -> txt5
0a 00 00 11 ff ff 70 04 ff fe ff ff ff ff 1b 64 

single suppress: 123456 -> txt4
0a 00 00 31 ff ff 70 03 ff ff ff ff 00 01 e2 40 

*/

/*		
return: encoded size !=0, or error=0.
*/	
int clickplc_exe_Copy_enc(void* obj,unsigned char* stream)
{
	int encsize = 0;
	EXE_Copy* copy = (EXE_Copy*)obj;
	
	//common bytes:
	stream[0] = EXE_CODE_INSTRUCTION_COPY;
	stream[1] = copy->type|copy->is_one_shot;
		
	stream[8] = 0xff;

	//enc destination
	switch (copy->type)
	{
	case EXE_CODE_INSTRUCTION_COPY_SINGLE:
		
		stream[2] = 0x00;
		stream[3] = copy->src_type;
		
		if (copy->is_dst_addr_by_pointer)
		{
			clickplc_reg_enc(&copy->dst_reg_start_ref,	&stream[4]);
			clickplc_reg_enc(&copy->dst_pointer_reg_ref,&stream[6]);
		}
		else
		{
			stream[4] = 0xff;
			stream[5] = 0xff;
			clickplc_reg_enc(&copy->dst_reg_start_ref,	&stream[6]);//6,7
		}
		stream[9] = ~copy->int2txt_not_suppress_zero_flt2txt_exponent_form;
		stream[10]= 0xff;
		stream[11]= 0xff;
		
		break;
	case EXE_CODE_INSTRUCTION_COPY_BLOCK:
	case EXE_CODE_INSTRUCTION_COPY_PACK:

		stream[2] = EXE_CODE_INSTRUCTION_COPY_SRC_REG;//=0x11
		stream[3] = copy->src_type;
		//...
		stream[4] = 0xff;
		stream[5] = 0xff;
		clickplc_reg_enc(&copy->dst_reg_start_ref,&stream[6]);//[6,7]
		stream[9] = 0xff;
		stream[10]= 0xff;
		stream[11]= 0xff;
		//...
		stream[16]= 0xff;
		stream[17]= 0xff;
		clickplc_reg_enc(&copy->src_reg_end_ref,&stream[18]);//[18,19]
		//!!!!!!!!!!! special case !!!!!!!!!!!
		encsize = 20;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		break;
		
	case EXE_CODE_INSTRUCTION_COPY_UNPACK:
	case EXE_CODE_INSTRUCTION_COPY_FILL:

#define EXE_CODE_INSTRUCTION_COPY_FILL_BY_REG			0x01
#define EXE_CODE_INSTRUCTION_COPY_FILL_BY_CONSTDEC16b	0x02
#define EXE_CODE_INSTRUCTION_COPY_FILL_BY_CONSTDEC32b	0x03
#define EXE_CODE_INSTRUCTION_COPY_FILL_BY_CONSTHEX		0x04
#define EXE_CODE_INSTRUCTION_COPY_FILL_BY_CONSTFLT32	0x06
#define EXE_CODE_INSTRUCTION_COPY_FILL_BY_CONSTSTR		0x07

		stream[2] = (copy->src_type>>4);				//src
		stream[3] = EXE_CODE_INSTRUCTION_COPY_SRC_REG;	//dst =0x11
		//...
		stream[4] = 0xff;
		stream[5] = 0xff;		
		clickplc_reg_enc(&copy->dst_reg_start_ref,	&stream[6]);//[6,7]
		stream[9] = 0xff;		
		clickplc_reg_enc(&copy->dst_reg_end_ref,	&stream[10]);//[10,11]
		//...
		
		break;
	}	
	//enc source
	switch (copy->src_type)
	{
	case EXE_CODE_INSTRUCTION_COPY_SRC_REG:

		if (copy->type == EXE_CODE_INSTRUCTION_COPY_SINGLE)
		{
			if (copy->is_src_addr_by_pointer)
			{
				clickplc_reg_enc(&copy->src_reg_start_ref,	&stream[12]);//[12,13]
				clickplc_reg_enc(&copy->src_pointer_reg_ref,&stream[14]);//[14,15]
			}
			else
			{
				stream[12]= 0xff;
				stream[13]= 0xff;
				clickplc_reg_enc(&copy->src_reg_start_ref,	&stream[14]);//[14,15]
			}			
		}
		else
		{
			stream[12]= 0xff;
			stream[13]= 0xff;
			clickplc_reg_enc(&copy->src_reg_start_ref,	&stream[14]);//[14,15]
		}
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTDEC16b:
		stream[12]= 0xff;
		stream[13]= 0xff;
		clickplc_int16_enc((unsigned short)copy->src_const_int16,&stream[14]);//[14,15]
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTDEC32b:
		clickplc_int32_enc((unsigned int)copy->src_const_int32,&stream[12]);//[12,13,14,15]
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTHEX:
		stream[12]= 0xff;
		stream[13]= 0xff;
		clickplc_int16_enc(copy->src_const_hex16bit,&stream[14]);//[14,15]
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTFLT32:
		{
			unsigned char tmp32[4];
			memcpy(tmp32,&copy->src_const_flt32,sizeof(float));			
			stream[15] = tmp32[0];
			stream[14] = tmp32[1];
			stream[13] = tmp32[2];
			stream[12] = tmp32[3];
			//printf("\tenc: %e -> %02x,%02x,%02x,%02x\n",copy->src_const_flt32,stream[12],stream[13],stream[14],stream[15]);
		}
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTSTR:
		clickplc_int16_enc(copy->ascii_string_size, &stream[12]);//[12,13]
		memcpy(&stream[14],copy->ascii_string,copy->ascii_string_size);
		//+padding nulls:		
		//!!!!!!!!!!! special case !!!!!!!!!!!
		encsize = 14 + copy->ascii_string_size;
		switch (encsize & 0x03)
		{
		case 0://already aligned
			break;
		case 1://+3
			stream[encsize + 0] = 0x00;	stream[encsize + 1] = 0x00;	stream[encsize + 2] = 0x00;
			encsize += 3;
			break;
		case 2://+2
			stream[encsize + 0] = 0x00;	stream[encsize + 1] = 0x00;
			encsize += 2;
			break;
		case 3://+1
			stream[encsize + 0] = 0x00;
			encsize++;
			break;
		}
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		break;

	default:
		printf("EXE Cope enc: Unknown src-type ! byte[3]=%02x !\n",copy->src_type);

	}
	//default enc.size:
	if(encsize==0) encsize = 16;
	
	return(encsize);
}

int clickplc_exe_Copy_dec(	unsigned char* stream,void* obj)
{
	int encsize = 0;
	EXE_Copy* copy = (EXE_Copy*)obj;
	
	copy->type			= stream[1]&0xfe;
	copy->is_one_shot	= stream[1]&0x01;	
		
	copy->int2txt_not_suppress_zero_flt2txt_exponent_form = ~stream[9];
	//dec destination
	switch (copy->type)
	{
	case EXE_CODE_INSTRUCTION_COPY_SINGLE:
		
		copy->src_type		= stream[3];

		if ((stream[4]== 0xff) && (stream[5]==0xff))
		{
			copy->is_dst_addr_by_pointer = 0;
		}
		else
		{
			copy->is_dst_addr_by_pointer = 1;
		}
		
		if (copy->is_dst_addr_by_pointer)
		{
			clickplc_reg_dec(&stream[4],&copy->dst_reg_start_ref);
			clickplc_reg_dec(&stream[6],&copy->dst_pointer_reg_ref);
		}
		else
		{
			clickplc_reg_dec(&stream[6],&copy->dst_reg_start_ref);
		}
		break;

	case EXE_CODE_INSTRUCTION_COPY_BLOCK:
	case EXE_CODE_INSTRUCTION_COPY_PACK:

		copy->src_type		= stream[3];

		clickplc_reg_dec(&stream[6],	&copy->dst_reg_start_ref);	//[6,7]
		clickplc_reg_dec(&stream[18],	&copy->src_reg_end_ref);	//[18,19]
		//!!!!!!!!!!! special case !!!!!!!!!!!
		encsize = 20;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		break;

	case EXE_CODE_INSTRUCTION_COPY_UNPACK:
	case EXE_CODE_INSTRUCTION_COPY_FILL:
		copy->src_type = (stream[2]<<4)|0x01;
		//copy->fillunpack_in_type = stream[3];	//dst		

		if (stream[3] != EXE_CODE_INSTRUCTION_COPY_SRC_REG)
		{
			printf("EXE Cope dec: Unknown fillunpack_in_type ! byte[3]=%02x !\n",stream[3]);
		}

		clickplc_reg_dec(&stream[6],	&copy->dst_reg_start_ref);	//[6,7]
		clickplc_reg_dec(&stream[10],	&copy->dst_reg_end_ref);	//[10,11]
		break;
	}	
	//dec source
	switch (copy->src_type)
	{
	case EXE_CODE_INSTRUCTION_COPY_SRC_REG:

		if (copy->type == EXE_CODE_INSTRUCTION_COPY_SINGLE)
		{
			if ((stream[12]== 0xff) && (stream[13]==0xff))
			{
				copy->is_src_addr_by_pointer = 0;
			}
			else
			{
				copy->is_src_addr_by_pointer = 1;
			}

			if (copy->is_src_addr_by_pointer)
			{
				clickplc_reg_dec(&stream[12],&copy->src_reg_start_ref);		//[12,13]
				clickplc_reg_dec(&stream[14],&copy->src_pointer_reg_ref);	//[14,15]
			}
			else
			{
				clickplc_reg_dec(&stream[14],&copy->src_reg_start_ref);		//[14,15]
			}			
		}
		else
		{
			clickplc_reg_dec(&stream[14],&copy->src_reg_start_ref);	//[14,15]
		}
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTDEC16b:
		{
			unsigned short tmp16;
			clickplc_int16_dec(&stream[14],&tmp16);//[14,15]
			copy->src_const_int16 = (short)tmp16;
		}		
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTDEC32b:
		{
			unsigned int tmp32;
			clickplc_int32_dec(&stream[12],&tmp32);//[12,13,14,15]
			copy->src_const_int32 = (int)tmp32;
		}

		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTHEX:
		clickplc_int16_dec(&stream[14],&copy->src_const_hex16bit);//[14,15]
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTFLT32:
		{
//if (sizeof(float)==4)
			unsigned char tmp32[4];
			//clickplc_int32_dec(&stream[12],&tmp32);//[12,13,14,15]
			tmp32[0] = stream[15];
			tmp32[1] = stream[14];
			tmp32[2] = stream[13];
			tmp32[3] = stream[12];
			memcpy(&copy->src_const_flt32,tmp32,sizeof(float));
//else
//			printf("sizeof(float)!=4 !!!\n");
//endif
			//printf("\tdec: %e <- %02x,%02x,%02x,%02x\n",copy->src_const_flt32,stream[12],stream[13],stream[14],stream[15]);
		}
		break;

	case EXE_CODE_INSTRUCTION_COPY_SRC_CONSTSTR:
		clickplc_int16_dec(&stream[12],&copy->ascii_string_size);//[12,13]
		memcpy(copy->ascii_string,&stream[14],copy->ascii_string_size);
		//!!!!!!!!!!! special case !!!!!!!!!!!
		encsize = 14 + copy->ascii_string_size;
		if (encsize & 0x03)
		{
			encsize += 4-(encsize & 0x03);
		}
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		break;

	default:
		printf("EXE Cope dec: Unknown src-type ! byte[3]=%02x !\n",copy->src_type);
	}
	//default enc.size:
	if(encsize==0) encsize = 16;	

	return(encsize);
}


/*

DRUM

The Drum instruction simulates an electromechanical drum sequencer, using either a Time Based or an Event Base sequencing strategy. 
Each Drum instruction is capable of sequencing through 1 to 16 steps and turning ON as many as 16 outputs in a user defined pattern. 
Outputs can be either physical outputs or internal control relays. A Flag Bit is turned ON to indicate the completion of the sequence.

DRUM encoding:

00		1 byte{0x20}
01		1 byte{

time based = 0x01 
| ms=0x00
| sec=0x02
| min=0x04
| hrs=0x06
| days=0x08

event based= 0x0b	
}
02		1 byte{number of steps: 1...16}
03		1 byte{number of outputs: 1...16}
04		2 bytes{DS reg - current step number}
06		2 bytes{
time based: elapsed time in step, TD only
event based: 0xff,0xff
}

08		2 bytes{complete bit reg, C only}

10		2 bytes{
if load new step number=on: new step number reg, DS only;
if load new step number=off: 0xaa,0xaa;
}

12		32 bytes{	16x2
time based:
16 output bit regs; 0x00,0x00 if disabled;
event based:
16 output bit regs; 0x00,0x00 if disabled;
}

44		4 x steps bytes{
time based:
2 bytes{duration(ms,sec,min,hrs,days)},
2 bytes{bit encoded sequence, 0-bit->step#1, 15-bit->step#16}
event based:
2 bytes{event bit regs},
2 bytes{bit encoded sequence, 0-bit->step#1, 15-bit->step#16}
}
_______________________________________
Total: 44 + 4 x steps; min: 44 bytes, max: 108 bytes.

Time based

###	00 07 ff ff enable
###	01 00 c0 00 reset
###	01 00 c0 01 step no
###	01 00 c0 02 jog in

ms		ds4		td1		c1		ds8
20 01 10 10 [00 03] [60 00] [d0 00] [00 07] 16 steps x 16 outputs
20 01 0f 10 [00 03] [60 00] [d0 00] [00 07] 15 steps x 16 outputs
20 01 01 01 [00 03] [60 00] [d0 00] [00 07] 1 step x 1 output
sec
20 03 10 10 [00 03] [60 00] [d0 00] [00 07] 
min
20 05 10 10 [00 03] [60 00] [d0 00] [00 07] 
hrs
20 07 10 10 [00 03] [60 00] [d0 00] [00 07] 
days
20 09 10 10 [00 03] [60 00] [d0 00] [00 07] 

16 outputs					1 output
c4 00 						c4 00 
c4 01 						00 00
c4 02 						00 00
c4 03 						00 00
c4 04 						00 00
c4 05 						00 00
c4 06 						00 00
c4 07 						00 00
c4 08 						00 00
c4 09 						00 00
c4 0a 						00 00
c4 0b 						00 00
c4 0c 						00 00
c4 0d 						00 00
c4 0e 						00 00
c4 0f 						00 00

time	16-bit encoded pattern				1x1
00 0a 55 55 		10		0101 0101 0101 0101					00 0a 00 01
00 14 00 02 		20		0000 0000 0000 0010
00 1e 00 04 		30		0000 0000 0000 0100
00 28 00 08 		40		0000 0000 0000 1000
00 32 00 10 		50		0000 0000 0001 0000
00 3c 00 20 		60		0000 0000 0010 0000
00 46 00 40 		70		0000 0000 0100 0000
00 50 00 80 		80		0000 0000 1000 0000
00 5a 01 00 		90		0000 0001 0000 0000
00 64 02 00 		100		0000 0010 0000 0000
00 6e 04 00 		110		0000 0100 0000 0000
00 78 08 00 		120		0000 1000 0000 0000
00 82 10 00 		130		0001 0000 0000 0000
00 8c 20 00 		140		0010 0000 0000 0000
00 96 40 00 		150		0100 0000 0000 0000		15x16
00 a0 80 00 		160		1000 0000 0000 0000		16x16


Event based:

event based 16x16, events: x001...x016; outputs: y001...y016; step: ds4, new step: ds8, complete bit: c1;
+ jog input;
+ load new step number;

###	01 00 d0 01 enable
###	01 00 d0 02 reset
###	01 00 d0 03 step no.
###	01 00 d0 04 jog in
###	20 0b 10 10 [00 03] ff ff [d0 00] [00 07] 

event based 16x15, events: x001...x016; outputs: y001...y015; step: ds4, new step: ds8, complete bit: c1;
+ jog input;
+ load new step number;

###	20 0b 10 0f [00 03] ff ff [d0 00] [00 07] 

event based 15x16, events: x001...x015; outputs: y001...y016; step: ds4, new step: ds8, complete bit: c1;
+ jog input;
+ load new step number;

###	20 0b 0f 10 [00 03] ff ff [d0 00] [00 07] 

event based 16x16, events: x001...x016; outputs: y001...y016; step: ds4, complete bit: c1;
+ jog input;

###	01 00 d0 01 enable
###	01 00 d0 02 reset
###	01 00 d0 03 step no.			 
###	20 0b 10 10 [00 03] ff ff [d0 00] [aa aa] 				 

event based 16x16, events: x001...x016; outputs: y001...y016; step: ds4, complete bit: c1;

###	01 00 d0 01 enable
###	01 00 d0 02 step no.								
###	20 0a 10 10 [00 03] ff ff [d0 00] [aa aa] 			 

16x16

###	c4 00 c4 01 
###	c4 02 c4 03 
###	c4 04 c4 05 
###	c4 06 c4 07 
###	c4 08 c4 09 
###	c4 0a c4 0b 
###	c4 0c c4 0d 		16x15
###	c4 0e c4 0f 		###	c4 0e 00 00 

###	c0 00 55 55 
###	c0 01 00 02 
###	c0 02 00 04 
###	c0 03 00 08 
###	c0 04 00 10 
###	c0 05 00 20 
###	c0 06 00 40 
###	c0 07 00 80 
###	c0 08 01 00 
###	c0 09 02 00 
###	c0 0a 04 00 
###	c0 0b 08 00 
###	c0 0c 10 00 
###	c0 0d 20 00 								15x16
###	c0 0e 40 00 		16x15					###	c0 0e 40 00 
###	c0 0f 80 00         ###	c0 0f 00 00 


jog in = on:

00 07 ff ff enable
01 00 c0 00 reset
01 00 c0 01 step no: load new
01 00 c0 02 jog in
20 05 10 10 00 03 60 00 d0 00 00 07...

jog in = off:

00 07 ff ff enable
01 00 c0 00 reset
01 00 c0 01 step no: load new
20 04 10 10 00 03 60 00 d0 00 00 07 ...

*/

/*		
return: encoded size !=0, or error=0.
*/	
int clickplc_exe_Drum_enc(	void* obj,unsigned char* stream)
{
	int encsize;
	EXE_Drum* drum = (EXE_Drum*)obj;
	int i;
	
	stream[0] = EXE_CODE_INSTRUCTION_DRUM;
	stream[1] = drum->drum_type|drum->is_jog_in_enabled;
	stream[2] = drum->n_drum_steps;
	stream[3] = drum->n_drum_outputs;
	clickplc_reg_enc(&drum->curr_step_reg_ref,&stream[4]);
	if (drum->drum_type < EXE_CODE_INSTRUCTION_DRUM_EVENTBASED)
	{
		clickplc_reg_enc(&drum->elapsed_step_time_reg_ref,&stream[6]);
	}
	else
	{
		stream[6] = 0xff;
		stream[7] = 0xff;
	}
	clickplc_reg_enc(&drum->complete_bit_reg_ref,&stream[8]);
	if (drum->is_load_new_step_enabled)
	{
		clickplc_reg_enc(&drum->new_step_reg_ref,&stream[10]);
	}
	else
	{
		stream[10] = 0xaa;
		stream[11] = 0xaa;
	}
	for (i = 0; i<DRUM_MAX_OUTPUTS; i++)
	{
		clickplc_reg_enc(&drum->outputs_bit_regs_ref[i],&stream[12+i*2]);
	}
	if (drum->drum_type < EXE_CODE_INSTRUCTION_DRUM_EVENTBASED)
	{	//Time based
		for (i = 0; i<drum->n_drum_steps; i++)
		{	
			clickplc_int16_enc(drum->steps_time_duration[i],&stream[44+i*4]);
			clickplc_int16_enc(drum->steps_enc_sequences[i],&stream[46+i*4]);
		}
	}
	else
	{	//Event based
		for (i = 0; i<drum->n_drum_steps; i++)
		{	
			clickplc_reg_enc	(&drum->steps_event_bits_ref[i],&stream[44+i*4]);
			clickplc_int16_enc	(drum->steps_enc_sequences[i],	&stream[46+i*4]);
		}
	}
	encsize = 44 + 4*drum->n_drum_steps;
	return(encsize);
}

int clickplc_exe_Drum_dec(	unsigned char* stream,void* obj)
{
	int encsize;
	EXE_Drum* drum = (EXE_Drum*)obj;
	int i;

	drum->drum_type			= stream[1]&0xfe;
	drum->is_jog_in_enabled = stream[1]&0x01;
	drum->n_drum_steps		= stream[2];
	drum->n_drum_outputs	= stream[3];
	clickplc_reg_dec(&stream[4],&drum->curr_step_reg_ref);
	if (drum->drum_type < EXE_CODE_INSTRUCTION_DRUM_EVENTBASED)
	{
		clickplc_reg_dec(&stream[6],&drum->elapsed_step_time_reg_ref);
	}
	clickplc_reg_dec(&stream[8],&drum->complete_bit_reg_ref);
	if ((stream[10] == 0xaa) && (stream[11] == 0xaa))
	{
		drum->is_load_new_step_enabled = 0;
	}
	else
	{
		drum->is_load_new_step_enabled = 1;
		clickplc_reg_dec(&stream[10],&drum->new_step_reg_ref);
	}
	for (i = 0; i<DRUM_MAX_OUTPUTS; i++)
	{
		clickplc_reg_dec(&stream[12+i*2],&drum->outputs_bit_regs_ref[i]);
	}
	if (drum->drum_type < EXE_CODE_INSTRUCTION_DRUM_EVENTBASED)
	{	//Time based
		for (i = 0; i<drum->n_drum_steps; i++)
		{	
			clickplc_int16_dec(&stream[44+i*4],&drum->steps_time_duration[i]);
			clickplc_int16_dec(&stream[46+i*4],&drum->steps_enc_sequences[i]);
		}
	}
	else
	{	//Event based
		for (i = 0; i<drum->n_drum_steps; i++)
		{	
			clickplc_reg_dec	(&stream[44+i*4],&drum->steps_event_bits_ref[i]);
			clickplc_int16_dec	(&stream[46+i*4],&drum->steps_enc_sequences[i]);
		}
	}
	encsize = 44 + 4*drum->n_drum_steps;	
	return(encsize);
}


/*
SEARCH:

00	1 byte{0x0c}
01	1 byte{compare: 
		==	(0x10 or 0x20) | 0x00
		!=	(0x10 or 0x20) | 0x01
		>	(0x10 or 0x20) | 0x02
		>=	(0x10 or 0x20) | 0x03
		<	(0x10 or 0x20) | 0x04
		<=	(0x10 or 0x20) | 0x05
02	1 byte{searching data's container: 0x11 - register, 0x71 - ascii string}
03	1 byte{0x11}
04	2 bytes{0xff,0xff}
06	2 bytes{dest.reg, DS only}
08	1 byte{0xff}
09	1 byte{0xff - cont.search=off, 0xfe - cont.search=on}
10	2 bytes{found bit reg, C only}
12	2 bytes{0xff,0xff}
14	2 bytes{first reg from search range}
16	2 bytes{0xff,0xff}
18	2 bytes{last reg from search range}

if searching data's container = 0x11 (register):
20	2 bytes{0xff,0xff}
22	2 bytes{searching register}
---------------------------------------Total = 24 bytes
if searching data's container = 0x71 (ascii string):
20	2 bytes{string size=N}
22	N bytes{string}
+padding nulls
---------------------------------------Total = (22 + string size) 4-aligned by nulls


0c 10 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07 	==		==ds8 [ds101...ds111], ->ds4,c1
0c 20 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		1s

0c 12 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		>		> ds8 [ds101...ds111], ->ds4,c1
0c 22 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		1s

0c 13 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07 	>=		>=ds8 [ds101...ds111], ->ds4,c1
0c 23 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		1s

0c 11 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		!=		!=ds8 [ds101...ds111], ->ds4,c1
0c 21 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		1s

0c 14 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		<		< ds8 [ds101...ds111], ->ds4,c1
0c 24 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		1s

0c 15 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07 	<=		<=ds8 [ds101...ds111], ->ds4,c1
0c 25 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07		1s

==ds8 [ds101...ds111], ->ds4,c1			cont.search=on
0c 10 11 11 ff ff 00 03 ff fe d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07 			

==ds8 [ds101...ds111], ->ds4,c1		1s
0c 20 11 11 ff ff 00 03 ff ff d0 00 ff ff 00 64 ff ff 00 6e ff ff 00 07 

=="ABC" [txt1...txt100], ->ds4,c1
0c 10 71 11 ff ff 00 03 ff ff d0 00 ff ff 70 00 ff ff 70 63 00 03 41 42 43 00 00 00 

=="ABC" [txt1...txt100], ->ds4,c1		cont.search=on,1s
0c 20 71 11 ff ff 00 03 ff fe d0 00 ff ff 70 00 ff ff 70 63 00 03 41 42 43 00 00 00 

<= df1 [dd1...dd100], -> ds4,c1
0c 15 11 11 ff ff 00 03 ff ff d0 00 ff ff 11 94 ff ff 12 5a ff ff 1b 58 

== dd1 [dd2...dd8], -> ds4,c1	cont.search=on,1s
0c 20 11 11 ff ff 00 03 ff fe d0 00 ff ff 11 96 ff ff 11 a2 ff ff 11 94 

== dd1 [ds1...ds3], -> ds4,c1	cont.search=on,1s
0c 20 11 11 ff ff 00 03 ff fe d0 00 ff ff 00 00 ff ff 00 02 ff ff 11 94 

== dh1 [dh101...dh201], -> ds4,c1
0c 10 11 11 ff ff 00 03 ff ff d0 00 ff ff 19 c8 ff ff 1a 2c ff ff 19 64 

*/

/*
#define EXE_CODE_INSTRUCTION_SEARCH_EQUAL				0x10
#define EXE_CODE_INSTRUCTION_SEARCH_NOTEQUAL			0x11
#define EXE_CODE_INSTRUCTION_SEARCH_MORE				0x12
#define EXE_CODE_INSTRUCTION_SEARCH_MOREOREQUAL			0x13
#define EXE_CODE_INSTRUCTION_SEARCH_LESS				0x14
#define EXE_CODE_INSTRUCTION_SEARCH_LESSOREQUAL			0x15

#define EXE_CODE_INSTRUCTION_SEARCH_EQUAL_1TIME			0x20
#define EXE_CODE_INSTRUCTION_SEARCH_NOTEQUAL_1TIME		0x21
#define EXE_CODE_INSTRUCTION_SEARCH_MORE_1TIME			0x22
#define EXE_CODE_INSTRUCTION_SEARCH_MOREOREQUAL_1TIME	0x23
#define EXE_CODE_INSTRUCTION_SEARCH_LESS_1TIME			0x24
#define EXE_CODE_INSTRUCTION_SEARCH_LESSOREQUAL_1TIME	0x25
*/

/*		
return: encoded size !=0, or error=0.
*/	
int clickplc_exe_Search_enc(void* obj,unsigned char* stream)
{
	int encsize;
	EXE_Search* search = (EXE_Search*)obj;
	
	stream[0] = EXE_CODE_INSTRUCTION_SEARCH;
	stream[1] = search->search_condition&0x0f;
	if (search->is_one_shot)
	{
		stream[1] |= 0x20;
	}
	else
	{
		stream[1] |= 0x10;
	}
	stream[2] = search->searching_container;
	stream[3] = 0x11;
	stream[4] = 0xff;
	stream[5] = 0xff;
	clickplc_reg_enc(&search->result_reg_ref,&stream[6]);
	stream[8] = 0xff;
	stream[9] = ~search->is_continuous_search;
	clickplc_reg_enc(&search->found_bit_ref,&stream[10]);
	stream[12] = 0xff;
	stream[13] = 0xff;
	clickplc_reg_enc(&search->search_range_first_reg_ref,&stream[14]);
	stream[16] = 0xff;
	stream[17] = 0xff;
	clickplc_reg_enc(&search->search_range_last_reg_ref,&stream[18]);

	switch(search->searching_container)
	{
	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_ASCIISTR:

		clickplc_int16_enc(search->searching_string_size,&stream[20]);
		memcpy(&stream[22],search->searching_string,search->searching_string_size);
		encsize = 22 + search->searching_string_size;
		switch (encsize & 0x03)
		{
		case 0://already aligned
			break;
		case 1://+3
			stream[encsize + 0] = 0x00;	stream[encsize + 1] = 0x00;	stream[encsize + 2] = 0x00;
			encsize += 3;
			break;
		case 2://+2
			stream[encsize + 0] = 0x00;	stream[encsize + 1] = 0x00;
			encsize += 2;
			break;
		case 3://+1
			stream[encsize + 0] = 0x00;
			encsize++;
			break;
		}
		break;

	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC16bit:
		stream[20] = 0xff;
		stream[21] = 0xff;
		clickplc_int16_enc((unsigned short)search->searching_const_int16,&stream[22]);
		encsize = 24;
		break;

	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC32bit:
		clickplc_int32_enc((unsigned int)search->searching_const_int32,&stream[20]);
		encsize = 24;
		break;

	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX16:
		stream[20] = 0xff;
		stream[21] = 0xff;
		clickplc_int16_enc(search->searching_const_hex16bit,&stream[22]);
		encsize = 24;
		break;

	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_FLT32:
		{
			unsigned char tmp32[4];
			memcpy(tmp32,&search->searching_const_flt32,sizeof(float));
			stream[23] = tmp32[0];
			stream[22] = tmp32[1];
			stream[21] = tmp32[2];
			stream[20] = tmp32[3];
		}
		encsize = 24;
		break;

	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_REGISTER:
	default:
		stream[20] = 0xff;
		stream[21] = 0xff;
		clickplc_reg_enc(&search->searching_reg_ref,&stream[22]);
		encsize = 24;
	}
	return(encsize);
}

int clickplc_exe_Search_dec(unsigned char* stream,void* obj)
{
	int encsize;
	EXE_Search* search = (EXE_Search*)obj;
	
	search->search_condition = (stream[1]&0x0f)|0x10;
	if (stream[1]&0x20)
	{		
		search->is_one_shot = 1;
	} 
	else
	{
		search->is_one_shot = 0;
	}
	
	search->searching_container = stream[2];
	clickplc_reg_dec(&stream[6],	&search->result_reg_ref);
	search->is_continuous_search = ~stream[9];
	clickplc_reg_dec(&stream[10],	&search->found_bit_ref);
	clickplc_reg_dec(&stream[14],	&search->search_range_first_reg_ref);
	clickplc_reg_dec(&stream[18],	&search->search_range_last_reg_ref);
	
	//printf("search->searching_container = %02x\n",search->searching_container);

	switch(search->searching_container)
	{
	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_ASCIISTR:
		
		clickplc_int16_dec(&stream[20],	&search->searching_string_size);
		memcpy(search->searching_string,&stream[22],search->searching_string_size);
		encsize = 22 + search->searching_string_size;
		if (encsize & 0x03)
		{
			encsize += 4 - (encsize & 0x03);
			//printf("search: encsize = %d\n",encsize);
		}
		break;

	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC16bit:
		clickplc_int16_dec(&stream[22],	(unsigned short*)&search->searching_const_int16);
		encsize = 24;
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC32bit:
		clickplc_int32_dec(&stream[20],	(unsigned int*)&search->searching_const_int32);
		encsize = 24;
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX16:
		clickplc_int16_dec(&stream[22],	&search->searching_const_hex16bit);
		encsize = 24;
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_FLT32:
		{
			unsigned char tmp32[4];
			tmp32[0] = stream[23];
			tmp32[1] = stream[22];
			tmp32[2] = stream[21];
			tmp32[3] = stream[20];
			memcpy(&search->searching_const_flt32,tmp32,sizeof(float));
		}
		encsize = 24;
		break;
		
	case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_REGISTER:
	default:
		clickplc_reg_dec(&stream[22],	&search->searching_reg_ref);
		encsize = 24;
	}
	return(encsize);
}

/*

################################################
Send ASCII encoding:

00	1 byte{0x11}
01	1 byte{type of message/protocol: static(0x02), dynamic(0x01), modbus(0x00)}
02	1 byte{0x00 - ?!}
03	1 byte{port index|flags(for dynamic msg: 0x10 if constant length of msg has been encoded in last 2 bytes)}
04	4 bytes{0xff,0xff...}
08	2 bytes{success bit reg}
10	2 bytes{sending bit reg or 0xff,0xff}
12	8 bytes{0xff...}
20	1 byte{byte swap}
21	1 byte{n terminal bytes}
22	2 bytes{terminal bytes, 0x00 if none}
24	2 bytes{static msg: string length; dynamic msg: source TXT reg}

26	static msg: ascii bytes of string + nuls
26	dynamic msg: 2 bytes{constant number of sending bytes or DS reg, holding length of msg}

static text; 2 terminal : 0x0a,0x0d; 3 ASCII :"AAA"; success->C2, sending->C1

11 02 00 01 ff ff ff ff d0 01 d0 00 ff ff ff ff ff ff ff ff 00 02 0a 0d 00 03 41 41 41 00 00 00 

static text; 2 terminal : 0x0a,0x0d; 3 ASCII :"AAA"; success->C2
11 02 00 01 ff ff ff ff d0 01 ff ff ff ff ff ff ff ff ff ff 00 02 0a 0d 00 03 41 41 41 00 00 00

static text; 1 terminal : 0x0a; 4 ASCII :"AAAA"; success->C2, sending->C1 

11 02 00 01 ff ff ff ff d0 01 d0 00 ff ff ff ff ff ff ff ff 00 01 00 0a 00 04 41 41 41 41 00 00 


dynamic text; 2 terminal : 0x0a,0x0b; 128 bytes from TXT1; no byte swap; success->C2, sending->C1

11 01 00 11 ff ff ff ff d0 01 d0 00 ff ff ff ff ff ff ff ff 00 02 0a 0b 70 00 00 80 

dynamic text; 2 terminal : 0x0a,0x0d; DS1 bytes from TXT3; byte swap for all; success->C2, sending->C1

11 01 00 01 ff ff ff ff d0 01 d0 00 ff ff ff ff ff ff ff ff 01 02 0a 0d 70 02 00 00 

dynamic text; no terminal bytes; 32 bytes from TXT2; byte swap for all but nul; success->C2, sending->C1

11 01 00 11 ff ff ff ff d0 01 d0 00 ff ff ff ff ff ff ff ff 02 00 00 00 70 01 00 20 

*/

#define SEND_ASCII_FLAGS_MSG_LENGTH_IS_CONST 0x10

/*		
return: encoded size !=0, or error=0.
*/	
int clickplc_exe_Send_enc(void* obj,unsigned char* stream)
{
	int encsize;
	EXE_Send* send = (EXE_Send*)obj;

	stream[0] = EXE_CODE_INSTRUCTION_SEND;
	stream[1] = send->msg_type;
	stream[2] = 0x00;
	stream[3] = send->port_index;
	if (send->is_const_length_of_dynamic)
	{
		stream[3] |= SEND_ASCII_FLAGS_MSG_LENGTH_IS_CONST;
	}
	stream[4] = 0xff;
	stream[5] = 0xff;
	stream[6] = 0xff;
	stream[7] = 0xff;
	clickplc_reg_enc(&send->success_bit_ref,&stream[8]);
	if (send->is_sending_bit_used)
	{
		clickplc_reg_enc(&send->sending_bit_ref,&stream[10]);
	}
	else
	{
		stream[10] = 0xff;
		stream[11] = 0xff;
	}

	//8 bytes 0xff
	stream[12] = 0xff;
	stream[13] = 0xff;
	stream[14] = 0xff;
	stream[15] = 0xff;
	stream[16] = 0xff;
	stream[17] = 0xff;
	stream[18] = 0xff;
	stream[19] = 0xff;
	
	stream[20] = send->byte_swap;
	stream[21] = send->n_terminals;
	switch (send->n_terminals)
	{	
	case 1:
	case 2:
		stream[22] = send->terminal_byte_1;
		stream[23] = send->terminal_byte_2;
		break;

	case 0:
	default:
		stream[22] = 0x00;
		stream[23] = 0x00;
	}
	switch (send->msg_type)
	{
	case SEND_MSG_TYPE_MODBUS:
		//TODO!!!
		encsize = 28;
		break;

	case SEND_MSG_TYPE_DYNAMIC:
		clickplc_reg_enc(&send->dynamic_msg_source_ref,&stream[24]);
		if (send->is_const_length_of_dynamic)
		{
			clickplc_int16_enc(send->dynamic_msg_const_length,&stream[26]);
		}
		else
		{
			clickplc_reg_enc(&send->dynamic_msg_length_ref,&stream[26]);
		}
		encsize = 28;
		break;
		
	case SEND_MSG_TYPE_STATIC:
	default:
		clickplc_int16_enc(send->static_msg_length,&stream[24]);
		memcpy(&stream[26],send->static_msg,send->static_msg_length);
		encsize = 26 + send->static_msg_length;
		switch (encsize & 0x03)
		{
		case 0://already aligned
			break;
		case 1://+3
			stream[encsize + 0] = 0x00;	stream[encsize + 1] = 0x00;	stream[encsize + 2] = 0x00;
			encsize += 3;
			break;
		case 2://+2
			stream[encsize + 0] = 0x00;	stream[encsize + 1] = 0x00;
			encsize += 2;
			break;
		case 3://+1
			stream[encsize + 0] = 0x00;
			encsize++;
			break;
		}
	}
	return(encsize);
}

int clickplc_exe_Send_dec(	unsigned char* stream,void* obj)
{
	int encsize;
	EXE_Send* send = (EXE_Send*)obj;

	send->msg_type = stream[1];
	send->port_index = stream[3]&0x0f;	
	send->is_const_length_of_dynamic = stream[3]&SEND_ASCII_FLAGS_MSG_LENGTH_IS_CONST;
	
	clickplc_reg_dec(&stream[8],&send->success_bit_ref);
	
	if( (stream[10]==0xff) && (stream[11]==0xff) )
	{
		send->is_sending_bit_used = 0;
	}
	else
	{
		send->is_sending_bit_used = 1;
	}
	if (send->is_sending_bit_used)
	{
		clickplc_reg_dec(&stream[10],&send->sending_bit_ref);
	}
	
	send->byte_swap		= stream[20];
	send->n_terminals	= stream[21];
	
	switch (send->n_terminals)
	{	
	case 1:
	case 2:
		send->terminal_byte_1 = stream[22];
		send->terminal_byte_2 = stream[23];
		break;

	case 0:
	default:
		send->terminal_byte_1 = 0x00;
		send->terminal_byte_2 = 0x00;
	}
	switch (send->msg_type)
	{
	case SEND_MSG_TYPE_MODBUS:
		//TODO!!!
		encsize = 28;
		break;
	
	case SEND_MSG_TYPE_DYNAMIC:
		clickplc_reg_dec(&stream[24],&send->dynamic_msg_source_ref);
		if (send->is_const_length_of_dynamic)
		{
			clickplc_int16_dec(&stream[26],&send->dynamic_msg_const_length);
		}
		else
		{
			clickplc_reg_dec(&stream[26],&send->dynamic_msg_length_ref);
		}
		encsize = 28;
		break;
	
	case SEND_MSG_TYPE_STATIC:
	default:
		clickplc_int16_dec(&stream[24],&send->static_msg_length);
		memcpy(send->static_msg,&stream[26],send->static_msg_length);
		encsize = 26 + send->static_msg_length;
		if (encsize & 0x03)
		{
			encsize += 4-(encsize & 0x03);
		}
	}
	return(encsize);
}

/*

#########################################################################################
Receive ASCII:

00		1 byte{0x10}
01		1 byte{data length type: fixed(0x02), variable(0x01), modbus(0x00)}
02		1 byte{0x00 meaning ?!}
03		1 byte{port index: 0,1,2}
04		2 bytes{0xff,0xff}
06		2 bytes{dest TXT-reg}
08		2 bytes{success bit reg}
10		2 bytes{receiving bit reg}
12		2 bytes{0xff,0xff}
14		2 bytes{Overflow bit reg}
16		2 bytes{InterTimeoutErr bit reg}
18		2 bytes{FirstTimeoutErr bit reg}

fixed:
20		1 byte{0x00}
21		1 byte{byte swap: no(0x00), all(0x01), all but nul(0x02)}
22		2 bytes{length holding DS reg}
24		2 bytes{Interval character Timeout: 0 - none, 1 - 100 ms, 2 - 200 ms..., max 5 - 500 ms}
26		2 bytes{First character Timeout: 0 - none, 1 - 100 ms, 2 - 200 ms..., max 5 - 500 ms}

variable:
20		1 byte{byte swap: no(0x00), all(0x01), all but nul(0x02)}
21		1 byte{number of terminal bytes: 0,1,2}
22		1 byte{first terminal byte or 0x00}
23		1 byte{second terminal byte or 0x00}
24		2 bytes{Interval character Timeout: 0 - none, 1 - 100 ms, 2 - 200 ms..., max 5 - 500 ms}
26		2 bytes{First character Timeout: 0 - none, 1 - 100 ms, 2 - 200 ms..., max 5 - 500 ms}

__________________
Total: 28 bytes.
__________________

Examples:

fixed; port=2; timeouts: no; byte swap: no; dest.: TXT1; length: DS7; bits: C1,C2,C3,C4,C5

10 02	00	01	ff ff	70 00	d0 01	d0 00	ff ff	d0 04	d0 03	d0 02	00	00 	00 06 		00 00 00 00 

fixed; port=2; timeouts: no; byte swap: all; dest.: TXT1; length: DS1; bits: C1,C2,C3,C4,C5

10 02	00	01	ff ff	70 00	d0 01	d0 00	ff ff	d0 04	d0 03	d0 02	00	01	00 00		00 00 00 00 

fixed; port=2; timeouts: no; byte swap: all but nul; dest.: TXT1; length: DS1; bits: C1,C2,C3,C4,C5

10 02	00	01	ff ff	70 00	d0 01	d0 00	ff ff	d0 04	d0 03	d0 02	00	02	00 00		00 00 00 00 

fixed; port=2; timeouts: 200ms,100ms; byte swap: no; dest.: TXT1; length: DS1; bits: C1,C2,C3,C4,C5

10 02	00	01	ff ff	70 00	d0 01	d0 00	ff ff	d0 04	d0 03	d0 02	00	00	00 00		00 02 00 01 

fixed; port=2; timeouts: no,300ms; byte swap: all; dest.: TXT1; length: DS1; bits: C1,C2,C3,C4,C5

10 02	00	01	ff ff	70 00	d0 01	d0 00	ff ff	d0 04	d0 03	d0 02	00	01	00 00		00 00 00 03 

--------

variable; port=2; timeouts: 200ms,100ms; byte swap: no; dest.: TXT1; terminals: 2=0x0a,0x0d; bits: C1,C2,C3,C4,C5

10 01	00	01	ff ff	70 00	d0 01	d0 00	ff ff	d0 04	d0 03	d0 02	00	02 0a 0d	00 02 00 01 

variable; port=2; timeouts: no; byte swap: all; dest.: TXT1; terminals: 1=0x0a; bits: C1,C2,C3,C4,C5

10 01	00 01	ff ff	70 00	d0 01	d0 00	ff ff	d0 04	d0 03	d0 02	01	01 0a 00	00 00 00 00 

variable; port=2; timeouts: no; byte swap: all; dest.: TXT1; terminals: 1=0x0a; bits: C1,C2,C3,C4,,

10 01	00	01	ff ff	70 00	d0 01	d0 00	ff ff	ff ff	d0 03	d0 02	01	01 0a 00	00 00 00 00 

variable; port=2; timeouts: no; byte swap: all; dest.: TXT1; terminals: 1=0x0a; bits: ,C2,,,C5

10 01	00	01	ff ff	70 00	d0 01	ff ff	ff ff	d0 04	ff ff	ff ff	01	01 0a 00	00 00 00 00 

variable; port=2; timeouts: 200ms,100ms; byte swap: all; dest.: TXT1; terminals: 1=0x0a; bits: ,,,,,

10 01	00	01	ff ff	70 00	ff ff	ff ff	ff ff	ff ff	ff ff	ff ff	01	01 0a 00	00 02 00 01 

variable; port=2; timeouts: no; byte swap: all but nul; dest.: TXT1; terminals: 1=0x0a; bits: ,,,,,

10 01	00	01	ff ff	70 00	ff ff	ff ff	ff ff	ff ff	ff ff	ff ff	02	01 0a 00	00 00 00 00 

variable; port=2; timeouts: 200ms,100ms; byte swap: all but nul; dest.: TXT1; terminals: 1=0x0a; bits: ,,,,,

10 01	00	01	ff ff	70 00	ff ff	ff ff	ff ff	 ff ff	ff ff	ff ff	02	01 0a 00	00 02 00 01 

Modbus:

modbus 984 addressing
					Master: Y1	success	rcv		err	 ErCode:DS1				slave ID|mfc| 	  | n-bits	 | slave addr.
10 00	00 01	ff ff	c4 00	d0 01	d0 00	d0 02		00 00	ff ff ff ff		01 01 00 10 ff ff 00 02 
								placeholder|														  
10 00	00 01	ff ff	c4 00	d0 01	ff ff	d0 02		00 00	ff ff ff ff		64 01 00 10 ff ff 00 02
																									  [3]
10 00	00 01	ff ff	c4 00	d0 01	ff ff	d0 02		ff ff	ff ff ff ff		64 01 00 10 ff ff 00 02
																									  
					Master: Y2							ErCode:DD1									  [7]
10 00	00 01	ff ff	c4 01	d0 01	d0 00	d0 02		11 94	ff ff ff ff		01 01 00 0f ff ff 00 06				[000007]
																						|mfc=1			000001...065535	[000003]
10 00	00 01	ff ff	c4 00	d0 01	d0 00	d0 02		00 00	ff ff ff ff		01 01 00 10 ff ff 00 02 
																						|mfc=2			100001...165535	[100001]
10 00	00 01	ff ff	c4 00	d0 01	d0 00	d0 02		11 94	ff ff ff ff		64 02 00 10 ff ff 00 00	
						[ds101]															|mfc=3			400001...465535	[400001]
10 00	00 01	ff ff	00 64	d0 01	d0 00	d0 02		11 94	ff ff ff ff		64 03 00 20 ff ff 00 00
																							 No. of master addresses=32(=0x20),Word swap = OFF;

						[ds101]															|mfc=3			400001...465535	[400001]
10 00	00 01	ff ff	00 64	d0 01	d0 00	d0 02		11 94	ff ff ff ff		64 03 00 20 ff [fe] [00 00]
																							No. of master addresses=32(=0x20),Word swap = [ON];
						[df1]															|mfc=4			300001...365535	[300001]
10 00	00 01	ff ff	1b 58	d0 01	d0 00	d0 02		11 94	ff ff ff ff		64 04 00 7c ff fe 00 00
																							No. of master addresses=62(=0x7c),Word swap = [ON];

modbus hex addressing
																									  [3 hex]
10 00	00 01	ff ff	c4 00	d0 01	ff ff	d0 02		ff ff	ff ff ff ff		64 01 00 10 ff ff 00 03
10 00	00 01	ff ff	c4 00	d0 01	d0 00	d0 02		11 94	ff ff ff ff		64 01 00 0e ff ff 00 03

click addressing
																									  [CT1]
10 00	00 01	ff ff	c4 00	d0 01	d0 00	d0 02		11 94	ff ff ff ff		64 01 00 0e ff ff c0 00 


########################################################################################################################################################
When the CLICK PLC is used as a MODBUS Slave and could not handle the message from the MODBUS Master, 
it returns an Exception Response (Exception Code) to the MODBUS Master.

Exception Response (Exception Code)
The CLICK PLC supports the following Exception Responses:

01		Illegal Function		The CLICK PLC does not support the function code received from the MODBUS Master.

02		Illegal Data Address	The MODBUS Master tried to access to an invalid Address.

03		Illegal Data Value		The Data Length is Zero or exceeds the maximum size.
								The Data for Write Single Coil is not FF00h (ON) or 0000h (OFF).
								The PLC mode change request from the MODBUS Master is not valid.
04		Slave Device Failure	Password is locked. 
								When the PLC mode switch is in the STOP position, the MODBUS Master requested to switch to RUN mode.
								When the PLC mode switch is in the RUN position, the MODBUS Master requested to switch to the Firmware Update mode.
########################################################################################################################################################

*/

/*		
	return: encoded size !=0, or error=0.
*/	
int clickplc_exe_Receive_enc(void* obj,unsigned char* stream)
{
	int encsize = 28;
	EXE_Receive* receive = (EXE_Receive*)obj;
	
	stream[0] = EXE_CODE_INSTRUCTION_RECEIVE;
	stream[1] = receive->data_type;
	stream[2] = 0x00;
	stream[3] = receive->port_index;
	stream[4] = 0xff;
	stream[5] = 0xff;
	
	clickplc_reg_enc(&receive->dest_ref,		&stream[6]);
	
	if(receive->is_success_bit_used)
	{
		clickplc_reg_enc(&receive->success_bit_ref,	&stream[8]);		
	}
	else
	{
		stream[8] = 0xff;
		stream[9] = 0xff;
	}

	if(receive->is_receiving_bit_used)
	{
		clickplc_reg_enc(&receive->receiving_bit_ref,	&stream[10]);
	}
	else
	{
		stream[10] = 0xff;
		stream[11] = 0xff;
	}
	
	stream[12] = 0xff;
	stream[13] = 0xff;
	
	if(receive->is_overflow_bit_used)
	{
		clickplc_reg_enc(&receive->overflow_bit_ref,	&stream[14]);
	}
	else
	{
		stream[14] = 0xff;
		stream[15] = 0xff;
	}

	if(receive->is_intertimeout_bit_used)
	{
		clickplc_reg_enc(&receive->intertimeout_bit_ref,	&stream[16]);
	}
	else
	{
		stream[16] = 0xff;
		stream[17] = 0xff;
	}
	
	if(receive->is_firsttimeout_bit_used)
	{
		clickplc_reg_enc(&receive->firsttimeout_bit_ref,	&stream[18]);
	}
	else
	{
		stream[18] = 0xff;
		stream[19] = 0xff;
	}

	switch (receive->data_type)
	{
	case RECEIVE_TYPE_MODBUS:
		//TODO!!!
		break;

	case RECEIVE_TYPE_VARIABLE:
		stream[20] = receive->byte_swap;
		stream[21] = receive->variable_n_terminals;
		switch (receive->variable_n_terminals)
		{		
		case 1:
			stream[22] = receive->variable_terminal_byte_1;
			stream[23] = RECEIVE_ASCII_TIMEOUTS_NONE;
			break;
		case 2:
			stream[22] = receive->variable_terminal_byte_1;
			stream[23] = receive->variable_terminal_byte_2;
			break;

		case 0:
		default:
			stream[22] = RECEIVE_ASCII_TIMEOUTS_NONE;
			stream[23] = RECEIVE_ASCII_TIMEOUTS_NONE;
		}
		clickplc_int16_enc(receive->intertimeout,&stream[24]);
		clickplc_int16_enc(receive->firsttimeout,&stream[26]);		
		break;

	case RECEIVE_TYPE_FIXED:
	default:		
		stream[20] = 0x00;
		stream[21] = receive->byte_swap;
		clickplc_reg_enc(&receive->fixed_length_holding_ref,	&stream[22]);
		clickplc_int16_enc(receive->intertimeout,&stream[24]);
		clickplc_int16_enc(receive->firsttimeout,&stream[26]);
		break;
	}
	
	return(encsize);
}

int clickplc_exe_Receive_dec(unsigned char* stream,void* obj)
{
	int encsize = 28;
	EXE_Receive* receive = (EXE_Receive*)obj;
	
	receive->data_type = stream[1];
	receive->port_index = stream[3];
	
	clickplc_reg_dec(&stream[6],	&receive->dest_ref);
	
	if ( (stream[8]==0xff) && (stream[9]==0xff) )
	{
		receive->is_success_bit_used = 0;
	}
	else
	{
		receive->is_success_bit_used = 1;
	}	
	if(receive->is_success_bit_used)
	{
		clickplc_reg_dec(&stream[8],	&receive->success_bit_ref);
	}
	
	if ( (stream[10]==0xff) && (stream[11]==0xff) )
	{
		receive->is_receiving_bit_used = 0;
	}
	else
	{
		receive->is_receiving_bit_used = 1;
	}
	if(receive->is_receiving_bit_used)
	{
		clickplc_reg_dec(&stream[10],	&receive->receiving_bit_ref);
	}
	
	if ( (stream[14]==0xff) && (stream[15]==0xff) )
	{
		receive->is_overflow_bit_used = 0;
	}
	else
	{
		receive->is_overflow_bit_used = 1;
	}
	if(receive->is_overflow_bit_used)
	{
		clickplc_reg_dec(&stream[14],&receive->overflow_bit_ref);
	}
	
	if ( (stream[16]==0xff) && (stream[17]==0xff) )
	{
		receive->is_intertimeout_bit_used = 0;
	}
	else
	{
		receive->is_intertimeout_bit_used = 1;
	}
	if(receive->is_intertimeout_bit_used)
	{
		clickplc_reg_dec(&stream[16],&receive->intertimeout_bit_ref);
	}
	
	if ( (stream[18]==0xff) && (stream[19]==0xff) )
	{
		receive->is_firsttimeout_bit_used = 0;
	}
	else
	{
		receive->is_firsttimeout_bit_used = 1;
	}
	if(receive->is_firsttimeout_bit_used)
	{
		clickplc_reg_dec(&stream[18],	&receive->firsttimeout_bit_ref);
	}
	
	switch (receive->data_type)
	{
	case RECEIVE_TYPE_MODBUS:
		//TODO!!!
		break;
		
	case RECEIVE_TYPE_VARIABLE:
		receive->byte_swap				= stream[20];
		receive->variable_n_terminals	= stream[21];
		switch (receive->variable_n_terminals)
		{		
		case 1:
			receive->variable_terminal_byte_1 = stream[22];
			receive->variable_terminal_byte_2 = RECEIVE_ASCII_TIMEOUTS_NONE;
			break;

		case 2:
			receive->variable_terminal_byte_1 = stream[22];
			receive->variable_terminal_byte_2 = stream[23];
			break;
			
		case 0:
		default:
			receive->variable_terminal_byte_1 = RECEIVE_ASCII_TIMEOUTS_NONE;
			receive->variable_terminal_byte_2 = RECEIVE_ASCII_TIMEOUTS_NONE;
		}
		clickplc_int16_dec(&stream[24],&receive->intertimeout);
		clickplc_int16_dec(&stream[26],&receive->firsttimeout);		
		break;
		
	case RECEIVE_TYPE_FIXED:
	default:
		
		receive->byte_swap = stream[21];
		clickplc_reg_dec(&stream[22],&receive->fixed_length_holding_ref);
		clickplc_int16_dec(&stream[24],&receive->intertimeout);
		clickplc_int16_dec(&stream[26],&receive->firsttimeout);
		break;
	}
	
	return(encsize);
}

/*
IF	01 
ON	00 c0 00 
AND	02 
==	10 00 11 ff ff 00 00 ff ff 00 01 
IF	01 
==	10 00 11 ff ff 00 02 ff ff 00 03 
AND	02 
>	12 00 11 ff ff 00 04 ff ff 00 05 
Connect_2_Upper	00 03 ff ff 

IF	01 
ON	00 c0 01 

AND	02 
UP	04 c0 02 

Connect_2_Upper	00 03 ff ff 

IF	01 
OFF	02 c0 03 

AND	02 
<	14 00 11 ff ff 00 05 ff ff 00 06 

Connect_2_Upper	00 03 ff ff 

IF	01 
!=	11 00 11 ff ff 00 09 ff ff 00 0b 

AND	02 
ON	00 c0 07 

Connect_2_Upper	00 03 ff ff 

IF	01 
UP	04 d0 00 

AND	02 
DOWN	05 d0 01 

IF	01 
<	14 00 21 ff ff 11 9c ff ff 00 0a		dd5 < 10 

IF	01 
<	14 00 12 ff ff 00 0a ff ff 11 9c		10 < dd5


IF	01 
<	14 00 12 ff ff 00 0a ff ff 11 9c		10 < dd5
AND	02 
<	14 00 22 ff ff 00 0a ff ff 00 07		10 < 7
AND	02 
>	12 00 32 ff ff 03 e8 00 01 86 a0		1000 < 100 000 

IF	01 
>=	13 00 21 ff ff 11 9c ff ff 00 0a		dd5 >= 10
AND	02 
<	14 00 21 ff ff 11 9c ff ff 00 64		dd5 < 100

IF	01 
>=	13 00 21 ff ff 11 9c ff ff 00 64		dd5 >= 100
AND	02 
<	14 00 21 ff ff 11 9c ff ff 03 e8		dd5 < 1000

IF	01 
>=	13 00 21 ff ff 11 9c ff ff 03 e8		dd5 >= 1000
AND	02 
<	14 00 21 ff ff 11 9c ff ff 27 10		dd5 < 10 000

IF	01 
>=	13 00 21 ff ff 11 9c ff ff 27 10		dd5 >= 10 000
AND	02 
<	14 00 31 ff ff 11 9c 00 01 86 a0		dd5 < 100 000

IF	01 
>=	13 00 31 ff ff 11 9c 00 01 86 a0		dd5 >= 100 000
AND	02 
<	14 00 31 ff ff 11 9c 00 0f 42 40		dd5 < 1 000 000

IF	01 
>=	13 00 13 00 01 86 a0 ff ff 11 9c		100 000 >= dd5
AND	02 
<	14 00 13 00 0f 42 40 ff ff 11 9c		1 000 000 < dd5

*/

#define EXE_CODE_REGISTER	0x01
#define EXE_CODE_CONSTINT16	0x02
#define EXE_CODE_CONSTINT32	0x03

int clickplc_exe_Condition_enc			(void* obj,unsigned char* stream)
{
	int encsize = 0;
	EXE_Condition* condition = (EXE_Condition*)obj;

	stream[0] = condition->cond_logic;//IF,AND,OR

	switch(condition->cond_type)
	{
	case EXE_CODE_SUBINSTR_ON:
	//case (EXE_CODE_SUBINSTR_ON|EXE_CODE_SUBINSTR_MODIFIER_IMMEDIATE)://ON/OFF for X only
	case EXE_CODE_SUBINSTR_OFF:					
	//case (EXE_CODE_SUBINSTR_OFF|EXE_CODE_SUBINSTR_MODIFIER_IMMEDIATE)://ON/OFF for X only
	case EXE_CODE_SUBINSTR_UP:		//no modifiers
	case EXE_CODE_SUBINSTR_DOWN:	//no modifiers		
		stream[1] = condition->cond_type|condition->is_immediate;
		clickplc_reg_enc(&condition->bit_reg_ref,&stream[2]);//[2,3]
		encsize = 4;
		break;

	case EXE_CODE_SUBINSTR_COMPARE_EQUAL:
	case EXE_CODE_SUBINSTR_COMPARE_NOTEQUAL:
	case EXE_CODE_SUBINSTR_COMPARE_MORE:
	case EXE_CODE_SUBINSTR_COMPARE_MOREOREQUAL:
	case EXE_CODE_SUBINSTR_COMPARE_LESS:
	case EXE_CODE_SUBINSTR_COMPARE_LESSOREQUAL:

		stream[1] = condition->cond_type;
		stream[2] = 0x00;

		if (condition->is_A_const)
		{
			int tmp;

			tmp = (condition->cmp_A_const < 0)? -condition->cmp_A_const : condition->cmp_A_const;

			if (tmp < 0x10000)
			{	//16-bit
				stream[3] = EXE_CODE_CONSTINT16;

				stream[4] = 0xff;
				stream[5] = 0xff;

				clickplc_int16_enc((unsigned short)condition->cmp_A_const,&stream[6]);//[6,7]

			}
			else
			{	//32-bit
				stream[3] = EXE_CODE_CONSTINT32;

				clickplc_int32_enc((unsigned int)condition->cmp_A_const,&stream[4]);//[4,5,6,7]
			}
		}
		else
		{
			stream[3] = EXE_CODE_REGISTER;

			stream[4] = 0xff;
			stream[5] = 0xff;

			clickplc_reg_enc(&condition->cmp_A_reg_ref,&stream[6]);//[6,7]

		}

		if (condition->is_B_const)
		{
			int tmp;

			tmp = (condition->cmp_B_const < 0)? -condition->cmp_B_const : condition->cmp_B_const;

			if (tmp < 0x10000)
			{	//16-bit
				stream[3] |= EXE_CODE_CONSTINT16<<4;

				stream[8] = 0xff;
				stream[9] = 0xff;

				clickplc_int16_enc((unsigned short)condition->cmp_B_const,&stream[10]);//[10,11]
			}
			else
			{	//32-bit
				stream[3] |= EXE_CODE_CONSTINT32<<4;

				clickplc_int32_enc((unsigned int)condition->cmp_B_const,&stream[8]);//[8,9,10,11]
			}
		}
		else
		{
			stream[3] |= EXE_CODE_REGISTER<<4;

			stream[8] = 0xff;
			stream[9] = 0xff;

			clickplc_reg_enc(&condition->cmp_B_reg_ref,&stream[10]);//[10,11]
		}

		encsize = 12;
		break;
	}

	return(encsize);
}
int clickplc_exe_Condition_dec			(unsigned char* stream,void* obj)
{
	int encsize = 0;
	EXE_Condition* condition = (EXE_Condition*)obj;

	condition->cond_logic = stream[0];//IF,AND,OR

	if (stream[1] < EXE_CODE_SUBINSTR_COMPARE_EQUAL)
	{
		switch(stream[1])
		{
		case EXE_CODE_SUBINSTR_ON:
		case (EXE_CODE_SUBINSTR_ON|EXE_CODE_SUBINSTR_MODIFIER_IMMEDIATE)://ON/OFF for X only
		case EXE_CODE_SUBINSTR_OFF:					
		case (EXE_CODE_SUBINSTR_OFF|EXE_CODE_SUBINSTR_MODIFIER_IMMEDIATE)://ON/OFF for X only
			condition->cond_type	= stream[1]&0xfe;
			condition->is_immediate = stream[1]&0x01;
			break;
		
		case EXE_CODE_SUBINSTR_UP:		//no modifiers
		case EXE_CODE_SUBINSTR_DOWN:	//no modifiers		
			condition->cond_type	= stream[1];
			condition->is_immediate = 0;
			break;
		}
		
		clickplc_reg_dec(&stream[2],&condition->bit_reg_ref);//[2,3]
		encsize = 4;
	}
	else
	{
		switch(stream[1])
		{
		case EXE_CODE_SUBINSTR_COMPARE_EQUAL:
		case EXE_CODE_SUBINSTR_COMPARE_NOTEQUAL:
		case EXE_CODE_SUBINSTR_COMPARE_MORE:
		case EXE_CODE_SUBINSTR_COMPARE_MOREOREQUAL:
		case EXE_CODE_SUBINSTR_COMPARE_LESS:
		case EXE_CODE_SUBINSTR_COMPARE_LESSOREQUAL:

			condition->cond_type = stream[1];
			
			condition->is_A_const = ( (stream[3]&0x0f) ==  EXE_CODE_REGISTER )? 0:1;
			condition->is_B_const = ( (stream[3]&0xf0) == (EXE_CODE_REGISTER<<4) )? 0:1;

			if (condition->is_A_const)
			{
				switch (stream[3]&0x0f)
				{
				case EXE_CODE_CONSTINT32:
					{	//32-bit
						unsigned int tmp32;
						clickplc_int32_dec(&stream[4],&tmp32);//[4,5,6,7]
						condition->cmp_A_const = (int)tmp32;
					}
					break;

				default:
				case EXE_CODE_CONSTINT16:
					{	//16-bit
						unsigned short tmp16;
						clickplc_int16_dec(&stream[6],&tmp16);//[6,7]
						condition->cmp_A_const = (int)tmp16;
					}
					break;
				}
			}
			else
			{
				clickplc_reg_dec(&stream[6],&condition->cmp_A_reg_ref);//[6,7]
			}

			if (condition->is_B_const)
			{
				switch (stream[3]&0xf0)
				{
				case EXE_CODE_CONSTINT32<<4:
					{	//32-bit
						unsigned int tmp32;
						clickplc_int32_dec(&stream[8],&tmp32);//[8,9,10,11]
						condition->cmp_B_const = (int)tmp32;
					}
					break;

				default:
				case EXE_CODE_CONSTINT16<<4:
					{	//16-bit
						unsigned short tmp16;
						clickplc_int16_dec(&stream[10],&tmp16);//[10,11]
						condition->cmp_B_const = (int)tmp16;
					}
					break;
				}
			}
			else
			{
				clickplc_reg_dec(&stream[10],&condition->cmp_B_reg_ref);//[10,11]
			}

			encsize = 12;
			break;
		}
	}

	return(encsize);
}

//0x00 0x03 0xff 0xff
int clickplc_exe_Connect2Upper_enc		(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_CONNECT_CONTACTS;
	stream[2] = 0xff;
	stream[3] = 0xff;
	return(encsize);
}
int clickplc_exe_Connect2Upper_dec		(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}

//0x00 0x07 0xff 0xff	used as a placeholder for unconditional connection
int clickplc_exe_ConnectUncond_enc		(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_CONNECT_UNCONDITIONALLY;
	stream[2] = 0xff;
	stream[3] = 0xff;
	return(encsize);
}
int clickplc_exe_ConnectUncond_dec		(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}

//EXE_CODE_CNTRL_SUBPROGRAMM_RETURN				0x18		//00 0x18 0xff 0xff
int clickplc_exe_SubProgrammReturn_enc			(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_SUBPROGRAMM_RETURN;
	stream[2] = 0xff;
	stream[3] = 0xff;
	return(encsize);
}
int clickplc_exe_SubProgrammReturn_dec			(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}
//EXE_CODE_CNTRL_INTERRUPT_RETURN				0x19		//00 0x19 0xff 0xff
int clickplc_exe_InterruptReturn_enc			(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = 0x00;
	stream[1] = EXE_CODE_CNTRL_INTERRUPT_RETURN;
	stream[2] = 0xff;
	stream[3] = 0xff;
	return(encsize);
}
int clickplc_exe_InterruptReturn_dec			(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}


//EXE_CODE_INSTRUCTION_EOF				0xff	//0xff,0xff,0xff,0xff
int clickplc_exe_EOF_enc			(void* obj,unsigned char* stream)
{
	int encsize = 4;
	stream[0] = EXE_CODE_INSTRUCTION_EOF;
	stream[1] = EXE_CODE_INSTRUCTION_EOF;
	stream[2] = EXE_CODE_INSTRUCTION_EOF;
	stream[3] = EXE_CODE_INSTRUCTION_EOF;
	return(encsize);
}
int clickplc_exe_EOF_dec			(unsigned char* stream,void* obj)
{
	int encsize = 4;
	return(encsize);
}

/*
COILS (OUT,SET,RST)

#define EXE_CODE_SUBINSTR_COIL_OUT					0x00
#define EXE_CODE_SUBINSTR_COIL_SET					0x10
#define EXE_CODE_SUBINSTR_COIL_RST					0x20
#define EXE_CODE_SUBINSTR_COIL_MODIFIER_IMMEDIATE		0x01	// SET/RST for Y only
#define EXE_CODE_SUBINSTR_COIL_MODIFIER_1TIME			0x02
#define EXE_CODE_SUBINSTR_COIL_MODIFIER_RANGE			0x04

04 04 d0 00 ff ff d0 04		out c1-c5

04 06 d0 00 ff ff d0 04		out c1-c5, 1s

04 04 c4 00 ff ff c4 02		out y1-y3, 

04 06 c4 00 ff ff c4 02		out y1-y3, 1s

04 07 c4 00 ff ff c4 02		out y1-y3, 1s, imm

04 14 c4 00 ff ff c4 02		set y1-y3

04 15 c4 00 ff ff c4 02		set y1-y3, imm

04 24 c4 00 ff ff c4 02		rst y1-y3

04 25 c4 00 ff ff c4 02		rst y1-y3, imm

04 00 d0 00					out c1

04 10 d0 00					set c1

04 20 d0 00					rst c1

*/

#define EXE_CODE_SUBINSTR_COIL_MODIFIER_IMMEDIATE		0x01	// SET/RST for Y only
#define EXE_CODE_SUBINSTR_COIL_MODIFIER_1TIME			0x02
#define EXE_CODE_SUBINSTR_COIL_MODIFIER_RANGE			0x04

//OUT	04 00 c4 00 
int clickplc_exe_OUT_enc				(void* obj,unsigned char* stream)
{
	int encsize;
	EXE_Out* coil = (EXE_Out*)obj;
	
	stream[0] = EXE_CODE_INSTRUCTION_COIL;
	stream[1] = EXE_CODE_SUBINSTR_COIL_OUT;
		
	if (coil->is_one_shot)	stream[1] |= EXE_CODE_SUBINSTR_COIL_MODIFIER_1TIME;
	if (coil->is_immediate)	stream[1] |= EXE_CODE_SUBINSTR_COIL_MODIFIER_IMMEDIATE;

	if (coil->is_range)
	{
		stream[1] |= EXE_CODE_SUBINSTR_COIL_MODIFIER_RANGE;

		clickplc_reg_enc(&coil->first_bit_reg_ref,&stream[2]);//2,3
		stream[4] = 0xff;
		stream[5] = 0xff;
		clickplc_reg_enc(&coil->last_bit_reg_ref,&stream[6]);//6,7
		encsize = 8;
	}
	else
	{
		clickplc_reg_enc(&coil->first_bit_reg_ref,&stream[2]);//2,3
		encsize = 4;
	}
	
	return(encsize);
}
int clickplc_exe_OUT_dec				(unsigned char* stream,void* obj)
{
	int encsize;
	EXE_Out* coil = (EXE_Out*)obj;
	
	if (stream[1] & EXE_CODE_SUBINSTR_COIL_MODIFIER_1TIME)		coil->is_one_shot = 1;
	else														coil->is_one_shot = 0;

	if (stream[1] & EXE_CODE_SUBINSTR_COIL_MODIFIER_IMMEDIATE)	coil->is_immediate = 1;
	else														coil->is_immediate = 0;

	if (stream[1] & EXE_CODE_SUBINSTR_COIL_MODIFIER_RANGE)
	{
		coil->is_range = 1;
		clickplc_reg_dec(&stream[2],&coil->first_bit_reg_ref);//2,3
		clickplc_reg_dec(&stream[6],&coil->last_bit_reg_ref);//6,7
		encsize = 8;
	}
	else
	{
		coil->is_range = 0;
		clickplc_reg_dec(&stream[2],&coil->first_bit_reg_ref);//2,3
		encsize = 4;
	}
	return(encsize);
}

int clickplc_exe_SET_enc				(void* obj,unsigned char* stream)
{
	int encsize = 0;
	EXE_Set_Rst* coil = (EXE_Set_Rst*)obj;

	stream[0] = EXE_CODE_INSTRUCTION_COIL;
	stream[1] = EXE_CODE_SUBINSTR_COIL_SET;
		
	if (coil->is_immediate)	stream[1] |= EXE_CODE_SUBINSTR_COIL_MODIFIER_IMMEDIATE;

	if (coil->is_range)
	{
		stream[1] |= EXE_CODE_SUBINSTR_COIL_MODIFIER_RANGE;

		clickplc_reg_enc(&coil->first_bit_reg_ref,&stream[2]);//2,3
		stream[4] = 0xff;
		stream[5] = 0xff;
		clickplc_reg_enc(&coil->last_bit_reg_ref,&stream[6]);//6,7
		encsize = 8;
	}
	else
	{
		clickplc_reg_enc(&coil->first_bit_reg_ref,&stream[2]);//2,3
		encsize = 4;
	}

	return(encsize);
}
int clickplc_exe_RST_enc				(void* obj,unsigned char* stream)
{
	int encsize = 0;
	EXE_Set_Rst* coil = (EXE_Set_Rst*)obj;

	stream[0] = EXE_CODE_INSTRUCTION_COIL;
	stream[1] = EXE_CODE_SUBINSTR_COIL_RST;

	if (coil->is_immediate)	stream[1] |= EXE_CODE_SUBINSTR_COIL_MODIFIER_IMMEDIATE;

	if (coil->is_range)
	{
		stream[1] |= EXE_CODE_SUBINSTR_COIL_MODIFIER_RANGE;

		clickplc_reg_enc(&coil->first_bit_reg_ref,&stream[2]);//2,3
		stream[4] = 0xff;
		stream[5] = 0xff;
		clickplc_reg_enc(&coil->last_bit_reg_ref,&stream[6]);//6,7
		encsize = 8;
	}
	else
	{
		clickplc_reg_enc(&coil->first_bit_reg_ref,&stream[2]);//2,3
		encsize = 4;
	}

	return(encsize);
}

int clickplc_exe_SET_RST_dec				(unsigned char* stream,void* obj)
{
	int encsize = 0;
	EXE_Set_Rst* coil = (EXE_Set_Rst*)obj;

	if (stream[1] & EXE_CODE_SUBINSTR_COIL_MODIFIER_IMMEDIATE)	coil->is_immediate = 1;
	else														coil->is_immediate = 0;

	if (stream[1] & EXE_CODE_SUBINSTR_COIL_MODIFIER_RANGE)
	{
		coil->is_range = 1;
		clickplc_reg_dec(&stream[2],&coil->first_bit_reg_ref);//2,3
		clickplc_reg_dec(&stream[6],&coil->last_bit_reg_ref);//6,7
		encsize = 8;
	}
	else
	{
		coil->is_range = 0;
		clickplc_reg_dec(&stream[2],&coil->first_bit_reg_ref);//2,3
		encsize = 4;
	}

	return(encsize);
}

void*	clickplc_exe_object_calloc		(int object_code)
{
	void* obj = 0;

	switch(object_code)
	{
	case CLICK_EXE_Codes_Condition:	
		obj = calloc(1,sizeof(EXE_Condition));
		break;


	case CLICK_EXE_Codes_Marker_Connect2UpperContacts:
	case CLICK_EXE_Codes_Marker_ConnectUnconditionally:
		//nothing
		break;

	case CLICK_EXE_Codes_Marker_SubProgrammStart:
		obj = calloc(1,sizeof(EXE_SubProgrammStart));
		break;
	case CLICK_EXE_Codes_Marker_InterruptStart:
		obj = calloc(1,sizeof(EXE_InterruptStart));
		break;
	case CLICK_EXE_Codes_Instruction_SubProgrammReturn:
	case CLICK_EXE_Codes_Instruction_InterruptReturn:
	case CLICK_EXE_Codes_Marker_EOF:
		//nothing
		break;

	case CLICK_EXE_Codes_Instruction_Out:
		obj = calloc(1,sizeof(EXE_Out));
		break;

	case CLICK_EXE_Codes_Instruction_Set:
	case CLICK_EXE_Codes_Instruction_Rst:
		obj = calloc(1,sizeof(EXE_Set_Rst));
		break;

	case CLICK_EXE_Codes_Instruction_ShiftReg:
		obj = calloc(1,sizeof(EXE_ShiftRegister));
		break;
	case CLICK_EXE_Codes_Instruction_Copy:
		obj = calloc(1,sizeof(EXE_Copy));
		break;
	case CLICK_EXE_Codes_Instruction_Search:
		obj = calloc(1,sizeof(EXE_Search));
		break;
	case CLICK_EXE_Codes_Instruction_Timer:
		obj = calloc(1,sizeof(EXE_Timer));
		break;
	case CLICK_EXE_Codes_Instruction_Counter:
		obj = calloc(1,sizeof(EXE_Counter));
		break;
	case CLICK_EXE_Codes_Instruction_Send:
		obj = calloc(1,sizeof(EXE_Send));
		break;
	case CLICK_EXE_Codes_Instruction_Receive:
		obj = calloc(1,sizeof(EXE_Receive));
		break;
	case CLICK_EXE_Codes_Instruction_Drum:
		obj = calloc(1,sizeof(EXE_Drum));
		break;
	case CLICK_EXE_Codes_Instruction_Call:
		obj = calloc(1,sizeof(EXE_Call));
		break;
	case CLICK_EXE_Codes_Marker_EndOfSubProgramm:
	case CLICK_EXE_Codes_Marker_EndOfInterrupt:
		//nothing
		break;
	case CLICK_EXE_Codes_Instruction_Math:
		obj = calloc(1,sizeof(EXE_Math));
		break;
	case CLICK_EXE_Codes_Instruction_For:
		obj = calloc(1,sizeof(EXE_For));
		break;

	case CLICK_EXE_Codes_Instruction_Next:
	case CLICK_EXE_Codes_Instruction_EndOfMain:
		//nothing
		break;

	default:
	case CLICK_EXE_Codes_DummyEnd:
		//nothing
		break;
	}

	return(obj);
}

void	clickplc_exe_object_free		(int object_code,void** pobject)
{
	if (*pobject)
	{
		free(*pobject);
		*pobject = 0;
	}
	return;
}


char* get_clickplc_exe_codes_name(int code_id)
{
	char* str_name = "Unknown!!!";

	switch(code_id)
	{
	case CLICK_EXE_Codes_Condition:
		str_name = "Condition";
		break;
	case CLICK_EXE_Codes_Marker_Connect2UpperContacts:
		str_name = "Connect_2_Upper";
		break;
	case CLICK_EXE_Codes_Marker_ConnectUnconditionally:
		str_name = "***Always***";
		break;
	case CLICK_EXE_Codes_Marker_SubProgrammStart:
		str_name = "SUB.Start";
		break;
	case CLICK_EXE_Codes_Marker_InterruptStart:
		str_name = "INT.Start";
		break;
	case CLICK_EXE_Codes_Instruction_SubProgrammReturn:
		str_name = "SUB.RETURN";
		break;
	case CLICK_EXE_Codes_Instruction_InterruptReturn:
		str_name = "INT.RETURN";
		break;
	case CLICK_EXE_Codes_Marker_EOF:
		str_name = "EOF";
		break;
	case CLICK_EXE_Codes_Instruction_Out:
		str_name = "OUT";
		break;
	case CLICK_EXE_Codes_Instruction_Set:
		str_name = "SET";
		break;
	case CLICK_EXE_Codes_Instruction_Rst:
		str_name = "RST";
		break;
	case CLICK_EXE_Codes_Instruction_ShiftReg:
		str_name = "SHIFT_REG";
		break;
	case CLICK_EXE_Codes_Instruction_Copy:
		str_name = "COPY";
		break;
	case CLICK_EXE_Codes_Instruction_Search:
		str_name = "SEARCH";
		break;
	case CLICK_EXE_Codes_Instruction_Timer:
		str_name = "TIMER";
		break;
	case CLICK_EXE_Codes_Instruction_Counter:
		str_name = "COUNTER";
		break;
	case CLICK_EXE_Codes_Instruction_Send:
		str_name = "SEND";
		break;
	case CLICK_EXE_Codes_Instruction_Receive:
		str_name = "RECEIVE";
		break;
	case CLICK_EXE_Codes_Instruction_Drum:
		str_name = "DRUM";
		break;
	case CLICK_EXE_Codes_Instruction_Call:
		str_name = "CALL";
		break;
	case CLICK_EXE_Codes_Marker_EndOfSubProgramm:
		str_name = "SUB.END";
		break;
	case CLICK_EXE_Codes_Marker_EndOfInterrupt:
		str_name = "INT.END";
		break;
	case CLICK_EXE_Codes_Instruction_Math:
		str_name = "MATH";
		break;
	case CLICK_EXE_Codes_Instruction_For:
		str_name = "FOR";
		break;
	case CLICK_EXE_Codes_Instruction_Next:
		str_name = "NEXT";
		break;
	case CLICK_EXE_Codes_Instruction_EndOfMain:
		str_name = "MAIN END";
		break;

	default:
	case CLICK_EXE_Codes_DummyEnd:
		break;
	}

	return(str_name);
}

//returns some of CLICK_EXE_Codes, success != CLICK_EXE_Codes_DummyEnd.
int clickplc_exe_code_get(unsigned char* stream)
{
	int code_id = CLICK_EXE_Codes_DummyEnd;
	unsigned int stream_byte_0;
	unsigned int stream_byte_1;

	stream_byte_0 = stream[0];
	stream_byte_1 = stream[1];

	//first byte
	switch (stream_byte_0)
	{
	case EXE_CODE_CNTRL_PREFIX_0x00:

		//second byte
		switch (stream_byte_1)
		{
		case EXE_CODE_CNTRL_CONNECT_CONTACTS:		//0x00 0x03 0xff 0xff
			code_id = CLICK_EXE_Codes_Marker_Connect2UpperContacts;						
			break;

		case EXE_CODE_CNTRL_CONNECT_UNCONDITIONALLY://0x00 0x07 0xff 0xff
			code_id = CLICK_EXE_Codes_Marker_ConnectUnconditionally;						
			break;

		case EXE_CODE_CNTRL_SUBPROGRAMM_CALL:		//0x00,0x10,0xNN,0xNN	- subprogram #N call
			code_id = CLICK_EXE_Codes_Instruction_Call;						
			break;

		case EXE_CODE_CNTRL_SUBPROGRAMM_START:		//0x00,0x11,0xNN,0xNN	- subprogram #N start
			code_id = CLICK_EXE_Codes_Marker_SubProgrammStart;						
			break;

		case EXE_CODE_CNTRL_INTERRUPT_START:		//0x00,0x12,0xNN,0xNN	- subprogram #N start
			code_id = CLICK_EXE_Codes_Marker_InterruptStart;
			break;

		case EXE_CODE_CNTRL_SUBPROGRAMM_RETURN:					//00 18 0xff 0xff
			code_id = CLICK_EXE_Codes_Instruction_SubProgrammReturn;
			break;
		case EXE_CODE_CNTRL_INTERRUPT_RETURN:			//00 19 0xff 0xff
			code_id = CLICK_EXE_Codes_Instruction_InterruptReturn;
			break;

		case EXE_CODE_CNTRL_MATH_CALL_DEC:				//0x00,0x40,0xff,0xff,0xff,0xff	- MATH call
		case EXE_CODE_CNTRL_MATH_CALL_DEC_1TIME:		//0x00,0x41,0xff,0xff,0xff,0xff	- MATH call one time
		case EXE_CODE_CNTRL_MATH_CALL_HEX:				//0x00,0x42,0xff,0xff,0xff,0xff	- MATH call
		case EXE_CODE_CNTRL_MATH_CALL_HEX_1TIME:		//0x00,0x43,0xff,0xff,0xff,0xff	- MATH call one time
			code_id = CLICK_EXE_Codes_Instruction_Math;
			break;

		case EXE_CODE_CNTRL_FOR_LOOP:				//00 20 00 03 [NN NN NN NN]	- FOR [ N loop cycles]
		case EXE_CODE_CNTRL_FOR_LOOP_1TIME:			//00 21 00 03 [NN NN NN NN]	- FOR [ N loop cycles] one time
			code_id = CLICK_EXE_Codes_Instruction_For;						
			break;

		case EXE_CODE_CNTRL_NEXT:					//00 22 0xff 0xff
			code_id = CLICK_EXE_Codes_Instruction_Next;						
			break;

		case EXE_CODE_CNTRL_SUBPROGRAMM_END:					//0x00,81,0xff,0xff
			code_id = CLICK_EXE_Codes_Marker_EndOfSubProgramm;						
			break;

		case EXE_CODE_CNTRL_INTERRUPT_END:		//0x00,82,0xff,0xff
			code_id = CLICK_EXE_Codes_Marker_EndOfInterrupt;						
			break;

		case EXE_CODE_CNTRL_MAIN_END:					//0x00,80,0xff,0xff
			code_id = CLICK_EXE_Codes_Instruction_EndOfMain;						
			break;

		default:
			//Unknown
			;
		}
		break;

	case EXE_CODE_INSTRUCTION_LOGIC_IF:
	case EXE_CODE_INSTRUCTION_LOGIC_AND:
	case EXE_CODE_INSTRUCTION_LOGIC_OR:
		code_id = CLICK_EXE_Codes_Condition;
		break;				

	case EXE_CODE_INSTRUCTION_COIL:
		//second byte
		if (stream_byte_1 == EXE_CODE_INSTRUCTION_SHIFTREG_SECONDBYTE)
		{
			code_id = CLICK_EXE_Codes_Instruction_ShiftReg;
		}
		else
		{	//second byte

			if ((stream_byte_1&0xf0) == EXE_CODE_SUBINSTR_COIL_OUT)
			{
				code_id = CLICK_EXE_Codes_Instruction_Out;
			}
			if (stream_byte_1 & EXE_CODE_SUBINSTR_COIL_SET)
			{
				code_id = CLICK_EXE_Codes_Instruction_Set;
			}
			if (stream_byte_1 & EXE_CODE_SUBINSTR_COIL_RST)
			{
				code_id = CLICK_EXE_Codes_Instruction_Rst;
			}
		}
		break;

	case EXE_CODE_INSTRUCTION_COPY:				//copy					

		code_id = CLICK_EXE_Codes_Instruction_Copy;
		break;

	case EXE_CODE_INSTRUCTION_SEARCH:			//search
		code_id = CLICK_EXE_Codes_Instruction_Search;
		break;

	case EXE_CODE_INSTRUCTION_TIMER:			//timer
		code_id = CLICK_EXE_Codes_Instruction_Timer;
		break;

	case EXE_CODE_INSTRUCTION_COUNTER:			//counter
		code_id = CLICK_EXE_Codes_Instruction_Counter;
		break;

	case EXE_CODE_INSTRUCTION_SEND:				//send to port

		code_id = CLICK_EXE_Codes_Instruction_Send;
		break;

	case EXE_CODE_INSTRUCTION_RECEIVE:	//receive from port					

		code_id = CLICK_EXE_Codes_Instruction_Receive;
		break;

	case EXE_CODE_INSTRUCTION_DRUM:

		code_id = CLICK_EXE_Codes_Instruction_Drum;
		break;

	case EXE_CODE_INSTRUCTION_EOF:				//0xff,0xff,0xff,0xff

		code_id = CLICK_EXE_Codes_Marker_EOF;
		break;

	default:
		//unknown
		;
	}

	return(code_id);
}