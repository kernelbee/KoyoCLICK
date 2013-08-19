#ifndef __clickplc_ckp_nick_h__
#define __clickplc_ckp_nick_h__

#include "ckp.h"

#define CLICKPLC_CKP_NICK_HEADER_SIZE 64
#define CLICKPLC_CKP_NICK_SUBHEADERS_SIZE 7
#define CLICKPLC_CKP_NICK_FOOTER_SIZE 3

enum tagCLICKPLC_CKP_NICK_Registers
{
	CLICKPLC_CKP_NICK_Register_X	=		0,
	CLICKPLC_CKP_NICK_Register_Y	=		1,
	CLICKPLC_CKP_NICK_Register_C	=		2,
	CLICKPLC_CKP_NICK_Register_T	=		3,	
	CLICKPLC_CKP_NICK_Register_CT	=		4,
	
	CLICKPLC_CKP_NICK_Register_SC	=		5,
	CLICKPLC_CKP_NICK_Register_DS	=		6,
	CLICKPLC_CKP_NICK_Register_DD	=		7,
	CLICKPLC_CKP_NICK_Register_DH	=		8,
	CLICKPLC_CKP_NICK_Register_DF	=		9,
				
	CLICKPLC_CKP_NICK_Register_XD	=		0x0a,
	CLICKPLC_CKP_NICK_Register_YD	=		0x0b,
	CLICKPLC_CKP_NICK_Register_TD	=		0x0c,
	CLICKPLC_CKP_NICK_Register_CTD	=		0x0d,

	CLICKPLC_CKP_NICK_Register_SD	=		0x0e,
	CLICKPLC_CKP_NICK_Register_TXT	=		0x0f

}CLICKPLC_CKP_NICK_Registers;


typedef struct tagCKP_NICK_Item
{
	/*
		TODO!!!

		[0]:	unknown;
		[1]:	unknown;
		[2]:	unknown;

		[3]:	reg.number;
		
		[4]:	unknown;
		[5]:	unknown;
		
		[6]:	reg.id (see enum CLICKPLC_CKP_NICK_Registers);
	*/
	unsigned char	subheader[CLICKPLC_CKP_NICK_SUBHEADERS_SIZE];

	unsigned char*	pstr_register_name;
	unsigned char*	pstr_register_number;
	unsigned char*	pstr_nickname;
	unsigned char*	pstr_initial_value;

}CKP_NICK_Item;

typedef struct tagCLICKPLC_CKP_NICK
{
	/*
		8 bytes	header[0]...[7]:	SC-NICK<space>;
		2 bytes header[8],[9]:		0x00;
		2 bytes header[10],[11]:	version, example: 0x1e,0x01 -> 30,1 -> 1.30;
		52 bytes header[12]...[63]:	0x00;
	*/
	
	unsigned char	header[CLICKPLC_CKP_NICK_HEADER_SIZE];
	
	unsigned char	n_items;
	CKP_NICK_Item*	items;
	
	/*
		TODO!!!
	*/
	unsigned char	footer[CLICKPLC_CKP_NICK_FOOTER_SIZE];

}CLICKPLC_CKP_NICK;

#ifdef __cplusplus
extern "C" {
#endif
		
	void parser_NICK(unsigned char* stream,int size,CLICKPLC_CKP_NICK* nick);
	void destroy_NICK(CLICKPLC_CKP_NICK* nick);
	
#ifdef __cplusplus
}
#endif


#endif //__clickplc_ckp_nick_h__
