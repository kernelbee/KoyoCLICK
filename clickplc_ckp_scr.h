#ifndef __clickplc_ckp_scr_h__
#define __clickplc_ckp_scr_h__

#include "ckp.h"
#include "clickplc_exe_codes.h"
#include "protocmn.h"


enum tagCKP_SCR_Ops
{
	SCR_Op_ContactNO	= 0,//"ContactNO"
	SCR_Op_ContactNC,		//"ContactNO"
	SCR_Op_Edge,			//"Edge"
	SCR_Op_Compare,			//"Compare"
	SCR_Op_Out,				//"Out"
	SCR_Op_Set,				//"Out"
	SCR_Op_Rst,				//"Out"
	SCR_Op_Timer,			//"Tmr"
	SCR_Op_Counter,			//"Cnt"
	SCR_Op_Math,			//"Math"
	SCR_Op_Drum,			//"Drum"
	SCR_Op_ShiftReg,		//"SR"
	SCR_Op_Copy,			//"Copy"
	SCR_Op_Search,			//"Search"
	SCR_Op_Call,			//"Call"
	SCR_Op_Return,			//"Return"
	SCR_Op_For,				//"For"
	SCR_Op_Next,			//"Next"
	SCR_Op_End,				//"End"
	SCR_Op_RD,				//"RD"
	SCR_Op_SD,				//"SD"
	SCR_Op_NOP				//

}CKP_SCR_Ops;

typedef struct tagSCROpsData
{
	
	unsigned short	code;
	char*			enc_string;
	int				enc_string_length;
	char*			name;
	unsigned char	u1;
	unsigned char	u2;

}SCROpsData;

#define SCR_OPCODE_NOP			0x0000
#define SCR_OPCODE_CONTACTNO	0x2711
#define SCR_OPCODE_CONTACTNC	0x2712
#define SCR_OPCODE_EDGE			0x2713
#define SCR_OPCODE_COMPARE		0x2714
#define SCR_OPCODE_OUT			0x2715
#define SCR_OPCODE_SET			0x2716
#define SCR_OPCODE_RESET		0x2717
#define SCR_OPCODE_TMR			0x2718
#define SCR_OPCODE_CNT			0x2719
#define SCR_OPCODE_MATH			0x271a
#define SCR_OPCODE_DRUM			0x271b
#define SCR_OPCODE_SR			0x2720
#define SCR_OPCODE_COPY			0x2721
#define SCR_OPCODE_SEARCH		0x2722
#define SCR_OPCODE_CALL			0x2723
#define SCR_OPCODE_RETURN		0x2724
#define SCR_OPCODE_FOR			0x2725
#define SCR_OPCODE_NEXT			0x2726
#define SCR_OPCODE_END			0x2727
#define SCR_OPCODE_RD			0x2728
#define SCR_OPCODE_SD			0x2729

static SCROpsData SCR_Ops_DataBase[] = 
{
	{SCR_OPCODE_CONTACTNO,	"ContactNO",		9,	"ContNO",	0x01,0x01},
	{SCR_OPCODE_CONTACTNC,	"ContactNO",		9,	"ContNC",	0x01,0x01},
	{SCR_OPCODE_EDGE,		"Edge",				4,	"Edge",		0x01,0x01},
	{SCR_OPCODE_COMPARE,	"Compare",			7,	"Compare",	0x01,0x01},
	{SCR_OPCODE_OUT,		"Out",				3,	"Out",		0x01,0x01},
	{SCR_OPCODE_SET,		"Out",				3,	"Set",		0x01,0x01},
	{SCR_OPCODE_RESET,		"Out",				3,	"Reset",	0x01,0x01},
	{SCR_OPCODE_TMR,		"Tmr",				3,	"Timer",	0x02,0x03},
	{SCR_OPCODE_CNT,		"Cnt",				3,	"Counter",	0x03,0x03},
	{SCR_OPCODE_MATH,		"Math",				4,	"Math",		0x01,0x03},
	{SCR_OPCODE_DRUM,		"Drum",				4,	"Drum",		0x04,0x03},
	{SCR_OPCODE_SR,			"SR",				2,	"ShiftReg",	0x03,0x02},
	{SCR_OPCODE_COPY,		"Copy",				4,	"Copy",		0x02,0x02},
	{SCR_OPCODE_SEARCH,		"Search",			6,	"Search",	0x02,0x03},
	{SCR_OPCODE_CALL,		"Call",				4,	"Call",		0x01,0x01},
	{SCR_OPCODE_RETURN,		"Return",			6,	"Return",	0x01,0x01},
	{SCR_OPCODE_FOR,		"For",				3,	"For",		0x01,0x01},
	{SCR_OPCODE_NEXT,		"Next",				4,	"Next",		0x01,0x01},
	{SCR_OPCODE_END,		"End",				3,	"End",		0x01,0x01},
	{SCR_OPCODE_RD,			"RD",				2,	"Receive",	0x03,0x03},
	{SCR_OPCODE_SD,			"SD",				2,	"Send",		0x03,0x03},
	{SCR_OPCODE_NOP,		"",					0,	"NOP",		0x01,0x01}
};

#define SCR_REG_STRING_MAX_SIZE 32

#define SCR_OBJECT_MAX_ENC_SIZE 4096

#define CLICKPLC_CKP_SCR_HEADER_SIZE	64

#define CKP_SCR_FOOTER_SIZE			4
#define CKP_SCR_FOOTER_SIZE_MAIN	(CKP_SCR_FOOTER_SIZE+2)

#define SCR_RUNG_HEADER_SIZE 16

typedef struct tagSCR_Operation
{	
	int					opid;
	
#define CKP_SCR_OPHEADER_SIZE		12
#define CKP_SCR_TMR_OPHEADER_SIZE	13
#define CKP_SCR_CNT_OPHEADER_SIZE	14
#define CKP_SCR_SR_OPHEADER_SIZE	14
#define CKP_SCR_DRUM_OPHEADER_SIZE	38	

	/*
	CKP_TEXT		name[CKP_SCR_MAX_FILE_NAME+1];//max 24 char/wchar
	unsigned short	opcode;				

	+ optional bytes in the end
												//[0]
	unsigned char		flags_oneshot_imm;		//[1]: 1 - one shot (ON) or imm (ON), 3 - one shot(ON)+imm(ON);
	*/
	
	unsigned char		op_pos_row;	//[0]: 1,2,3...255
	unsigned char		op_pos_col;	//[1]: 0,1,2,...31
	unsigned char		unknown1_bytes[2];	//[2,3]: ??? 00,00
	unsigned short		op_index;	//[4,5]
	unsigned char		unknown2_bytes[2];	//[6,7]: ??? 00,00
	
	int					scr_opcode_offset;
	
	void*				body;
	int					body_sizeof;

	int					body_enc_size;
	/*
	{optional}	[2,3] corresponding step number (1,2,3...) from beginning of SCR (in EXE);
	[4,5,6] ???

	{always}	[7] = op_pos_row

	{always}	[8,9] byte offset from beginning of SCR (in CKP), 
	for Tmr: [9,10]
	for Cnt,SR: [10,11]
	for Drum: [11,12]
	*/
	
}SCR_Operation;

typedef struct tagSCR_Rung
{	
	unsigned short		rung_index;			//[0,1]
#define CKP_SCR_MAX_COMMENT_SIZE		0x05ec //1516 bytes
	unsigned short		enc_comment_size;	//[2,3], 1516 bytes max {0x05ec}
	unsigned char		unknown1_bytes[2];	//[4,5]
	unsigned char*		enc_comment;		//[6...]max text size: 1368 bytes
	//unsigned char		unknown2_bytes[6];	
	unsigned short		rung_dim_x;//0002...0007
	unsigned short		rung_dim_y;//0003
	unsigned char		u0;//0x00
	unsigned char		u1;//=0x01 or 0x00

	//////////////////////////////////////////////////////////////////////////			
	unsigned short		n_magic_sign;		//=0x0020
	unsigned char*		magic_sign;			//62 bytes:{0x00,0x00...}
	//////////////////////////////////////////////////////////////////
	unsigned short		n_padding_0s;		//=0x0020
	unsigned char*		padding_0s;			//64 bytes:{0x00,0x00...}
	
	unsigned short		n_ops;				//[0,1] number of operations inside current rung
	unsigned short		magic_word;			//[2,3] = SCR_SubHeader subhdr.magic_word		
	unsigned char		unknown4_bytes[2];	//[4,5]
	/////////////////////////////////////////////////////////////////////////	
	SCR_Operation*		ops;
		
}SCR_Rung;

typedef struct tagCLICKPLC_CKP_SCR
{
	//[0]...[7]:	SC-SCR<space><space>;	8 bytes
	unsigned char		version[CPK_VERSION_STAMP_SIZE];//[8,9,10,11]: 4 bytes example:	0x00, 0x00, 0x1E, 0x01 -> 30,1 -> 1.30;
	unsigned char		zero_bytes2[52];				//[12]...[63]:	0x00;				52 bytes
	//////////////////////////////////////////////////////////////////
	unsigned short		scr_id;							//[64,65]:	1 (Main), 2, 3...
	CKP_TEXT			name[CKP_SCR_MAX_FILE_NAME+1];	//[66...]	max 24 char/wchar
	
	unsigned short		n_padding_As;	//usually = 64: {0x20 0x00}	
	unsigned char*		padding_As;		//padding A: {0x41 0x00}	
	//////////////////////////////////////////////////////////////////////////
#define CKP_SCR_SUBHDR_MIN_SIZE		17
	unsigned short		magic_word;//[0,1]
	unsigned char		unknown_byte;//[2]
	unsigned short		n_rungs;//[3,4]
	
	//////////////////////////////////////////////////////////////////
	SCR_Rung*			rungs;
	//////////////////////////////////////////////////////////////////
														
	unsigned short		n_steps_mainonly;
	//////////////////////////////////////////////////////////////////////////
	
}CLICKPLC_CKP_SCR;


#define SCR_INFO_ENC_SIZE		12
#define SCR_INFO_ENC_SIZE_TMR	13
#define SCR_INFO_ENC_SIZE_SRCNT	14
#define SCR_INFO_ENC_SIZE_DRUM	38 //min

typedef struct tagSCR_OpBodyAlways
{
	unsigned short	scr_offset;//enc: IN{scr-offset of stream_at_opname}, OUT{scr-offset of $$$} ; dec: decoded from stream;	
	unsigned char	scr_pos_row;
	unsigned short	exe_step;

}SCR_OpBodyAlways;

typedef struct tagSCR_Operation_Out
{	
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	int					is_one_shot;
	int					is_immediate;//for Y only
	int					is_range;	
	EXE_Register		first_bit_reg_ref;
	EXE_Register		last_bit_reg_ref;//optional

}SCR_Operation_Out;

typedef struct tagSCR_Operation_SetRst
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	int					is_immediate;//for Y only
	int					is_range;

	EXE_Register		first_bit_reg_ref;
	EXE_Register		last_bit_reg_ref;//optional

}SCR_Operation_SetRst;

typedef struct tagSCR_Operation_Call
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////

	unsigned short		file_list_index;//file_list_index != SCR IDs
	char				calling_scr_name[CKP_SCR_MAX_FILE_NAME+1];
	int					calling_scr_name_size;

}SCR_Operation_Call;

typedef struct tagSCR_Operation_Contact
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	unsigned char		is_immediate;	//	ON/OFF for X only
	EXE_Register		bit_reg_ref;	//	ON,OFF,UP,DOWN only

}SCR_Operation_Contact;

typedef struct tagSCR_Operation_Edge
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	int					is_down;
	EXE_Register		bit_reg_ref;	//	ON,OFF,UP,DOWN only

}SCR_Operation_Edge;

typedef struct tagSCR_Operation_Compare
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
#define SCR_CODE_SUBINSTR_COMPARE_EQUAL				0x00	//==
#define SCR_CODE_SUBINSTR_COMPARE_NOTEQUAL			0x11	//!=
#define SCR_CODE_SUBINSTR_COMPARE_MORE				0x12	//>
#define SCR_CODE_SUBINSTR_COMPARE_MOREOREQUAL		0x13	//>=
#define SCR_CODE_SUBINSTR_COMPARE_LESS				0x14	//<
#define SCR_CODE_SUBINSTR_COMPARE_LESSOREQUAL		0x15	//<=
	unsigned char	cond_type;

	//COMPARE A vs B
	int				is_A_const;
	int				is_B_const;

	int				cmp_A_const;
	EXE_Register	cmp_A_reg_ref;

	int				cmp_B_const;
	EXE_Register	cmp_B_reg_ref;

}SCR_Operation_Compare;

typedef struct tagSCR_Operation_End
{
	SCR_OpBodyAlways	info;

}SCR_Operation_End;

typedef struct tagSCR_Operation_Return
{
	SCR_OpBodyAlways	info;

}SCR_Operation_Return;

typedef struct tagSCR_Operation_For
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	int	is_one_shot;
	int is_variable;
	
	unsigned int	const_cycles;
	EXE_Register	variable_cycles_ref;

}SCR_Operation_For;

typedef struct tagSCR_Operation_Next
{
	SCR_OpBodyAlways	info;

}SCR_Operation_Next;

typedef struct tagSCR_Operation_Copy
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////

#define CODE_COPY_SINGLE	0x00
#define CODE_COPY_BLOCK		0x02
#define CODE_COPY_FILL		0x04
#define CODE_COPY_PACK		0x06
#define CODE_COPY_UNPACK	0x08

	unsigned char	type;
	unsigned char	is_one_shot;
	unsigned char	int2txt_not_suppress_zero_flt2txt_exponent_form;		//0 - suppress zero, 1 - not suppress zero
																			//0 - real number, 1 - exponent form
	unsigned char	src_type;

	/***************** single only ********************/
	unsigned char	is_src_addr_by_pointer;	// 0/1
	unsigned char	is_dst_addr_by_pointer;	// 0/1	
	EXE_Register	src_pointer_reg_ref;
	EXE_Register	dst_pointer_reg_ref;
	/**************************************************/

	EXE_Register	src_reg_start_ref;
	EXE_Register	src_reg_end_ref;

	EXE_Register	dst_reg_start_ref;
	EXE_Register	dst_reg_end_ref;

	int				src_const_int32;
	short			src_const_int16;
	unsigned short	src_const_hex16bit;
	float			src_const_flt32;

#define CODE_COPY_ASCII_MAX 128
	unsigned short	ascii_string_size;
	unsigned char	ascii_string[CODE_COPY_ASCII_MAX+1];

	//////////////////////////////////////////////////////////////////////////
	//* there is no such item in EXE		
	unsigned int	src_const_hex32bit;

}SCR_Operation_Copy;

typedef struct tagSCR_Operation_ShiftRegister
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////

	EXE_Register start_ref;	//C-regs only
	EXE_Register end_ref;

	/*
	EXE_Register in_data_bit_ref;
	EXE_Register in_clock_bit_ref;
	EXE_Register in_reset_bit_ref;
	*/

}SCR_Operation_ShiftRegister;

typedef struct tagSCR_Operation_Counter
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////

/*
#define COUNTER_COUNT_TYPE_UP		0x00
#define COUNTER_COUNT_TYPE_DOWN		0x02
#define COUNTER_COUNT_TYPE_UPDOWN	0x04

//	UP:			00 00
//	DOWN:		fc 21 01 05 3a 00 00 00 00 01 00 2b 22 02 00 2c 22 ff ff	+ 00 00
//	UP&DOWN:	fc 21 02 05 3a 00 00 09 22 01 00 0a 22 02 00 0b 22 ff ff	+ 00 00

*/	
	unsigned char	count_type;

	int				is_sp_const_number;

	int				sp_const_number;
	EXE_Register	sp_reg_ref;	//int16 or int32 only

	/*	
	//IN
	EXE_Register	in_up_bit_ref;
	EXE_Register	in_down_bit_ref;
	EXE_Register	in_reset_bit_ref;
	*/

	//OUT
	EXE_Register	out_complete_bit_ref;

	//////////////////////////////////////////////////////////////////////////
	//* there is no such item in EXE
	EXE_Register	counter_reg_ref;//CTDx

}SCR_Operation_Counter;

typedef struct tagSCR_Operation_Timer
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	unsigned char	units;
	unsigned char	flags;
	
	int				is_sp_const_number;

	short			sp_const_number;	//in units
	EXE_Register	sp_reg_ref;			//DSx only
		
	EXE_Register	timer_bit_ref;		//Tx
	//////////////////////////////////////////////////////////////////////////
	//* there is no such item in EXE
	EXE_Register	counter_reg_ref;	//TDx
	
}SCR_Operation_Timer;

typedef struct tagSCR_Operation_Search
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	unsigned char	search_condition;
	unsigned char	searching_container;
	
	unsigned char	is_one_shot;			//	0/1
	unsigned char	is_continuous_search;	//	0/1
	
	EXE_Register	result_reg_ref;			//DS only
	EXE_Register	found_bit_ref;			//C only
	
	EXE_Register	search_range_first_reg_ref;
	EXE_Register	search_range_last_reg_ref;
	
	EXE_Register	searching_reg_ref;
	int				searching_const_int32;
	short			searching_const_int16;
	unsigned short	searching_const_hex16bit;
	float			searching_const_flt32;
	
	unsigned short	searching_string_size;
	unsigned char	searching_string[EXE_CODE_INSTRUCTION_SEARCH_ASCII_MAX+1];
	
	//////////////////////////////////////////////////////////////////////////
	//* there is no such item in EXE
#define EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX8			0xff	//EXE: encoding as ASCII-string;
																	//SCR: encoding as $xx.

}SCR_Operation_Search;

typedef struct tagSCR_Operation_Math
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
	unsigned char		is_hex;		// 0/1
	unsigned char		is_one_shot;// 0/1
	
	EXE_Register		result_reg_ref;	//if dec: any {int16,int32,float}regs, except DH!; if hex: DH only.

	//					original formula
	unsigned short		formula_size;
	unsigned char		formula[EXE_CODE_INSTRUCTION_MATH_ASCII_MAX];
	
	//////////////////////////////////////////////////////////////////////////
	//there is no such item in EXE
	int					is_vars_by_nicknames;//how to identify variables: 0 - by address; 1- by nick names;
#define SCR_MATH_MAX_N_VARS 125
	EXE_Register		vars[SCR_MATH_MAX_N_VARS];
	int					n_vars;
	//					@
	unsigned char		formula_a[EXE_CODE_INSTRUCTION_MATH_ASCII_MAX];
	unsigned short		formula_a_size;
	//					#@
	unsigned char		formula_na[EXE_CODE_INSTRUCTION_MATH_ASCII_MAX];
	unsigned short		formula_na_size;
	//					#
	unsigned char		formula_n[EXE_CODE_INSTRUCTION_MATH_ASCII_MAX];
	unsigned short		formula_n_size;

}SCR_Operation_Math;

typedef struct tagSCR_Operation_NOP
{
	SCR_OpBodyAlways	info;
	//////////////////////////////////////////////////////////////////////////
}SCR_Operation_NOP;

#ifdef __cplusplus
extern "C" {
#endif
	//testers of SCR tags
	int is_opname(char* test_str,int test_strlen);
	int is_opcode(unsigned short test_num,int* popid);

	//return: encoded size, or error: =0.
	int clickplc_scr_info_enc(SCR_OpBodyAlways* info,unsigned char* stream,int opid);
	int clickplc_scr_info_dec(unsigned char* stream,SCR_OpBodyAlways* info,int opid);

	//return: encoded size, or error: =0.
	int clickplc_scr_EmptyBody_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset,int opid);
	int clickplc_scr_EmptyBody_enc(void* obj,unsigned char* stream_before_opname,int opid);

	//return: encoded size, ok=2, error=0.	
	int clickplc_ckp_int16_enc(	unsigned short number,unsigned char* stream2bytes);	
	int clickplc_ckp_int16_dec(	unsigned char* stream2bytes,unsigned short* pnumber);

	//return: encoded size, ok=4, error=0.	
	int clickplc_ckp_int32_enc(	unsigned int number,unsigned char* stream4bytes);
	int clickplc_ckp_int32_dec(	unsigned char* stream4bytes,unsigned int* pnumber);

	//returns: type of data; writes parsed&decoded data (according to type)
#define CODE_DATATYPE_UNKNOWN		(-1)
#define CODE_DATATYPE_REG			0
#define CODE_DATATYPE_REGBYPTR		1
#define CODE_DATATYPE_CONSTDEC16b	2
#define CODE_DATATYPE_CONSTDEC32b	3
#define CODE_DATATYPE_CONSTHEX8		4 //$x
#define CODE_DATATYPE_CONSTHEX16	5
#define CODE_DATATYPE_CONSTHEX32	6
#define CODE_DATATYPE_CONSTFLT32	7
#define CODE_DATATYPE_CONSTSTR		8
/*	
	case CODE_DATATYPE_UNKNOWN:
	case CODE_DATATYPE_REG:
	case CODE_DATATYPE_REGBYPTR:
	case CODE_DATATYPE_CONSTDEC16b:
	case CODE_DATATYPE_CONSTDEC32b:
	case CODE_DATATYPE_CONSTHEX8:
	case CODE_DATATYPE_CONSTHEX16:
	case CODE_DATATYPE_CONSTHEX32:
	case CODE_DATATYPE_CONSTFLT32:
	case CODE_DATATYPE_CONSTSTR:
*/	
	int clickplc_ckp_str_parse( char* cstr_string,int str_len,		//IN
								EXE_Register*	preg,				//OUT
								EXE_Register*	ptrreg,				//OUT
								unsigned char*	pconst_hex8bit,		//OUT
								unsigned short*	pconst_hex16bit,	//OUT
								unsigned int*	pconst_hex32bit,	//OUT
								short*			pconst_int16,		//OUT
								int*			pconst_int32,		//OUT
								float*			pconst_flt32,		//OUT
								char*			const_str,			//OUT
								int				const_str_lenlim,	//IN
								int*			pconst_strlen		//OUT
								);

	//returns: size of encoded data (bytes):
	int clickplc_ckp_encode_parameter_as_pascal_str(unsigned char*	stream,				//IN/OUT
													int				prm_datatype,		//IN
													void*			prm0,				//IN
													void*			prm1,				//IN
													unsigned char*	orig_text,			//IN
													unsigned short	orig_text_size,		//IN
													unsigned short*	porig_text_size		//OUT
													);

#define CODE_DATATYPE_FORMULA		9
#define CODE_DATATYPE_FORMULA_A		10
#define CODE_DATATYPE_FORMULA_N		11
#define CODE_DATATYPE_FORMULA_NA	12
	//returns: type of data; writes parsed&decoded data (according to type)
	int clickplc_ckp_math_decode(	char* cstr_string,int str_len,		//IN
									
									int				exp_datatype,		//IN expecting type of data

									EXE_Register*	preg,				//OUT																		

									unsigned char*	formula,			//OUT
									int				formula_lenlim,		//IN
									unsigned short*	pformula_len		//OUT
									);

	//returns: size of encoded data (bytes):
	int clickplc_ckp_math_encode(		unsigned char*	stream,				//IN/OUT
										int				prm_datatype,		//IN
										void*			prm0,				//IN
										void*			prm1				//IN
										);



	//############################################################################
	//#############	    ENCODERs/DECODERs	for all objects ######################
	//	void*	obj:		object to be encoded/decoded
	//	uchar*	stream:		byte stream of encoded objects
	//			return:		object's encoded size !=0, or error=0.
	//############################################################################
		
	int clickplc_scr_OUT_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_OUT_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);
	
	int clickplc_scr_Set_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Rst_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_SetRst_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Rst_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Rst_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Call_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Call_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_ContactNO_enc			(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_ContactNC_enc			(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Contact_dec			(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);
	
	int clickplc_scr_Edge_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Edge_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Compare_enc			(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Compare_dec			(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);
	
	int clickplc_scr_For_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_For_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);
	
	int clickplc_scr_Copy_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Copy_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_ShiftReg_enc			(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_ShiftReg_dec			(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Counter_enc			(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Counter_dec			(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Timer_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Timer_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Search_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Search_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Math_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Math_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);
	
	int clickplc_scr_NOP_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_NOP_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);
	
	////////////////////////////Empty body/////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	int clickplc_scr_End_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_End_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Return_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Return_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);

	int clickplc_scr_Next_enc				(void* obj,unsigned char* stream_before_opname);
	int clickplc_scr_Next_dec				(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset);
	//////////////////////////////////////////////////////////////////////////
		
	typedef int (*ptr_clickplc_scr_object_enc)	(void* obj,unsigned char* stream);
	typedef int (*ptr_clickplc_scr_object_dec)	(unsigned char* stream,void* obj,unsigned short* pscr_offset);

	static ptr_clickplc_scr_object_dec CLICKPLC_SCR_Obj_Decoders[] = 
	{
		clickplc_scr_Contact_dec,	//SCR_Op_ContactNO	= 0,//"ContactNO"
		clickplc_scr_Contact_dec,	//SCR_Op_ContactNC,		//"ContactNO"
		clickplc_scr_Edge_dec,		//SCR_Op_Edge,			//"Edge"
		clickplc_scr_Compare_dec,	//SCR_Op_Compare,		//"Compare"
		clickplc_scr_OUT_dec,		//SCR_Op_Out,			//"Out"
		clickplc_scr_SetRst_dec,	//SCR_Op_Set,			//"Out"
		clickplc_scr_SetRst_dec,	//SCR_Op_Rst,			//"Out"
		clickplc_scr_Timer_dec,		//SCR_Op_Timer,			//"Tmr"
		clickplc_scr_Counter_dec,	//SCR_Op_Counter,		//"Cnt"
		clickplc_scr_Math_dec,		//SCR_Op_Math,			//"Math"
		0,//SCR_Op_Drum,									//"Drum"
		clickplc_scr_ShiftReg_dec,	//SCR_Op_ShiftReg,		//"SR"
		clickplc_scr_Copy_dec,		//SCR_Op_Copy,			//"Copy"
		clickplc_scr_Search_dec,	//SCR_Op_Search,		//"Search"
		clickplc_scr_Call_dec,		//SCR_Op_Call,			//"Call"
		clickplc_scr_Return_dec,	//SCR_Op_Return,		//"Return"
		clickplc_scr_For_dec,		//SCR_Op_For,			//"For"
		clickplc_scr_Next_dec,		//SCR_Op_Next,			//"Next"
		clickplc_scr_End_dec,		//SCR_Op_End,			//"End"
		0,//SCR_Op_RD,										//"RD"
		0,//SCR_Op_SD,										//"SD"
		0 //SCR_Op_NOP										//
	};
	
	static ptr_clickplc_scr_object_enc CLICKPLC_SCR_Obj_Encoders[] = 
	{
		clickplc_scr_ContactNO_enc,	//SCR_Op_ContactNO	= 0,//"ContactNO"
		clickplc_scr_ContactNC_enc,	//SCR_Op_ContactNC,		//"ContactNO"
		clickplc_scr_Edge_enc,		//SCR_Op_Edge,			//"Edge"
		clickplc_scr_Compare_enc,	//SCR_Op_Compare,		//"Compare"
		clickplc_scr_OUT_enc,		//SCR_Op_Out,			//"Out"
		clickplc_scr_Set_enc,		//SCR_Op_Set,			//"Out"
		clickplc_scr_Rst_enc,		//SCR_Op_Rst,			//"Out"
		clickplc_scr_Timer_enc,		//SCR_Op_Timer,			//"Tmr"
		clickplc_scr_Counter_enc,	//SCR_Op_Counter,		//"Cnt"
		clickplc_scr_Math_enc,		//SCR_Op_Math,			//"Math"
		0,//SCR_Op_Drum,									//"Drum"
		clickplc_scr_ShiftReg_enc,	//SCR_Op_ShiftReg,		//"SR"
		clickplc_scr_Copy_enc,		//SCR_Op_Copy,			//"Copy"
		clickplc_scr_Search_enc,	//SCR_Op_Search,		//"Search"
		clickplc_scr_Call_enc,		//SCR_Op_Call,			//"Call"
		clickplc_scr_Return_enc,	//SCR_Op_Return,		//"Return"
		clickplc_scr_For_enc,		//SCR_Op_For,			//"For"
		clickplc_scr_Next_enc,		//SCR_Op_Next,			//"Next"
		clickplc_scr_End_enc,		//SCR_Op_End,			//"End"
		0,//SCR_Op_RD,										//"RD"
		0,//SCR_Op_SD,										//"SD"
		0 //SCR_Op_NOP										//
	};
	//############################################################################
	//#############		CALLOCs/FREEs	for all objects ######################
	//	int		object_code:	object's id to be allocated (see CKP_SCR_Ops)
	//	void**	pobject:		container pointer's address
	//	calloc() return:		void* container for object (nul for bodyless objects).
	//############################################################################
	
	void	clickplc_scr_object_printf		(int object_id,void* object);
	
	void*	clickplc_scr_object_calloc		(int object_id,int* pobject_sizeof);
	void	clickplc_scr_object_free		(int object_id,void** pobject);
	
	int		parser_SCR(unsigned char* stream,int size,CLICKPLC_CKP_SCR* scr);
	int		parser_SCR_v1(unsigned char* stream,int size,CLICKPLC_CKP_SCR* scr);
	int		parser_rung(unsigned char* stream,int size,CLICKPLC_CKP_SCR* scr,SCR_Rung* rung,int scr_offset);
	int		parser_op(unsigned char* stream,int size,SCR_Operation* op,int scr_offset);
	void	destroy_SCR(CLICKPLC_CKP_SCR* scr);
	
#ifdef __cplusplus
}
#endif


#endif //__clickplc_ckp_scr_h__

