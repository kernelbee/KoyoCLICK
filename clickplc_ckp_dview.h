#ifndef __clickplc_ckp_dview_h__
#define __clickplc_ckp_dview_h__

#include "ckp.h"

#define CLICKPLC_CKP_DVIEW_HEADER_SIZE 64
#define CKP_DVIEW_MAX_FILE_NAME 24

typedef struct tagCLICKPLC_CKP_CDV_FILE
{
	CKP_TEXT	file_name[CKP_DVIEW_MAX_FILE_NAME+1];		//max 24 chars/wchars
	
	//example: [28 07] 00 00 ff fe	-> 0x0728 = 1832 bytes = 1960 - 64 - 30 - 6 - 23 - 5 = 1960 - 128;
	unsigned short	body_size;		//"StartOfBody" {0xff, 0xfe} + encoded strings	
	unsigned char	unknown_2x[2];	//00 00
	
	//empty file: file_name + four nulls (00, 00, 00, 00)
	CKP_TEXT*		body;//list of parameters and cells, including marker "StartOfBody" {0xff, 0xfe}
	
}CLICKPLC_CKP_CDV_FILE;

typedef struct tagCLICKPLC_CKP_TV_FILE
{
	//*.tv, for example: TextView.tv
	CKP_TEXT		file_name[CKP_DVIEW_MAX_FILE_NAME+1];		//const string: "TextView.tv"

	//example: [fa 06] 00 00 ff fe	-> 0x06fa = 1786 bytes
	unsigned short	body_size;		//"StartOfBody" {0xff, 0xfe} + encoded strings
	unsigned char	unknown_2x[2];	//00 00

	//empty file: file_name + four nulls (00, 00, 00, 00)

	CKP_TEXT*		body;//list of parameters and cells, including marker "StartOfBody" {0xff, 0xfe}

	/*

	body format:	
	
	ff, fe: StartOfBody

	13 parameters:

	0,0,2,

	628,297,30,80,80,80,60,200,200,55,

	ranges:	{start,end}, 
	251658241,251658255,	//TXT1...TXT15 

	251658250,251658260,	//TXT10...TXT20 

	251658260,251658289,	//TXT20...TXT49 

	0,0,	//empty cells

	0,0,

	0,0,
	...

	parameters: first 13 numbers;
	number#3: View (Current Text) option:
	0 - Text;
	1 - Hex;
	2 - Text&Hex;

	*/
	
}CLICKPLC_CKP_TV_FILE;


typedef struct tagCLICKPLC_CKP_DVIEW
{
	/******************header************************/
	unsigned char	header[CLICKPLC_CKP_DVIEW_HEADER_SIZE];
	/*
		header[0]...[7]:	SC-NICK<space>;	8 bytes
		header[8],[9]:		0x00;			2 bytes
		header[10],[11]:	version,		2 bytes		example: 0x1e,0x01 -> 30,1 -> 1.30;
		header[12]...[63]:	0x00;			52 bytes
	*/
	
	unsigned short	n_cdv_files;
	int	n_tv_files;//const, = 1;
		
	/******************dview files: *.cdv  ************************/
	CLICKPLC_CKP_CDV_FILE*	cdv_files;
	
	/******************tview files:	*.tv  *************************/
	CLICKPLC_CKP_TV_FILE*	tv_files;//only one

}CLICKPLC_CKP_DVIEW;


#ifdef __cplusplus
extern "C" {
#endif

	void parser_DVIEW(unsigned char* stream,int size,CLICKPLC_CKP_DVIEW* dview);
	void destroy_DVIEW(CLICKPLC_CKP_DVIEW* dview);
	
#ifdef __cplusplus
}
#endif


#endif //__clickplc_ckp_dview_h__
