#ifndef __clickplc_exe_codes_h__
#define __clickplc_exe_codes_h__

#define CLICKPLC_SEGID_DAT	0x0002
#define CLICKPLC_SEGID_PRM	0x0003
#define CLICKPLC_SEGID_PRJ	0x0004

#define CLICKPLC_DAT_VERSION	0x010A
#define CLICKPLC_PRM_VERSION	0x0200
#define CLICKPLC_PRJ_VERSION	0x0000

typedef struct tagCLICKPLC_EXE
{
#define CLICKPLC_SEGID_EXE	0x0001
	unsigned short	segid;	//EXE:1;	DAT:2;	PRM:3;	PRJ:4.

#define CLICKPLC_EXE_VERSION	0x0200
	unsigned short	version;	//0200h		010Ah	0200h	0000h

#define CLICKPLC_EXE_OFFSETS_CRC32				4	//32 BIT ANSI X3.66 CRC: http://web.archive.org/web/20080217222209/http://c.snippets.org/snip_lister.php?fname=crc_32.c
#define CLICKPLC_EXE_OFFSETS_CRC32_RANGE_BEGIN	8	//till the EOF
	unsigned int	crc32;

#define CLICKPLC_EXE_OFFSET_NETTO				8	//32-bit
	unsigned int	bits_netto;

#define CLICKPLC_EXE_OFFSET_MAIN_OFFSET			12	//32-bit
	unsigned int	bits_offset;

#define CLICKPLC_EXE_TOP_PADDING_NULLS_SIZE 16
	int				top_padding_nulls_bytes;
	
	unsigned char	*bits;

}CLICKPLC_EXE;



#define EXE_CODE_CNTRL_PREFIX_0x00				0x00

#define EXE_CODE_CNTRL_CONNECT_CONTACTS				0x03		//0x00 0x03 0xff 0xff
#define EXE_CODE_CNTRL_CONNECT_UNCONDITIONALLY		0x07		//0x00 0x07 0xff 0xff	used as a placeholder for unconditional connection

#define EXE_CODE_CNTRL_SUBPROGRAMM_CALL				0x10		//0x00,0x10,0xNN,0xNN	- subprogram #N call
#define EXE_CODE_CNTRL_SUBPROGRAMM_START			0x11		//0x00,0x11,0xNN,0xNN	- subprogram #N start
#define EXE_CODE_CNTRL_INTERRUPT_START				0x12		//0x00 0x12 0xnn,0xnn	- interrupt start: base is 0x03db, so for first  interrupt: 0x00 0x12 0x03,0xdc(=0xdb+0x01); 
																															//for second interrupt: 0x00 0x12 0x03,0xdd(=0xdb+0x02),...

#define EXE_CODE_CNTRL_SUBPROGRAMM_RETURN			0x18		//00 18 0xff 0xff
#define EXE_CODE_CNTRL_INTERRUPT_RETURN				0x19		//00 19 0xff 0xff

#define EXE_CODE_CNTRL_MATH_CALL_DEC				0x40		//0x00,0x40,0xff,0xff,0xff,0xff	- MATH call
#define EXE_CODE_CNTRL_MATH_CALL_DEC_1TIME			0x41		//0x00,0x41,0xff,0xff,0xff,0xff	- MATH call one time
#define EXE_CODE_CNTRL_MATH_CALL_HEX				0x42		//0x00,0x42,0xff,0xff,0xff,0xff	- MATH call
#define EXE_CODE_CNTRL_MATH_CALL_HEX_1TIME			0x43		//0x00,0x43,0xff,0xff,0xff,0xff	- MATH call

#define EXE_CODE_CNTRL_FOR_LOOP						0x20		//00 20 00 03 [NN NN NN NN]	- FOR [ N loop cycles]
#define EXE_CODE_CNTRL_FOR_LOOP_1TIME				0x21		//00 21 00 03 [NN NN NN NN]	- FOR [ N loop cycles] one time
#define EXE_CODE_CNTRL_NEXT							0x22		//00 22 0xff 0xff

#define EXE_CODE_CNTRL_INTERRUPT_END				0x82		//0x00,82,0xff,0xff
#define EXE_CODE_CNTRL_SUBPROGRAMM_END				0x81		//0x00,81,0xff,0xff
#define EXE_CODE_CNTRL_MAIN_END						0x80		//0x00,80,0xff,0xff

//instructions encoding: 2 bytes{INSTRUCTION,SUINSTR,[REGTYPE:REGINDEX]} 
/*
#define EXE_CODE_INSTRUCTION_LOGIC_IF			0x01
#define EXE_CODE_INSTRUCTION_LOGIC_AND			0x02
#define EXE_CODE_INSTRUCTION_LOGIC_OR			0x03
*/

//OUT, SET, RST
#define EXE_CODE_INSTRUCTION_COIL				0x04

#define EXE_CODE_SUBINSTR_COIL_OUT					0x00
#define EXE_CODE_SUBINSTR_COIL_SET					0x10
#define EXE_CODE_SUBINSTR_COIL_RST					0x20

#define EXE_CODE_INSTRUCTION_SHIFTREG_SECONDBYTE	0x30

#define EXE_CODE_INSTRUCTION_COPY				0x0a	//copy
#define EXE_CODE_INSTRUCTION_SEARCH				0x0c	//search
#define EXE_CODE_INSTRUCTION_TIMER				0x05	//timer
#define EXE_CODE_INSTRUCTION_COUNTER			0x06	//counter
#define EXE_CODE_INSTRUCTION_SEND				0x11	//send to port
#define EXE_CODE_INSTRUCTION_RECEIVE			0x10	//receive from port
#define EXE_CODE_INSTRUCTION_DRUM				0x20	//drum sequencer

#define EXE_CODE_INSTRUCTION_EOF				0xff	//0xff,0xff,0xff,0xff


/*

INTERRUPTS:

	#########################################################################################
	SOFTWARE Interrupts:
	#########################################################################################

	#######
	period:
	
	[1...60 000] ms;
	or
	[1...60] sec

	periods are encoded in DAT segment, as unsigned 16-bit, always in ms (sec * 1000 ms -> ms):

	DAT[112] =  period[interrupt#1] &0xff;		//lo
	DAT[113] = (period[interrupt#1]>>8) &0xff;	//hi

	DAT[114] =  period[interrupt#2] &0xff;		//lo
	DAT[115] = (period[interrupt#2]>>8) &0xff;	//hi
		
	#######
	numbers of interrupts:

	encoded in DAT segment from DAT[104]:

	DAT[104] = 1;//interrupt#1
	DAT[105] = 2;//interrupt#2
	
	if there is no any interrupt:
	DAT[104] = 0;

	#########################################################################################
	EXTERNAL Interrupts:
	#########################################################################################

	#######
	numbers of interrupts:

	encoded in DAT segment from DAT[96]:

	DAT[96] = 1;//interrupt#1
	DAT[97] = 2;//interrupt#2

	if there is no any interrupt:
	DAT[96] = 0;

	+ D->C...

*/


//REGISTERS

/*
Bit memory addresses:
Default Retentive Setup:
X Inputs 									X001 - X816 		Bit 	No 
Y Outputs 									Y001 - Y816 		Bit  	No 
C Control Relays (Internal Bits) 			C1 - C2000 			Bit 	No 
T Timer Bits 								T1 - T500 			Bit 	No 
CT Counter Bits 							CT1 - CT250 		Bit 	Yes 
SC System Control Relays (Internal Bits) 	SC1 - SC1000 				Always No


Data memory addresses:
Default Retentive Setup:
DS Data Register 				DS1 - DS4500 		Single Word Integer 	Yes 
DD Data Register 				DD1 - DD1000 		Double Word Integer 	Yes 
DH Data Register 				DH1 - DH500 		Hex  					Yes 
DF Data Register 				DF1 - DF500 		Floating Point 			Yes 

XD Input Register 				XD0 - XD8 			Hex 					Always No 
YD Output Register 				YD0 - YD8 			Hex 					Always No 
TD Timer Current Values 		TD1 - TD500 		Single Word Integer 	No

CTD Counter Current Values 		CTD1 - CTD250 		Double Word Integer 	Yes 
SD  System Data Register 		SD1 - SD1000 		Single Word Integer 	Always No 
TXT Text Data Register 			TXT1 - TXT1000 		ASCII (7-bit) 			Always No

Only the DS Memory Type can be used as Pointer.  
Pointer Addressing uses the Pointer's data value to Point to a Memory location within the range of one of the eligible Memory types.  
Pointer Addressing can be used for the DS, DD, DH, DF, XD, YD, TD, CTD and TXT Memory types.

*/

enum tagCLICK_EXE_Registers
{
	CLICK_EXE_Register_X	= 0,
	CLICK_EXE_Register_Y	= 1,
	CLICK_EXE_Register_T	= 2,
	CLICK_EXE_Register_CT	= 3,
	CLICK_EXE_Register_C	= 4,
	CLICK_EXE_Register_SC	= 5,
	CLICK_EXE_Register_DS	= 6,
	CLICK_EXE_Register_DD	= 7,
	CLICK_EXE_Register_DH	= 8,
	CLICK_EXE_Register_DF	= 9,
	CLICK_EXE_Register_SD	= 10,
	CLICK_EXE_Register_TD	= 11,
	CLICK_EXE_Register_CTD	= 12,
	CLICK_EXE_Register_TXT	= 13,
	CLICK_EXE_Register_XD	= 14,
	CLICK_EXE_Register_YD	= 15

}CLICK_EXE_Registers;

/*
case CLICK_EXE_Register_X:
case CLICK_EXE_Register_Y:
case CLICK_EXE_Register_T:
case CLICK_EXE_Register_CT:
case CLICK_EXE_Register_C:
case CLICK_EXE_Register_SC:
case CLICK_EXE_Register_DS:
case CLICK_EXE_Register_DD:
case CLICK_EXE_Register_DH:
case CLICK_EXE_Register_DF:
case CLICK_EXE_Register_SD:
case CLICK_EXE_Register_TD:
case CLICK_EXE_Register_CTD:
case CLICK_EXE_Register_TXT:
case CLICK_EXE_Register_XD:
case CLICK_EXE_Register_YD:
*/

typedef struct tagCLICK_EXE_RegMetrics
{	
	unsigned int	baddr;
	int				n_min;
	int				n_max;	
	unsigned int	enc_max;
	char			*txtID;
	int				txtsize;
	char			*txtformat;

}CLICK_EXE_RegMetrics;


static CLICK_EXE_RegMetrics CLICK_EXE_RegDataBase[] = 
{	
	//base address		Nmin	Nmax	max
	{0xc000,			1,		816,	0xc10f,	"X",	1,	"%s%03d"},		//0	X
	{0xc400,			1,		816,	0xc50f,	"Y",	1,	"%s%03d"},		//1	Y
	{0xc800,			1,		500,	0xc9f3,	"T",	1,	"%s%d"},		//2 T
	{0xcc00,			1,		250,	0xccf9,	"CT",	2,	"%s%d"},		//3 CT
	{0xd000,			1,		2000,	0xd7cf,	"C",	1,	"%s%d"},		//4 C
	{0xf000,			1,		1000,	0xf3e7,	"SC",	2,	"%s%d"},		//5 SC
	{0x0000,			1,		4500,	0x1193,	"DS",	2,	"%s%d"},		//6 DS
	{0x1194,			1,		1000,	0x1962,	"DD",	2,	"%s%d"},		//7 DD
	{0x1964,			1,		500,	0x1b57,	"DH",	2,	"%s%d"},		//8 DH
	{0x1b58,			1,		500,	0x1f3e,	"DF",	2,	"%s%d"},		//9 DF
	{0x5000,			1,		1000,	0x53e7,	"SD",	2,	"%s%d"},		//10 SD
	{0x6000,			1,		500,	0x61f3,	"TD",	2,	"%s%d"},		//11 TD
	{0x6800,			1,		250,	0x69f2,	"CTD",	3,	"%s%d"},		//12 CTD
	{0x7000,			1,		1000,	0x73e7,	"TXT",	3,	"%s%d"},		//13 TXT
	{0x7c00,			0,		8,		0x7c08,	"XD",	2,	"%s%d"},		//14 XD
	{0x7c40,			0,		8,		0x7c48,	"YD",	2,	"%s%d"}		//15 YD	
};

typedef struct tagEXE_Register
{
	int reg_id;	//see enum CLICK_EXE_Registers
	int	number;	//not index!
}EXE_Register;

typedef struct tagEXE_Math
{
	unsigned char	is_hex;		// 0/1
	unsigned char	is_one_shot;// 0/1
	
	EXE_Register	result_reg_ref;	//if dec: any {int16,int32,float}regs, except DH!; if hex: DH only.
	
#define EXE_CODE_INSTRUCTION_MATH_ASCII_MAX	128	
	unsigned short	formula_size;
	unsigned char	formula[EXE_CODE_INSTRUCTION_MATH_ASCII_MAX];

}EXE_Math;

typedef struct tagEXE_Drum
{
#define EXE_CODE_INSTRUCTION_DRUM_TIMEBASED_ms		0x00		//0000 0000
#define EXE_CODE_INSTRUCTION_DRUM_TIMEBASED_sec		0x02		//0000 0010
#define EXE_CODE_INSTRUCTION_DRUM_TIMEBASED_min		0x04		//0000 0100
#define EXE_CODE_INSTRUCTION_DRUM_TIMEBASED_hrs		0x06		//0000 0110
#define EXE_CODE_INSTRUCTION_DRUM_TIMEBASED_days	0x08		//0000 1000

#define EXE_CODE_INSTRUCTION_DRUM_EVENTBASED		0x0a		//0000 1010

#define EXE_CODE_INSTRUCTION_DRUM_MODIFIER_JOGIN	0x01
	
	unsigned char drum_type;
	
	unsigned char is_jog_in_enabled;		// 0/1
	unsigned char is_load_new_step_enabled;	// 0/1
	
#define DRUM_MAX_STEPS		16
	unsigned char n_drum_steps;
#define DRUM_MAX_OUTPUTS	16
	unsigned char n_drum_outputs;
	
	EXE_Register	outputs_bit_regs_ref[DRUM_MAX_OUTPUTS];//common
	
	unsigned short	steps_time_duration[DRUM_MAX_STEPS];	//time based
	EXE_Register	steps_event_bits_ref[DRUM_MAX_STEPS];	//event based
	
	unsigned short	steps_enc_sequences[DRUM_MAX_STEPS];	//common
	
	EXE_Register	curr_step_reg_ref;			//DS only
	EXE_Register	elapsed_step_time_reg_ref;	//TD only
	EXE_Register	complete_bit_reg_ref;		//C only
	
	//is_load_new_step_enabled == 1
	EXE_Register	new_step_reg_ref;			//DS only	

}EXE_Drum;

typedef struct tagEXE_Copy
{
#define EXE_CODE_INSTRUCTION_COPY_SINGLE	0x00
#define EXE_CODE_INSTRUCTION_COPY_BLOCK		0x02
#define EXE_CODE_INSTRUCTION_COPY_FILL		0x04
#define EXE_CODE_INSTRUCTION_COPY_PACK		0x06
#define EXE_CODE_INSTRUCTION_COPY_UNPACK	0x08
#define EXE_CODE_INSTRUCTION_COPY_MODIFIER_1SHOT		0x01
	unsigned char	type;
	
	unsigned char	is_one_shot;
	
	unsigned char	int2txt_not_suppress_zero_flt2txt_exponent_form;		//0 - suppress zero, 1 - not suppress zero
																			//0 - real number, 1 - exponent form
	
#define EXE_CODE_INSTRUCTION_COPY_SRC_REG			0x11
#define EXE_CODE_INSTRUCTION_COPY_SRC_CONSTDEC16b	0x21
#define EXE_CODE_INSTRUCTION_COPY_SRC_CONSTDEC32b	0x31
#define EXE_CODE_INSTRUCTION_COPY_SRC_CONSTHEX		0x41
#define EXE_CODE_INSTRUCTION_COPY_SRC_CONSTFLT32	0x61
#define EXE_CODE_INSTRUCTION_COPY_SRC_CONSTSTR		0x71
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
	
#define EXE_CODE_INSTRUCTION_COPY_ASCII_MAX 128
	unsigned short	ascii_string_size;
	unsigned char	ascii_string[EXE_CODE_INSTRUCTION_COPY_ASCII_MAX];
	
}EXE_Copy;

typedef struct tagEXE_Search
{
#define EXE_CODE_INSTRUCTION_SEARCH_EQUAL				0x10
#define EXE_CODE_INSTRUCTION_SEARCH_NOTEQUAL			0x11
#define EXE_CODE_INSTRUCTION_SEARCH_MORE				0x12
#define EXE_CODE_INSTRUCTION_SEARCH_MOREOREQUAL			0x13
#define EXE_CODE_INSTRUCTION_SEARCH_LESS				0x14
#define EXE_CODE_INSTRUCTION_SEARCH_LESSOREQUAL			0x15
	unsigned char	search_condition;

#define EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_REGISTER	0x11
#define EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC16bit	0x21
#define EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC32bit	0x31
#define EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX16		0x41
#define EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_FLT32		0x61
#define EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_ASCIISTR	0x71
	unsigned char	searching_container;
	
	unsigned char	is_one_shot;			//	0/1
	unsigned char	is_continuous_search;	//	0/1
	
#define EXE_CODE_INSTRUCTION_SEARCH_RESULT_UNSUCCESSFUL (-1)	//-> result_reg_ref
	EXE_Register	result_reg_ref;			//DS only
	EXE_Register	found_bit_ref;			//C only
	
	EXE_Register	search_range_first_reg_ref;
	EXE_Register	search_range_last_reg_ref;
	
	EXE_Register	searching_reg_ref;

	int				searching_const_int32;
	short			searching_const_int16;
	unsigned short	searching_const_hex16bit;
	float			searching_const_flt32;
	
#define EXE_CODE_INSTRUCTION_SEARCH_ASCII_MAX 128
	unsigned short	searching_string_size;
	unsigned char	searching_string[EXE_CODE_INSTRUCTION_SEARCH_ASCII_MAX+1];

}EXE_Search;

typedef struct tagEXE_Timer
{
#define TIMER_UNITS_MASK		0x38	//111 00 0
#define TIMER_UNITS_DAY			0x20	//100 00 0
#define TIMER_UNITS_HOUR		0x18	//011 00 0
#define TIMER_UNITS_MIN			0x10	//010 00 0
#define TIMER_UNITS_SEC			0x08	//001 00 0
#define TIMER_UNITS_MS			0x00	//000 00 0
	unsigned char	units;
	
#define TIMER_FLAGS_RETAINED	0x04
#define TIMER_FLAGS_OFFDELAY	0x02

	unsigned char	flags;

#define TIMER_SPTYPE_MASK		0x01
	int				is_sp_const_number;

	short			sp_const_number;	//in units
	EXE_Register	sp_reg_ref;			//DSx only

	/*
	EXE_Register	in_enable_bit_ref;
	EXE_Register	in_reset_bit_ref;
	*/	
	EXE_Register	timer_bit_ref;		//Tx
	
}EXE_Timer;

typedef struct tagEXE_Counter
{
#define COUNTER_COUNT_TYPE_UP		0x00
#define COUNTER_COUNT_TYPE_DOWN		0x02
#define COUNTER_COUNT_TYPE_UPDOWN	0x04
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

}EXE_Counter;

typedef struct tagEXE_ShiftRegister
{
	EXE_Register start_ref;	//C-regs only
	EXE_Register end_ref;

	/*	
	EXE_Register in_data_bit_ref;
	EXE_Register in_clock_bit_ref;
	EXE_Register in_reset_bit_ref;
	*/

}EXE_ShiftRegister;

typedef struct tagEXE_Send
{
#define SEND_MSG_TYPE_STATIC	0x02
#define SEND_MSG_TYPE_DYNAMIC	0x01
#define SEND_MSG_TYPE_MODBUS	0x00
	unsigned char	msg_type;		//static(0x02) or dynamic(0x01) message
	
	unsigned char	port_index;	//0,1,2...
	
	unsigned char	is_const_length_of_dynamic;//for dynamic msg: 0x10 if constant length of msg has been encoded in last 2 bytes
	
	EXE_Register	success_bit_ref;
	
	unsigned char	is_sending_bit_used;
	EXE_Register	sending_bit_ref;
	
#define SEND_ASCII_BYTE_SWAP_NONE		0x00
#define SEND_ASCII_BYTE_SWAP_ALL		0x01
#define SEND_ASCII_BYTE_SWAP_ALLBUTNUL	0x02
	unsigned char	byte_swap;	//

	unsigned char	n_terminals;//0,1,2
	unsigned char	terminal_byte_1;
	unsigned char	terminal_byte_2;

#define SEND_ASCII_MAX_MSG_LENGTH 128	
	unsigned short	static_msg_length;			//max = 128
	unsigned char	static_msg[SEND_ASCII_MAX_MSG_LENGTH];

	EXE_Register	dynamic_msg_source_ref;		//TXT only
	unsigned short	dynamic_msg_const_length;	//max = 128
	EXE_Register	dynamic_msg_length_ref;		//DS only

}EXE_Send;

typedef struct tagEXE_Receive
{
#define RECEIVE_TYPE_FIXED		0x02
#define RECEIVE_TYPE_VARIABLE	0x01
#define RECEIVE_TYPE_MODBUS		0x00
	unsigned char	data_type;//fixed, variable, modbus
	
	unsigned char	port_index;	//0,1,2...
	
	EXE_Register	dest_ref;	//TXT only
	
	unsigned char	is_success_bit_used;
	EXE_Register	success_bit_ref;
	
	unsigned char	is_receiving_bit_used;
	EXE_Register	receiving_bit_ref;
	
	unsigned char	is_overflow_bit_used;
	EXE_Register	overflow_bit_ref;
	
	unsigned char	is_intertimeout_bit_used;
	EXE_Register	intertimeout_bit_ref;
	
	unsigned char	is_firsttimeout_bit_used;
	EXE_Register	firsttimeout_bit_ref;
	
#define RECEIVE_ASCII_BYTE_SWAP_NO			0x00
#define RECEIVE_ASCII_BYTE_SWAP_ALL			0x01
#define RECEIVE_ASCII_BYTE_SWAP_ALLBUTNUL	0x02
	unsigned char	byte_swap;
	
	EXE_Register	fixed_length_holding_ref;	//DS only
	
	unsigned char	variable_n_terminals;
	unsigned char	variable_terminal_byte_1;
	unsigned char	variable_terminal_byte_2;
	
#define RECEIVE_ASCII_TIMEOUTS_NONE		0x00
#define RECEIVE_ASCII_TIMEOUTS_100ms	0x01
#define RECEIVE_ASCII_TIMEOUTS_200ms	0x02
#define RECEIVE_ASCII_TIMEOUTS_300ms	0x03
#define RECEIVE_ASCII_TIMEOUTS_400ms	0x04
#define RECEIVE_ASCII_TIMEOUTS_500ms	0x05
	unsigned short	intertimeout;
	unsigned short	firsttimeout;
	
}EXE_Receive;

typedef struct tagEXE_For
{
	int is_one_shot;
	int is_variable;
	
	unsigned int	const_cycles;
	EXE_Register	variable_cycles_ref;

}EXE_For;

typedef struct tagEXE_Call
{
	unsigned short	subprogramm_no;//2,3,4...

}EXE_Call;

typedef struct tagEXE_SubProgrammStart
{
	unsigned short	subprogramm_no;//2,3,4...

}EXE_SubProgrammStart;

typedef struct tagEXE_InterruptStart
{
	unsigned short	interrupt_no;//1,2,3,4...

}EXE_InterruptStart;

typedef struct tagEXE_Condition
{
#define EXE_CODE_INSTRUCTION_LOGIC_IF			0x01
#define EXE_CODE_INSTRUCTION_LOGIC_AND			0x02
#define EXE_CODE_INSTRUCTION_LOGIC_OR			0x03

	unsigned char	cond_logic;//if,and,or
	
	//NO,NC,EDGE,COMPARE
#define EXE_CODE_SUBINSTR_ON						0x00
#define EXE_CODE_SUBINSTR_OFF						0x02
#define EXE_CODE_SUBINSTR_UP						0x04	//no modifiers
#define EXE_CODE_SUBINSTR_DOWN						0x05	//no modifiers

#define EXE_CODE_SUBINSTR_MODIFIER_IMMEDIATE		0x01	//ON/OFF for X only

#define EXE_CODE_SUBINSTR_COMPARE_EQUAL				0x10	//==
#define EXE_CODE_SUBINSTR_COMPARE_NOTEQUAL			0x11	//!=
#define EXE_CODE_SUBINSTR_COMPARE_MORE				0x12	//>
#define EXE_CODE_SUBINSTR_COMPARE_MOREOREQUAL		0x13	//>=
#define EXE_CODE_SUBINSTR_COMPARE_LESS				0x14	//<
#define EXE_CODE_SUBINSTR_COMPARE_LESSOREQUAL		0x15	//<=
		
	unsigned char	cond_type;
	unsigned char	is_immediate;	//	ON/OFF for X only

	EXE_Register	bit_reg_ref;	//	ON,OFF,UP,DOWN only
	
	//COMPARE A vs B
	int				is_A_const;
	int				is_B_const;

	int				cmp_A_const;
	EXE_Register	cmp_A_reg_ref;
		
	int				cmp_B_const;
	EXE_Register	cmp_B_reg_ref;

}EXE_Condition;

typedef struct tagEXE_Out
{		
	int				is_one_shot;
	int				is_immediate;//for Y only
	int				is_range;

	EXE_Register	first_bit_reg_ref;
	EXE_Register	last_bit_reg_ref;//optional

}EXE_Out;

typedef struct tagEXE_Set_Rst
{	
	int				is_immediate;//for Y only
	int				is_range;

	EXE_Register	first_bit_reg_ref;
	EXE_Register	last_bit_reg_ref;//optional

}EXE_Set_Rst;

#ifdef __cplusplus
extern "C" {
#endif
	//############################################################################
	//##################	Basic ENCODERs/DECODERs	##############################
	//############################################################################
	
	//return: encoded size, ok=2, error=0.	
	int clickplc_int16_enc(	unsigned short number,unsigned char* stream2bytes);	
	int clickplc_int16_dec(	unsigned char* stream2bytes,unsigned short* pnumber);
	
	//return: encoded size, ok=4, error=0.	
	int clickplc_int32_enc(	unsigned int number,unsigned char* stream4bytes);
	int clickplc_int32_dec(	unsigned char* stream4bytes,unsigned int* pnumber);
	
	//return: ok=1, error=0.	
	int clickplc_reg_check(	EXE_Register* reg);	
	
	//return: encoded size of register, ok=2, error=0.	
	int clickplc_reg_enc(	EXE_Register* reg,unsigned char* stream2bytes);
	
	//return: encoded size of register, ok=2, error=0.	
	int clickplc_reg_dec(	unsigned char* stream2bytes,EXE_Register* reg);

#define CKP_REG_TEXT_MAX_SIZE (16)
	//return: encoded size of register, error=0.	
	int clickplc_reg_pascal_enc(EXE_Register* reg,unsigned char* pascal_string);
	int clickplc_reg_pascal_dec(unsigned char* pascal_string,EXE_Register* reg);
	int clickplc_regid_cstr_dec(char* cstr_string,int* preg_id);
	int clickplc_reg_cstr_dec(char* cstr_string,EXE_Register* reg);
	int clickplc_regbyptr_cstr_dec(char* cstr_string,EXE_Register* reg,EXE_Register* ptrreg);	
	int clickplc_regbyptr_pascal_enc(EXE_Register* reg,EXE_Register* ptrreg,unsigned char* pascal_string);

	//############################################################################
	//#############	    ENCODERs/DECODERs	for all objects ######################
	//	void*	obj:		object to be encoded/decoded
	//	uchar*	stream:		byte stream of encoded objects
	//			return:		object's encoded size !=0, or error=0.
	//############################################################################
	int clickplc_exe_Timer_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Timer_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_Counter_enc			(void* obj,unsigned char* stream);
	int clickplc_exe_Counter_dec			(unsigned char* stream,void* obj);

	int clickplc_exe_ShiftReg_enc			(void* obj,unsigned char* stream);
	int clickplc_exe_ShiftReg_dec			(unsigned char* stream,void* obj);

	int clickplc_exe_For_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_For_dec				(unsigned char* stream,void* obj);
	
	int clickplc_exe_Next_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Next_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_EndOfSubProgramm_enc	(void* obj,unsigned char* stream);
	int clickplc_exe_EndOfSubProgramm_dec	(unsigned char* stream,void* obj);

	int clickplc_exe_EndOfInterrupt_enc		(void* obj,unsigned char* stream);
	int clickplc_exe_EndOfInterrupt_dec		(unsigned char* stream,void* obj);

	int clickplc_exe_EndOfMain_enc			(void* obj,unsigned char* stream);
	int clickplc_exe_EndOfMain_dec			(unsigned char* stream,void* obj);
	
	int clickplc_exe_Call_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Call_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_SubProgrammStart_enc	(void* obj,unsigned char* stream);
	int clickplc_exe_SubProgrammStart_dec	(unsigned char* stream,void* obj);

	int clickplc_exe_InterruptStart_enc		(void* obj,unsigned char* stream);
	int clickplc_exe_InterruptStart_dec		(unsigned char* stream,void* obj);

	int clickplc_exe_Math_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Math_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_Copy_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Copy_dec				(unsigned char* stream,void* obj);
	
	int clickplc_exe_Drum_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Drum_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_Search_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Search_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_Send_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_Send_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_Receive_enc			(void* obj,unsigned char* stream);
	int clickplc_exe_Receive_dec			(unsigned char* stream,void* obj);
	
	int clickplc_exe_Condition_enc			(void* obj,unsigned char* stream);
	int clickplc_exe_Condition_dec			(unsigned char* stream,void* obj);

	int clickplc_exe_Connect2Upper_enc		(void* obj,unsigned char* stream);
	int clickplc_exe_Connect2Upper_dec		(unsigned char* stream,void* obj);

	int clickplc_exe_ConnectUncond_enc		(void* obj,unsigned char* stream);
	int clickplc_exe_ConnectUncond_dec		(unsigned char* stream,void* obj);

	int clickplc_exe_SubProgrammReturn_enc	(void* obj,unsigned char* stream);
	int clickplc_exe_SubProgrammReturn_dec	(unsigned char* stream,void* obj);

	int clickplc_exe_InterruptReturn_enc	(void* obj,unsigned char* stream);
	int clickplc_exe_InterruptReturn_dec	(unsigned char* stream,void* obj);

	int clickplc_exe_EOF_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_EOF_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_OUT_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_OUT_dec				(unsigned char* stream,void* obj);

	int clickplc_exe_SET_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_RST_enc				(void* obj,unsigned char* stream);
	int clickplc_exe_SET_RST_dec			(unsigned char* stream,void* obj);

typedef int (*ptr_clickplc_exe_object_enc)	(void* obj,unsigned char* stream);
typedef int (*ptr_clickplc_exe_object_dec)	(unsigned char* stream,void* obj);

#define CLICKPLC_EXE_OBJECT_ENC_MAXSIZE		1024

enum tagCLICK_EXE_Codes
{
	CLICK_EXE_Codes_Condition	 = 0,
	
	//Markers: don't need any condition ahead
	CLICK_EXE_Codes_Marker_Connect2UpperContacts,
	CLICK_EXE_Codes_Marker_ConnectUnconditionally,
	CLICK_EXE_Codes_Marker_SubProgrammStart,
	CLICK_EXE_Codes_Marker_InterruptStart,	
	CLICK_EXE_Codes_Marker_EndOfSubProgramm,
	CLICK_EXE_Codes_Marker_EndOfInterrupt,
	CLICK_EXE_Codes_Marker_EOF,

	//Instructions: they need condition (or marker CLICK_EXE_Codes_Marker_ConnectUnconditionally) ahead
	CLICK_EXE_Codes_Instruction_Out,
	CLICK_EXE_Codes_Instruction_Set,
	CLICK_EXE_Codes_Instruction_Rst,
	CLICK_EXE_Codes_Instruction_ShiftReg,
	CLICK_EXE_Codes_Instruction_Copy,
	CLICK_EXE_Codes_Instruction_Search,
	CLICK_EXE_Codes_Instruction_Timer,
	CLICK_EXE_Codes_Instruction_Counter,
	CLICK_EXE_Codes_Instruction_Send,
	CLICK_EXE_Codes_Instruction_Receive,
	CLICK_EXE_Codes_Instruction_Drum,
	CLICK_EXE_Codes_Instruction_Call,
	CLICK_EXE_Codes_Instruction_Math,
	CLICK_EXE_Codes_Instruction_For,
	CLICK_EXE_Codes_Instruction_Next,
	CLICK_EXE_Codes_Instruction_SubProgrammReturn,
	CLICK_EXE_Codes_Instruction_InterruptReturn,
	CLICK_EXE_Codes_Instruction_EndOfMain,

	//End placeholder
	CLICK_EXE_Codes_DummyEnd
	
}CLICK_EXE_Codes;

char* get_clickplc_exe_codes_name(int code_id);

static ptr_clickplc_exe_object_enc CLICKPLC_EXE_Obj_Encoders[] = 
{
	clickplc_exe_Condition_enc,				//	CLICK_EXE_Codes_Condition
	
	clickplc_exe_Connect2Upper_enc,			//	CLICK_EXE_Codes_Marker_Connect2UpperContacts
	clickplc_exe_ConnectUncond_enc,			//	CLICK_EXE_Codes_Marker_ConnectUnconditionally
	clickplc_exe_SubProgrammStart_enc,		//	CLICK_EXE_Codes_Marker_SubProgrammStart
	clickplc_exe_InterruptStart_enc,		//	CLICK_EXE_Codes_Marker_InterruptStart
	clickplc_exe_EndOfSubProgramm_enc,		//	CLICK_EXE_Codes_Marker_EndOfSubProgramm
	clickplc_exe_EndOfInterrupt_enc,		//	CLICK_EXE_Codes_Marker_EndOfInterrupt
	clickplc_exe_EOF_enc,					//	CLICK_EXE_Codes_Marker_EOF
	
	clickplc_exe_OUT_enc,					//	CLICK_EXE_Codes_Instruction_Out
	clickplc_exe_SET_enc,					//	CLICK_EXE_Codes_Instruction_Set
	clickplc_exe_RST_enc,					//	CLICK_EXE_Codes_Instruction_Rst
	clickplc_exe_ShiftReg_enc,				//	CLICK_EXE_Codes_Instruction_ShiftReg
	clickplc_exe_Copy_enc,					//	CLICK_EXE_Codes_Instruction_Copy
	clickplc_exe_Search_enc,				//	CLICK_EXE_Codes_Instruction_Search
	clickplc_exe_Timer_enc,					//	CLICK_EXE_Codes_Instruction_Timer
	clickplc_exe_Counter_enc,				//	CLICK_EXE_Codes_Instruction_Counter
	clickplc_exe_Send_enc,					//	CLICK_EXE_Codes_Instruction_Send
	clickplc_exe_Receive_enc,				//	CLICK_EXE_Codes_Instruction_Receive
	clickplc_exe_Drum_enc,					//	CLICK_EXE_Codes_Instruction_Drum
	clickplc_exe_Call_enc,					//	CLICK_EXE_Codes_Instruction_Call
	clickplc_exe_Math_enc,					//	CLICK_EXE_Codes_Instruction_Math
	clickplc_exe_For_enc,					//	CLICK_EXE_Codes_Instruction_For
	clickplc_exe_Next_enc,					//	CLICK_EXE_Codes_Instruction_Next
	clickplc_exe_SubProgrammReturn_enc,		//	CLICK_EXE_Codes_Instruction_SubProgrammReturn
	clickplc_exe_InterruptReturn_enc,		//	CLICK_EXE_Codes_Instruction_InterruptReturn
	clickplc_exe_EndOfMain_enc,				//	CLICK_EXE_Codes_Instruction_EndOfMain

	0										//	CLICK_EXE_Codes_DummyEnd	
};

static ptr_clickplc_exe_object_dec CLICKPLC_EXE_Obj_Decoders[] = 
{
	clickplc_exe_Condition_dec,				//	CLICK_EXE_Codes_Condition

	clickplc_exe_Connect2Upper_dec,			//	CLICK_EXE_Codes_Marker_Connect2UpperContacts
	clickplc_exe_ConnectUncond_dec,			//	CLICK_EXE_Codes_Marker_ConnectUnconditionally
	clickplc_exe_SubProgrammStart_dec,		//	CLICK_EXE_Codes_Marker_SubProgrammStart
	clickplc_exe_InterruptStart_dec,		//	CLICK_EXE_Codes_Marker_InterruptStart
	clickplc_exe_EndOfSubProgramm_dec,		//	CLICK_EXE_Codes_Marker_EndOfSubProgramm
	clickplc_exe_EndOfInterrupt_dec,		//	CLICK_EXE_Codes_Marker_EndOfInterrupt
	clickplc_exe_EOF_dec,					//	CLICK_EXE_Codes_Marker_EOF

	clickplc_exe_OUT_dec,					//	CLICK_EXE_Codes_Instruction_Out
	clickplc_exe_SET_RST_dec,				//	CLICK_EXE_Codes_Instruction_Set
	clickplc_exe_SET_RST_dec,				//	CLICK_EXE_Codes_Instruction_Rst
	clickplc_exe_ShiftReg_dec,				//	CLICK_EXE_Codes_Instruction_ShiftReg
	clickplc_exe_Copy_dec,					//	CLICK_EXE_Codes_Instruction_Copy
	clickplc_exe_Search_dec,				//	CLICK_EXE_Codes_Instruction_Search
	clickplc_exe_Timer_dec,					//	CLICK_EXE_Codes_Instruction_Timer
	clickplc_exe_Counter_dec,				//	CLICK_EXE_Codes_Instruction_Counter
	clickplc_exe_Send_dec,					//	CLICK_EXE_Codes_Instruction_Send
	clickplc_exe_Receive_dec,				//	CLICK_EXE_Codes_Instruction_Receive
	clickplc_exe_Drum_dec,					//	CLICK_EXE_Codes_Instruction_Drum
	clickplc_exe_Call_dec,					//	CLICK_EXE_Codes_Instruction_Call
	clickplc_exe_Math_dec,					//	CLICK_EXE_Codes_Instruction_Math
	clickplc_exe_For_dec,					//	CLICK_EXE_Codes_Instruction_For
	clickplc_exe_Next_dec,					//	CLICK_EXE_Codes_Instruction_Next
	clickplc_exe_SubProgrammReturn_dec,		//	CLICK_EXE_Codes_Instruction_SubProgrammReturn
	clickplc_exe_InterruptReturn_dec,		//	CLICK_EXE_Codes_Instruction_InterruptReturn
	clickplc_exe_EndOfMain_dec,				//	CLICK_EXE_Codes_Instruction_EndOfMain

	0										//	CLICK_EXE_Codes_DummyEnd
};

//############################################################################
//#############		CALLOCs/FREEs	for all objects ######################
//	int		object_code:	object's id to be allocated (see CLICK_EXE_Codes)
//	void**	pobject:		container pointer's address
//	calloc() return:		void* container for object (nul for bodyless objects).
//############################################################################

void*	clickplc_exe_object_calloc		(int object_code);
void	clickplc_exe_object_free		(int object_code,void** pobject);

//############################################################################
//#############		PARSER for all objects ###############################
//	testing first 1 or 2 bytes of stream to recognize encoded object
//############################################################################

//returns one from CLICK_EXE_Codes, success != CLICK_EXE_Codes_DummyEnd.
int clickplc_exe_code_get(unsigned char* stream);

#ifdef __cplusplus
}
#endif

#endif //__clickplc_exe_codes_h__
