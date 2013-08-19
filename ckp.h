#ifndef __ckp_h__
#define __ckp_h__

#define CKP_TEXT char

#define CKP_SCR_MAX_FILE_NAME			24

#define CLICKPLC_CKP_STRING_SIZE_MAX 256

#define CPK_VERSION_STAMP_SIZE 4
	//v 1.32	
static unsigned char CPKVersionStamp[CPK_VERSION_STAMP_SIZE] = {0x00, 0x00, 0x1E, 0x01};

//markers (max length 8 bytes, aligned by 0x20):

#define CPK_MARKER_SIZE 8
#define CPK_MARKERS_MAX 6

enum SCMarkersIDs
{
	SCMarkersID_PRJ = 0,
	SCMarkersID_INI,
	SCMarkersID_NICK,
	SCMarkersID_DVIEW,
	SCMarkersID_SCR,
	SCMarkersID_CMORE,
	SCMarkersID_EndPlaceholder
};

static 	char *SCMarkers [CPK_MARKERS_MAX] = 
{
	"SC-PRJ  ",	//0
	"SC-INI  ",	//1
	"SC-NICK ",	//2
	"SC-DVIEW",	//3
	"SC-SCR  ",	//4
	"SC-CMORE"	//5
};

typedef struct tagCPKTopHeader
{
	unsigned short crc16sign;
	
	//SC-PRJ
	unsigned int seg_0_offset;
	unsigned int seg_0_length;
	//SC-INI
	unsigned int seg_1_offset;
	unsigned int seg_1_length;
	//SC-NICK
	unsigned int seg_2_offset;
	unsigned int seg_2_length;
	//SC-DVIEW
	unsigned int seg_3_offset;
	unsigned int seg_3_length;
	//SC-CMORE
	unsigned int seg_4_offset;
	unsigned int seg_4_length;

#define CPK_SCR_N_MAX (1<<8)
	unsigned short n_scr_files;

}CPKTopHeader;

typedef struct tagCPKTopSubHeader
{
	//SC-SCR
	unsigned int scr_file_offset;
	unsigned int scr_file_length;

}CPKTopSubHeader;

#define CPK_SCR_HEADER_SIZE (8+52+4)

typedef struct tagCPKSCRHeader
{
	//SC-SCR header
	char marker[CPK_MARKER_SIZE];
	unsigned char CPKVersionStamp[CPK_VERSION_STAMP_SIZE];

#define CPK_SCR_HEADER_UNKNOWN_DATA_SIZE 52
	unsigned char unknown_data[CPK_SCR_HEADER_UNKNOWN_DATA_SIZE];

	unsigned short scr_id;
	unsigned char scr_fname_length;

	unsigned short scr_fname[128];//0x00 + {0x20 0x00} + padding {0x41 0x00} = (wchar)'A'
		
#define CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE 64
	unsigned char unknown_data2[CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE];
	
#define CPK_SCR_HEADER_INTRO_SIZE 20
	unsigned char intro_codesign_1;//0xf6,0x98,0x90
	unsigned char intro_codesign_2;//0x00,0x01
	unsigned char intro_unknownbyte;//0x0d
	unsigned char intro_n_ranges;
	unsigned char intro_unknown_data[CPK_SCR_HEADER_INTRO_SIZE-4];
	
}CPKSCRHeader;

#define CPK_MAGICSIGN_SIZE 66

static unsigned char CPKSCRMagicSign[CPK_MAGICSIGN_SIZE] = {0x20, 0x00,
															0x01, 0x00, 
															0x01, 0x01,
															0x01, 0x02, 
															0x01, 0x03, 
															0x01, 0x04, 
															0x01, 0x05, 
															0x01, 0x06, 
															0x01, 0x07, 
															0x01, 0x08, 
															0x01, 0x09, 
															0x01, 0x0a, 
															0x01, 0x0b, 
															0x01, 0x0c, 
															0x01, 0x0d, 
															0x01, 0x0e, 
															0x01, 0x0f, 
															0x01, 0x10, 
															0x01, 0x11, 
															0x01, 0x12, 
															0x01, 0x13, 
															0x01, 0x14, 
															0x01, 0x15, 
															0x01, 0x16, 
															0x01, 0x17, 
															0x01, 0x18, 
															0x01, 0x19, 
															0x01, 0x1a, 
															0x01, 0x1b, 
															0x01, 0x1c, 
															0x01, 0x1d, 
															0x01, 0x1e, 
															0x01, 0x1f
															};
//|01 00 01 01 01 02 01 03 |01 04 01 05 01 06 01 07 |01 08 01 09 01 0a 01 0b |01 0c 01 0d 01 0e 01 0f |01 10 01 11 01 12 01 13 |01 14 01 15 01 16 01 17 
//|01 18 01 19 01 1a 01 1b |01 1c 01 1d 01 1e 01 1f
#define CPK_MAGICSIGN2_SIZE 64
static unsigned char CPKSCRMagicSign2[CPK_MAGICSIGN2_SIZE] = {0x00};

#define CPK_MAGICSIGN22_SIZE 66
static unsigned char CPKSCRMagicSign22[CPK_MAGICSIGN22_SIZE] = {0x20,0x00,0x00};

#define CPK_AAAASIGN_SIZE 64
static unsigned char CPKSCRMagicSignA[CPK_AAAASIGN_SIZE] =		{	0x20, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 
																	0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 
																	0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 
																	0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00};


#define CPK_MAGICTIMER2_SIZE 64
static unsigned char CPKSCRMagicTimer2[CPK_MAGICTIMER2_SIZE] = 
{
	0x1f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x01, 0x03, 0x01, 0x04, 0x01, 0x05, 0x01, 0x06, 
	0x01, 0x07, 0x01, 0x08, 0x01, 0x09, 0x01, 0x0a, 0x01, 0x0b, 0x01, 0x0c, 0x01, 0x0d, 0x01, 0x0e, 
	0x01, 0x0f, 0x01, 0x10, 0x01, 0x11, 0x01, 0x12, 0x01, 0x13, 0x01, 0x14, 0x01, 0x15, 0x01, 0x16, 
	0x01, 0x17, 0x01, 0x18, 0x01, 0x19, 0x01, 0x1a, 0x01, 0x1b, 0x01, 0x1c, 0x01, 0x1d, 0x01, 0x1e 
};

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*__ckp_h__*/
