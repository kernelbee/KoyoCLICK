#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>//isxdigit

#include "clickplc_ckp_scr.h"

//#define __RUN_TALKY_MODE__

#define __RUN_CHUTTY_MODE__

int clickplc_scr_info_enc(SCR_OpBodyAlways* info,unsigned char* stream,int opid)
{
	int encsize = SCR_INFO_ENC_SIZE;//by default


	switch(opid)
	{
	case SCR_Op_Timer:
		
		encsize = SCR_INFO_ENC_SIZE_TMR;
		
		//00 00		02 00	00 00 02 01 02		7e 01 00 00
		stream[0] = 0x00;
		stream[1] = 0x00;
		//{optional}	[2,3] corresponding step number (1,2,3...) from beginning of SCR (in EXE);
		clickplc_ckp_int16_enc(info->exe_step,&stream[2]);
		stream[4] = 0x00;
		stream[5] = 0x00;
		stream[6] = 0x02;
		//7			{always}	op_pos_row
		stream[7] = info->scr_pos_row;//stream[7] = 0x01;
		stream[8] = 0x02;
		
		//9,10		{always}	byte offset from beginning of SCR (in CKP), 	
		clickplc_ckp_int16_enc(info->scr_offset,&stream[9]);
		//11,12		00, 00	???	
		stream[11] = 0x00;
		stream[12] = 0x00;		
		break;			

	case SCR_Op_Drum:
		
		encsize = SCR_INFO_ENC_SIZE_DRUM;
		
		
		//11,12		{always}	byte offset from beginning of SCR (in CKP), 	
		clickplc_ckp_int16_enc(info->scr_offset,&stream[11]);
		break;

	case SCR_Op_Counter:
	case SCR_Op_ShiftReg:

		encsize = SCR_INFO_ENC_SIZE_SRCNT;

		//00 00		04 00	00 00 03 01 02 03	4d 02 00 00 

		//0,1		00, 00	???
		stream[0] = 0x00;
		stream[1] = 0x00;	
		//2,3
		//{optional}	[2,3] corresponding step number (1,2,3...) from beginning of SCR (in EXE);
		clickplc_ckp_int16_enc(info->exe_step,&stream[2]);

		//4,5,6,7,8,9	00 00 03 01 02 03
		stream[4] = 0x00;
		stream[5] = 0x00;
		stream[6] = 0x03;
		//7			{always}	op_pos_row
		stream[7] = info->scr_pos_row;//stream[7] = 0x01;

		stream[8] = 0x02;
		stream[9] = 0x03;
		
		//10,11		{always}	byte offset from beginning of SCR (in CKP), 	
		clickplc_ckp_int16_enc(info->scr_offset,&stream[10]);
		//12,13		00, 00	???	
		stream[12] = 0x00;
		stream[13] = 0x00;

		break;

	default:
		//////////////////////////////////////////////////////////////////////////
		//0,1		00, 00	???
		stream[0] = 0x00;
		stream[1] = 0x00;	
		//2,3
		//{optional}	[2,3] corresponding step number (1,2,3...) from beginning of SCR (in EXE);
		clickplc_ckp_int16_enc(info->exe_step,&stream[2]);	
		//4,5		00, 00	???
		//6					???
		stream[4] = 0x00;
		stream[5] = 0x00;
		stream[6] = 0x01;
		//7			{always}	op_pos_row
		stream[7] = info->scr_pos_row;
		//8,9		{always}	byte offset from beginning of SCR (in CKP), 	
		clickplc_ckp_int16_enc(info->scr_offset,&stream[8]);
		//10,11		00, 00	???	
		stream[10] = 0x00;
		stream[11] = 0x00;
	}

	return(encsize);
}

int clickplc_scr_info_dec(unsigned char* stream,SCR_OpBodyAlways* info,int opid)
{	
	int encsize = SCR_INFO_ENC_SIZE;//by default


	switch(opid)
	{
	case SCR_Op_Timer:

		encsize = SCR_INFO_ENC_SIZE_TMR;

		//{optional}	[2,3] corresponding step number (1,2,3...) from beginning of SCR (in EXE);
		clickplc_ckp_int16_dec(&stream[2],&info->exe_step);

		//7			{always}	op_pos_row
		info->scr_pos_row = stream[7];

		//9,10		{always}	byte offset from beginning of SCR (in CKP), 	
		clickplc_ckp_int16_dec(&stream[9],&info->scr_offset);
		//11,12		00, 00	???	
		break;	

	case SCR_Op_Drum:

		encsize = SCR_INFO_ENC_SIZE_DRUM;

		//11,12		{always}	byte offset from beginning of SCR (in CKP), 	
		clickplc_ckp_int16_dec(&stream[11],&info->scr_offset);
		break;

	case SCR_Op_Counter:
	case SCR_Op_ShiftReg:

		encsize = SCR_INFO_ENC_SIZE_SRCNT;

		//00 00		04 00	00 00 03 01 02 03	4d 02 00 00
		
		//0,1		00, 00	???
		//2,3
		//{optional}	[2,3] corresponding step number (1,2,3...) from beginning of SCR (in EXE);
		clickplc_ckp_int16_dec(&stream[2],&info->exe_step);

		//4,5,6,7,8,9	00 00 03 01 02 03

		//4,5		00, 00	???
		
		//6			03		???

		//7			{always}	op_pos_row
		info->scr_pos_row = stream[7];

		//10,11		{always}	byte offset from beginning of SCR (in CKP), 	
		clickplc_ckp_int16_dec(&stream[10],&info->scr_offset);
		//12,13		00, 00	???	
		break;

	default:
		//0,1		00, 00	???

		//2,3
		//{optional}	[2,3] corresponding step number (1,2,3...) from beginning of SCR (in EXE);
		clickplc_ckp_int16_dec(&stream[2],&info->exe_step);

		//4,5		00, 00	???
		
		//6			01		???

		//7			{always}	op_pos_row
		info->scr_pos_row = stream[7];

		//8,9		{always}	byte offset from beginning of SCR (in CKP), 
		clickplc_ckp_int16_dec(&stream[8],&info->scr_offset);

		//10,11		00, 00	???
	}

	return(encsize);
}

//return: encoded size, ok=2, error=0.	
int clickplc_ckp_int16_enc(	unsigned short number,unsigned char* stream2bytes)
{
	int retval = 2;
	stream2bytes[0] = (unsigned char)(number&0x00ff);
	stream2bytes[1] = (unsigned char)(number>>8);
	return(retval);
}

int clickplc_ckp_int16_dec(	unsigned char* stream2bytes,unsigned short* pnumber)
{
	int retval = 2;
	*pnumber  = ((unsigned short)stream2bytes[1]<<8)|(unsigned short)stream2bytes[0];
	return(retval);
}

//return: encoded size, ok=4, error=0.	
int clickplc_ckp_int32_enc(	unsigned int number,unsigned char* stream4bytes)
{
	int retval = 4;
	stream4bytes[0] = (unsigned char)( number     &0x000000ff);
	stream4bytes[1] = (unsigned char)((number>>8 )&0x000000ff);
	stream4bytes[2] = (unsigned char)((number>>16)&0x000000ff);
	stream4bytes[3] = (unsigned char)( number>>24);
	return(retval);
}

int clickplc_ckp_int32_dec(	unsigned char* stream4bytes,unsigned int* pnumber)
{
	int retval = 4;
	*pnumber  =		(unsigned int)stream4bytes[3]
	|	((unsigned int)stream4bytes[2]<<8)
		|	((unsigned int)stream4bytes[1]<<16)
		|	((unsigned int)stream4bytes[0]<<24);
	return(retval);
}

int is_opname(char* test_str,int test_strlen)
{
	int is_op = 0;
	int i;
	for (i = SCR_Op_ContactNO; i <= SCR_Op_SD; i++)
	{
		if(SCR_Ops_DataBase[i].enc_string_length == test_strlen)
		{
			if (strcmp(SCR_Ops_DataBase[i].enc_string,test_str)==0)
			{
				is_op = 1;
				break;
			}
		}
	}
	return(is_op);
}

int is_opcode(unsigned short test_num,int* popid)
{
	int is_op = 0;
	int i;
	for (i = SCR_Op_ContactNO; i <= SCR_Op_SD; i++)
	{
		if(SCR_Ops_DataBase[i].code == test_num)
		{
			if(popid) *popid = i;
			is_op = 1;
			break;
		}
	}
	return(is_op);
}

int work_parser_SCR(unsigned char* stream,int size);

int work_parser_SCR(unsigned char* stream,int size)
{
	int i_offset;
	int n_bytes;

	i_offset = CLICKPLC_CKP_SCR_HEADER_SIZE;

	{
		int offset;
		char text[1024];
		int op_number = 1;
		unsigned short opoffset = 0;
		int unknown_cnt = 0;

		offset = i_offset;

		do 
		{		
			if (size - offset >= CPK_MAGICSIGN22_SIZE)
			{
				if (memcmp(stream+offset,CPKSCRMagicSign,CPK_MAGICSIGN_SIZE)==0)
				{
					printf("\n\n\n__________________________________________________________________________________________%d\n### SIGN..\n\t\t\t",op_number++);
					offset+=CPK_MAGICSIGN_SIZE;

					unknown_cnt = 0;
				}
				else
				{
					if (memcmp(stream+offset,CPKSCRMagicSign22,CPK_MAGICSIGN22_SIZE)==0)
					{
						printf("### 0000..\n\t\t\t");
						offset+=CPK_MAGICSIGN2_SIZE;

						unknown_cnt = 0;
					}
					else
					{												
						if (memcmp(stream+offset,CPKSCRMagicSignA,CPK_AAAASIGN_SIZE)==0)
						{
							printf("\n### AAAA..\n\t\t\t");
							offset+=CPK_AAAASIGN_SIZE;

							unknown_cnt = 0;
						}
						else
						{
							if (is_pascal_string(stream+offset,text,&n_bytes))
							{
								//printf("\n{%24s},0x%02x,%d bytes\n",text,text[0],n_bytes);								
								offset+=n_bytes;

								if (is_opname(text,(n_bytes-1)/2))
								{
									unsigned short opcode;
									unsigned short opstep;
									int opid;

									printf("\n\n\n\n[%-20s]\t",text);

									opcode = (unsigned short)stream[offset+0]|((unsigned short)stream[offset+1]<<8);

									if (is_opcode(opcode,&opid))
									{										
										int tsize;
										offset += 2;

										switch(opcode)
										{
										case SCR_OPCODE_TMR:
											opoffset = (unsigned short)stream[offset+9]|((unsigned short)stream[offset+10]<<8);
											tsize = CKP_SCR_TMR_OPHEADER_SIZE;
											break;
										case SCR_OPCODE_CNT:
											opoffset = (unsigned short)stream[offset+10]|((unsigned short)stream[offset+11]<<8);
											tsize = CKP_SCR_CNT_OPHEADER_SIZE;
											break;
										case SCR_OPCODE_SR:
											opoffset = (unsigned short)stream[offset+10]|((unsigned short)stream[offset+11]<<8);
											tsize = CKP_SCR_SR_OPHEADER_SIZE;
											break;
										case SCR_OPCODE_DRUM:
											opoffset = (unsigned short)stream[offset+11]|((unsigned short)stream[offset+12]<<8);
											tsize = CKP_SCR_DRUM_OPHEADER_SIZE;
											break;
										default:
											opoffset = (unsigned short)stream[offset+8]|((unsigned short)stream[offset+9]<<8);
											tsize = CKP_SCR_OPHEADER_SIZE;
										}

										opstep = (unsigned short)stream[offset+2]|((unsigned short)stream[offset+3]<<8);

										printf("op: [%8s]\t%04x\t...%5d\tstep: %4d\n\t\t\t",SCR_Ops_DataBase[opid].name,opcode,opoffset,opstep);

										printf_buf_hex(stream+offset,tsize,0);
										offset += tsize;

										printf("\n\t\t\t");										
									}
								}
								else
								{
									printf("\n[%-20s]\n\t\t\t",text);
								}

								unknown_cnt = 0;

							}
							else
							{
								unknown_cnt++;

								if (opoffset == offset)
								{
									printf("\n$$$\n\t\t\t");
									opoffset = 0;

									unknown_cnt = 0;
								}

								printf("%02x ",stream[offset]);

								if (unknown_cnt && !(unknown_cnt%16))
								{
									printf("\n\t\t\t");
								}

								offset++;
							}
						}						
					}
				}
			}
			else
			{
				printf("%02x ",stream[offset]);
				offset++;
			}

		} while (offset < size);
	}

	return(i_offset);
}

int parser_op(unsigned char* stream,int size,SCR_Operation* op,int scr_offset)
{
	int i_offset = 0;
	int op_name_scr_offset;
	int op_name_op_offset;
	unsigned short op_scr_offset_dec;	
	char text[1024];
	int n_bytes;	

	//printf_buf_hex_N(stream+i_offset,128,"Next bytes:",128);

	/*
	unsigned char		op_pos_row;	//[0]: 1,2,3...255
	unsigned char		op_pos_col;	//[1]: 0,1,2,...31
	//[2,3]: ??? 00,00
	unsigned short		op_index;	//[4,5]
	//[6,7]: ??? 00,00
	*/

	op->op_pos_row = stream[i_offset + 0];
	op->op_pos_col = stream[i_offset + 1];

	i_offset += 2;

	op->unknown1_bytes[0] = stream[i_offset + 0];
	op->unknown1_bytes[1] = stream[i_offset + 1];
	i_offset += 2;
		
	clickplc_ckp_int16_dec(&stream[i_offset+0],&op->op_index);
	i_offset += 2;
	
	op->unknown2_bytes[0] = stream[i_offset + 0];
	op->unknown2_bytes[1] = stream[i_offset + 1];
	i_offset += 2;
	
	if (op->unknown2_bytes[0] != 0 || op->unknown2_bytes[1]!= 0)
	{
		printf("[6,7]: %02x %02x\n",			op->unknown2_bytes[0],op->unknown2_bytes[1]);
	}
	else
	{
		printf("\n");
	}
			
	//////////////////////////////////////////////////////////////////////////
	op_name_op_offset	= i_offset;
	op_name_scr_offset	= scr_offset + i_offset;
	//////////////////////////////////////////////////////////////////////////
	
	if(is_pascal_string(stream + i_offset,text,&n_bytes))
	{
		unsigned short opcode;

		i_offset += n_bytes;											
		op->scr_opcode_offset = i_offset;

		clickplc_ckp_int16_dec(&stream[i_offset+0],&opcode);
		i_offset += 2;

		if (is_opcode(opcode,&op->opid))
		{
#ifdef __RUN_CHUTTY_MODE__
			printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++ $%03d @%04x [%d:%d]\t",
				op->op_index,
				opcode,
				op->op_pos_row,
				op->op_pos_col);
#endif

			if (	op->opid== SCR_Op_Drum 
				||	SCR_Ops_DataBase[op->opid].u1 != op->unknown1_bytes[0] || SCR_Ops_DataBase[op->opid].u2 != op->unknown1_bytes[1])
			{
				printf("\t\t\t%04d %04X  [2,3]: %02x %02x",	op->op_index,opcode,op->unknown1_bytes[0],op->unknown1_bytes[1]);
			}
			
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////PROCESS OPERATION///////////////////////////////
			//////////////////////////////////////////////////////////////////////////

			if (strcmp(text,SCR_Ops_DataBase[op->opid].enc_string))
			{
				printf("%s != %s\n",text,SCR_Ops_DataBase[op->opid].enc_string);
			}

			//printf_buf_hex_16(stream+i_offset,64,"Next bytes:");

			//printf("op_scr_offset:= %04x\n",op_scr_offset);
			//printf_buf_hex_16(stream+op_scr_offset,64,"Op 64 bytes:");

			if (CLICKPLC_SCR_Obj_Decoders[op->opid]!=0)
			{													
				op->body = clickplc_scr_object_calloc(op->opid,&op->body_sizeof);

				//stream_after_opcode
				op->body_enc_size = CLICKPLC_SCR_Obj_Decoders[op->opid](stream+i_offset,op->body,&op_scr_offset_dec);
				i_offset += op->body_enc_size;

				op->body_enc_size += n_bytes + 2;
#ifdef __RUN_CHUTTY_MODE__
				printf("obj_enc_size: %d bytes\n",op->body_enc_size);													
				//printf("op_scr_offset_dec:= %04x\n",op_scr_offset_dec);
				//printf_buf_hex_16(stream+op_scr_offset_dec,64,"Op 64 bytes:");

				clickplc_scr_object_printf(op->opid,op->body);
#endif

				if(CLICKPLC_SCR_Obj_Encoders[op->opid])
				{														
					int enc_size;
					unsigned char enc_object[SCR_OBJECT_MAX_ENC_SIZE];
					SCR_OpBodyAlways *op_info;

					op_info = (SCR_OpBodyAlways*)op->body;
					op_info->scr_offset = op_name_scr_offset;

					//stream_before_opname
					enc_size = CLICKPLC_SCR_Obj_Encoders[op->opid](op->body,enc_object);

					if (	enc_size != op->body_enc_size 
						||	memcmp(enc_object,stream+op_name_op_offset,enc_size))
					{
						clickplc_scr_object_printf(op->opid,op->body);

						printf("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!! OBJECT: ENC(%d bytes) != ORIG(%d bytes) !!!!!!!!!!!!!!!!!!!!!!!!!\n",
							enc_size,op->body_enc_size);
						printf_buf_hex(enc_object,enc_size,"\nENC \t");
						printf_buf_hex(stream+op_name_op_offset,op->body_enc_size,"\nORIG\t");
					}
#ifdef __RUN_CHUTTY_MODE__
					else
					{
						printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tOK\n");
					}
#endif
				}

				clickplc_scr_object_free(op->opid,&op->body);
			}
			else
			{
				printf("!!!!!!!!!!!!!!!!!!!!!!!!!! Not supported operation: [%s]\n",SCR_Ops_DataBase[op->opid].name);
				printf_buf_hex_16(stream+i_offset,64,"First 64 bytes:");
				i_offset = 0;
			}
		}
		else
		{
			printf("$%03d Wrong OpCode @%04x!!!\n",op->op_index,opcode);
			i_offset = 0;
		}
	}
	else
	{
		printf("\nProblem: can't find op-name!\n");
		printf_buf_hex_16(stream+i_offset,64,"...bytes:");
		i_offset = 0;
	}																				

	return(i_offset);
}

int parser_rung(unsigned char* stream,int size,CLICKPLC_CKP_SCR* scr,SCR_Rung* rung,int scr_offset)
{
	int i_offset = 0;
	int n_bytes;

	clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->rung_index);
	i_offset += 2;

	//printf("\n\n\n###############################################################################  RUNG# %4d\n",rung->rung_index+1);
	printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  RUNG# %4d\n",rung->rung_index+1);

	clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->enc_comment_size);
	i_offset += 2;

	rung->unknown1_bytes[0] = stream[i_offset+0];
	rung->unknown1_bytes[1] = stream[i_offset+1];
	i_offset += 2;

	if (rung->unknown1_bytes[0] != 0 || rung->unknown1_bytes[1] != 0)
	{
		printf("\t\t\trung[4,5]: %02x %02x\n",rung->unknown1_bytes[0],rung->unknown1_bytes[1]);	
	}
	
	//rung parsing
	if (rung->enc_comment_size <= CKP_SCR_MAX_COMMENT_SIZE)
	{
		if (rung->enc_comment)	free(rung->enc_comment);
		rung->enc_comment = (unsigned char*)malloc(rung->enc_comment_size);
		memcpy(rung->enc_comment,stream+i_offset,rung->enc_comment_size);
		i_offset += rung->enc_comment_size;

		if(rung->enc_comment_size > 0)
		{
			printf("comments: %4d bytes\t",rung->enc_comment_size);
			printf("%02x %02x %02x %02x %02x %02x ... %02x %02x %02x %02x\n",
				rung->enc_comment[0],rung->enc_comment[1],
				rung->enc_comment[2],rung->enc_comment[3],
				rung->enc_comment[4],rung->enc_comment[5],
				rung->enc_comment[rung->enc_comment_size-4],
				rung->enc_comment[rung->enc_comment_size-3],
				rung->enc_comment[rung->enc_comment_size-2],
				rung->enc_comment[rung->enc_comment_size-1]
			);
		}

		/*
		rung->unknown2_bytes[0] = stream[i_offset+0];
		rung->unknown2_bytes[1] = stream[i_offset+1];
		rung->unknown2_bytes[2] = stream[i_offset+2];
		rung->unknown2_bytes[3] = stream[i_offset+3];
		rung->unknown2_bytes[4] = stream[i_offset+4];
		rung->unknown2_bytes[5] = stream[i_offset+5];
		i_offset += 6;

		//printf("\t\t\trung[6]: %02x %02x %02x %02x %02x %02x\n",
		printf("\t\t\trung[6]: %02x %02x %02x %02x %02x %02x\t\t\t",
			rung->unknown2_bytes[0],rung->unknown2_bytes[1],
			rung->unknown2_bytes[2],rung->unknown2_bytes[3],
			rung->unknown2_bytes[4],rung->unknown2_bytes[5]
		);
		*/

		clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->rung_dim_x);
		i_offset += 2;
		clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->rung_dim_y);
		i_offset += 2;
		rung->u0 = stream[i_offset+0];
		rung->u1 = stream[i_offset+1];
		i_offset += 2;
		
		if (rung->rung_dim_y != 0x0003)	printf("rung->rung_dim_y: %04X\n",rung->rung_dim_y);

		if (rung->u0 != 0x00)	printf("rung->u0: %02X\n",rung->u0);
		if (rung->u1 != 0x01)	printf("rung->u1: %02X\n",rung->u1);
		
__read_magic_sign_size:
		clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->n_magic_sign);

		if (rung->n_magic_sign == 0x0020)
		{
			unsigned short tword;
			int n_unknown,t_unknown;
			int is_found_magic = 0;
			int is_found_zerosign = 0;

			int t_offset;

			i_offset += 2;

			rung->n_magic_sign *= 2;

			if (rung->magic_sign)	free(rung->magic_sign);
			rung->magic_sign = (unsigned char*)malloc(rung->n_magic_sign);
			memcpy(rung->magic_sign,stream+i_offset,rung->n_magic_sign);
			i_offset += rung->n_magic_sign;
			
			t_offset = i_offset;
			n_unknown = 0;
			while(i_offset < size)
			{								
				clickplc_ckp_int16_dec(&stream[i_offset+0],&tword);

				if (tword == scr->magic_word)
				{
					is_found_magic = 1;

					if (n_unknown)
					{
						if (n_unknown!=2 || !is_found_zerosign)
						{
							printf("looking for signs: %3d unknowns:\t",n_unknown);
							printf_buf_hex_16(stream+t_unknown,n_unknown,0);
						}
					}

					break;
				}
				else
				{
					switch(tword)
					{
					case 0x001f:

						if (memcmp(stream+i_offset,CPKSCRMagicTimer2,CPK_MAGICTIMER2_SIZE)==0)
						{
							printf("### Timer2contacts..\n");
							i_offset += CPK_MAGICTIMER2_SIZE;

							if (n_unknown)
							{
								printf("looking for signs: %3d unknowns:\t",n_unknown);
								printf_buf_hex_16(stream+t_unknown,n_unknown,0);
							}

							n_unknown = 0;
						}
						else
						{
							if(n_unknown ==0) t_unknown = i_offset;
							n_unknown++;
							i_offset++;
						}
						break;

					case 0x0020:

						if (memcmp(stream+i_offset,CPKSCRMagicSign,CPK_MAGICSIGN_SIZE)==0)
						{
							printf("### MAGIC..\n");
							i_offset += CPK_MAGICSIGN_SIZE;

							if (n_unknown)
							{
								printf("looking for signs: %3d unknowns:\t",n_unknown);
								printf_buf_hex_16(stream+t_unknown,n_unknown,0);
							}

							n_unknown = 0;
						}
						else
						{
							if (memcmp(stream+i_offset,CPKSCRMagicSign22,CPK_MAGICSIGN22_SIZE)==0)
							{
								is_found_zerosign = 1;
								
								if (n_unknown)
								{
									printf("looking for signs: %3d unknowns:\t",n_unknown);
									printf_buf_hex_16(stream+t_unknown,n_unknown,0);
								}
								
								n_unknown = 0;
								
								printf("%08X (%01d) ### 0000...\n\n\n",scr_offset + i_offset,(scr_offset + i_offset)&0x03);
								i_offset += CPK_MAGICSIGN22_SIZE;
								
								clickplc_ckp_int16_dec(&stream[i_offset+0],&tword);
								
								if (tword == 0x0000)
								{
									i_offset += 2;
									
									//printf("\t\t\t\t\t\t\tNOP\n");
									
									rung->n_ops = 1;
									rung->ops = (SCR_Operation*)calloc(rung->n_ops,sizeof(SCR_Operation));
									rung->ops[0].opid		= SCR_Op_NOP;
									rung->ops[0].op_index	= SCR_OPCODE_NOP;
									rung->ops[0].op_pos_row = 1;
									rung->ops[0].op_pos_col = 31;
									
									rung->ops[0].scr_opcode_offset = i_offset;
									rung->ops[0].body = clickplc_scr_object_calloc(rung->ops[0].opid,&rung->ops[0].body_sizeof);									
									rung->ops[0].body_enc_size = clickplc_scr_NOP_dec(stream+i_offset,rung->ops[0].body,0);
									//i_offset += rung->ops[0].body_enc_size;
									
#ifdef __RUN_CHUTTY_MODE__
									printf("obj_enc_size: %d bytes\n",rung->ops[0].body_enc_size);
									clickplc_scr_object_printf(rung->ops[0].opid,rung->ops[0].body);
#endif

									clickplc_scr_object_free(rung->ops[0].opid,&rung->ops[0].body);
									goto __exit_point;
								}
							}
							else
							{
								//printf("looking for #0000: unknown[%3d] = %02x\n",n_unknown,stream[i_offset + 0]);
								if(n_unknown ==0) t_unknown = i_offset;
								n_unknown++;
								i_offset++;
							}
						}
						break;

					default:
						//printf("looking for #0000: unknown[%3d] = %02x\n",n_unknown,stream[i_offset + 0]);
						if(n_unknown ==0) t_unknown = i_offset;
						n_unknown++;
						i_offset++;
					}
				}				
			};

			//printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t%d bytes shift\n",i_offset-t_offset);
			//printf_buf_ascii_16(stream + t_offset,i_offset-t_offset,"\nin gap:\n");

			//if (tword == scr->magic_word || tword == 0x0000)
			if(is_found_magic)
			{
				i_offset -= 2;

				clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->n_ops);
				i_offset += 2;

				if (rung->n_ops > 0)
				{
					int i_op;

					rung->magic_word = scr->magic_word;
					i_offset += 2;

					rung->unknown4_bytes[0] = stream[i_offset + 0];
					rung->unknown4_bytes[1] = stream[i_offset + 1];
					i_offset += 2;

					printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t%d operation(s)\n",rung->n_ops);

					////////////////////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////OPERATIONS////////////////////////////////////////////////////////////
					////////////////////////////////////////////////////////////////////////////////////////////////////
					//if (rung->ops)	free(rung->ops);
					rung->ops = (SCR_Operation*)calloc(rung->n_ops,sizeof(SCR_Operation));

					for (i_op = 0; i_op < rung->n_ops; i_op++)
					{										
						//////////////////////////////////////////////////////////////////////////
						//parse op
						n_bytes = parser_op(stream + i_offset,size - i_offset,&rung->ops[i_op],scr_offset + i_offset);
						//////////////////////////////////////////////////////////////////////////
						if (n_bytes < 1)
						{
							i_offset = 0;
							goto __exit_point;
						}
						i_offset += n_bytes;
						//////////////////////////////////////////////////////////////////////////
					}
				}
				else
				{
					printf("rung->n_ops <= 0\n");
				}

			}
			else
			{
				printf("Can't find rung's magic word!\n");
			}
		}
		else
		{
			//printf("scr->n_magic_sign = %d\n",rung->n_magic_sign);
			{
				int n_bytes;
				char text[1025];

				if (is_pascal_string(stream + i_offset,text,&n_bytes))
				{
					printf("unknown string (enc in %d bytes): [%s]\n",n_bytes,text);
					i_offset += n_bytes;
				}
				else
				{
					printf("looking for #MAGIC unknown = %02x\n",stream[i_offset + 0]);

					if (i_offset < size)
					{
						i_offset++;
						goto __read_magic_sign_size;
					}
				}
			}
		}
	}
	else
	{
		printf("rung->enc_comment_size = %d! > %d\n",rung->enc_comment_size,CKP_SCR_MAX_COMMENT_SIZE);
		i_offset = 0;
	}

__exit_point:
	return(i_offset);
}

int parser_SCR_v1(unsigned char* stream,int size,CLICKPLC_CKP_SCR* scr)
{
	int i_offset = 0;
	int goon;

#ifdef __RUN_TALKY_MODE__		
	return work_parser_SCR(stream,size);
#endif

	goon = 1;
	do 
	{
		int n_bytes;

		switch(i_offset)
		{
		case 0:
			//[0]...[7]:	SC-SCR<space><space>;	8 bytes
			//[8,9,10,11]: 4 bytes example:	0x00, 0x00, 0x1E, 0x01 -> 30,1 -> 1.30;
			scr->version[0] = stream[8+0];
			scr->version[1] = stream[8+1];
			scr->version[2] = stream[8+2];
			scr->version[3] = stream[8+3];
			i_offset = 12;
			break;

		case 12:
			//[12]...[63]:	0x00;				52 bytes
			memcpy(scr->zero_bytes2,stream+12,CLICKPLC_CKP_SCR_HEADER_SIZE - 12);
			{
				int i;
				for (i = 0; i < CLICKPLC_CKP_SCR_HEADER_SIZE - 12; i++)
				{
					if (scr->zero_bytes2[i]!=0x00)
					{
						printf("scr->zero_bytes2[%d] = 0x%02x\n",i,scr->zero_bytes2[i]);
					}
				}
			}
			i_offset = CLICKPLC_CKP_SCR_HEADER_SIZE;
			break;

		case CLICKPLC_CKP_SCR_HEADER_SIZE:

			clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->scr_id);
			i_offset += 2;

			printf("SCR%4d\t",scr->scr_id);

			if (is_pascal_string(stream+i_offset,scr->name,&n_bytes))
			{
				i_offset += n_bytes;

				printf("%-24s\t",scr->name);

				clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->n_padding_As);

				if (scr->n_padding_As == 0x0020)
				{
					i_offset += 2;

					scr->n_padding_As *= 2;
					scr->n_padding_As -= 2;

					if (scr->padding_As)	free(scr->padding_As);
					scr->padding_As = (unsigned char*)malloc(scr->n_padding_As);
					memcpy(scr->padding_As,stream + i_offset,scr->n_padding_As);
					i_offset += scr->n_padding_As;

					clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->magic_word);
					i_offset += 2;

					printf("0x%04x\t",scr->magic_word);

					scr->unknown_byte = stream[i_offset+0];
					i_offset++;

					printf("0x%02x\t",scr->unknown_byte);

					clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->n_rungs);
					i_offset += 2;

					printf("%4d rung(s)\n",scr->n_rungs);
				}
				else
				{
					printf("scr->n_padding_As = %d\n",scr->n_padding_As);

					goon = 0;
				}
			}
			else
			{
				printf("scr->name!!!\n");

				goon = 0;
			}
			break;

		default:
			//////////////////////////////RUNGS/////////////////////////////////////////////////////////////////
			if (scr->n_rungs > 0)
			{
				int i_rung;

				if (scr->rungs)	free(scr->rungs);
				scr->rungs = (SCR_Rung*)calloc(scr->n_rungs,sizeof(SCR_Rung));

				for (i_rung = 0; i_rung < scr->n_rungs; i_rung++)
				{	
					//////////////////////////////////////////////////////////////////////////
					//parse rung
					//////////////////////////////////////////////////////////////////////////
					n_bytes = parser_rung(stream + i_offset,size - i_offset,scr,&scr->rungs[i_rung],i_offset);
					//////////////////////////////////////////////////////////////////////////
					if (n_bytes < 1)
					{
						goon = 0;
						break;
					}
					i_offset += n_bytes;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////
			if (goon)
			{
				if (scr->scr_id == 1)
				{
					scr->n_steps_mainonly = (unsigned short)stream[size-2]|((unsigned short)stream[size-1]<<8);

					printf("%4d step(s)\n",scr->n_steps_mainonly);
				}
				else
				{
					scr->n_steps_mainonly = 0;
					printf("\n");
				}
				goon = 0;
			}
			//end of default
		}//switch offset

	} while (goon && i_offset < size);

	return(i_offset);
}

int parser_SCR(unsigned char* stream,int size,CLICKPLC_CKP_SCR* scr)
{
	int i_offset;
	int n_bytes;
	char text[1024];

#ifdef __RUN_TALKY_MODE__		
	return work_parser_SCR(stream,size);
#endif
	//[0]...[7]:	SC-SCR<space><space>;	8 bytes
	//[8,9,10,11]: 4 bytes example:	0x00, 0x00, 0x1E, 0x01 -> 30,1 -> 1.30;
	scr->version[0] = stream[8+0];
	scr->version[1] = stream[8+1];
	scr->version[2] = stream[8+2];
	scr->version[3] = stream[8+3];

	//[12]...[63]:	0x00;				52 bytes
	memcpy(scr->zero_bytes2,stream+12,52);
	{
		int i;

		for (i = 0; i < 52; i++)
		{
			if (scr->zero_bytes2[i]!=0x00)
			{
				printf("scr->zero_bytes2[%d] = 0x%02x\n",i,scr->zero_bytes2[i]);
			}
		}
	}
	i_offset = CLICKPLC_CKP_SCR_HEADER_SIZE;
		
	clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->scr_id);
	i_offset += 2;

	printf("SCR%4d\t",scr->scr_id);
	
	if (is_pascal_string(stream+i_offset,scr->name,&n_bytes))
	{
		i_offset += n_bytes;

		printf("%-24s\t",scr->name);
				
		clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->n_padding_As);

		if (scr->n_padding_As == 0x0020)
		{
			i_offset += 2;

			scr->n_padding_As *= 2;
			scr->n_padding_As -= 2;

			if (scr->padding_As)	free(scr->padding_As);
			scr->padding_As = (unsigned char*)malloc(scr->n_padding_As);
			memcpy(scr->padding_As,stream + i_offset,scr->n_padding_As);
			i_offset += scr->n_padding_As;
						
			clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->magic_word);
			i_offset += 2;

			printf("0x%04x\t",scr->magic_word);

			scr->unknown_byte = stream[i_offset+0];
			i_offset++;

			printf("0x%02x\t",scr->unknown_byte);
						
			clickplc_ckp_int16_dec(&stream[i_offset+0],&scr->n_rungs);
			i_offset += 2;

			printf("%4d rung(s)\n",scr->n_rungs);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////RUNGS/////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			if (scr->n_rungs > 0)
			{
				int i_rung;

				if (scr->rungs)	free(scr->rungs);
				scr->rungs = (SCR_Rung*)calloc(scr->n_rungs,sizeof(SCR_Rung));

				for (i_rung = 0; i_rung < scr->n_rungs; i_rung++)
				{
					SCR_Rung *rung;

					rung = &scr->rungs[i_rung];
					
					clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->rung_index);
					i_offset += 2;
					
					printf("\n\n\n###############################################################################  RUNG# %4d\n",rung->rung_index+1);
										
					clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->enc_comment_size);
					i_offset += 2;
					
					rung->unknown1_bytes[0] = stream[i_offset+0];
					rung->unknown1_bytes[1] = stream[i_offset+1];
					i_offset += 2;

					printf("\t\t\t[2]: %02x %02x\n",rung->unknown1_bytes[0],rung->unknown1_bytes[1]);
					
					if (rung->enc_comment_size <= CKP_SCR_MAX_COMMENT_SIZE)
					{
						if (rung->enc_comment)	free(rung->enc_comment);
						rung->enc_comment = (unsigned char*)malloc(rung->enc_comment_size);
						memcpy(rung->enc_comment,stream+i_offset,rung->enc_comment_size);
						i_offset += rung->enc_comment_size;
												
						if(rung->enc_comment_size > 0)
						{
							printf("comments: %4d bytes\t",rung->enc_comment_size);
							printf("%02x %02x %02x %02x %02x %02x ... %02x %02x %02x %02x\n",
									rung->enc_comment[0],rung->enc_comment[1],
									rung->enc_comment[2],rung->enc_comment[3],
									rung->enc_comment[4],rung->enc_comment[5],
									rung->enc_comment[rung->enc_comment_size-4],
									rung->enc_comment[rung->enc_comment_size-3],
									rung->enc_comment[rung->enc_comment_size-2],
									rung->enc_comment[rung->enc_comment_size-1]
									);
						}
/*
						rung->unknown2_bytes[0] = stream[i_offset+0];
						rung->unknown2_bytes[1] = stream[i_offset+1];
						rung->unknown2_bytes[2] = stream[i_offset+2];
						rung->unknown2_bytes[3] = stream[i_offset+3];
						rung->unknown2_bytes[4] = stream[i_offset+4];
						rung->unknown2_bytes[5] = stream[i_offset+5];
						
						printf("\t\t\t[6]: %02x %02x %02x %02x %02x %02x\n",
								rung->unknown2_bytes[0],rung->unknown2_bytes[1],
								rung->unknown2_bytes[2],rung->unknown2_bytes[3],
								rung->unknown2_bytes[4],rung->unknown2_bytes[5]
								);
*/
						i_offset += 6;

__read_magic_sign_size:
						clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->n_magic_sign);
						
						if (rung->n_magic_sign == 0x0020)
						{
							unsigned short tword;
							int is_found_magic = 0;

							int t_offset;

							i_offset += 2;

							rung->n_magic_sign *= 2;
							
							if (rung->magic_sign)	free(rung->magic_sign);
							rung->magic_sign = (unsigned char*)malloc(rung->n_magic_sign);
							memcpy(rung->magic_sign,stream+i_offset,rung->n_magic_sign);
							i_offset += rung->n_magic_sign;
							
/*
							t_offset = i_offset;
							
							while(i_offset < size)
							{								
								clickplc_ckp_int16_dec(&stream[i_offset+0],&tword);

								if (tword == scr->magic_word)
								{
									break;
								}

								i_offset++;
							};
							
							printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t%d bytes shift\n",i_offset-t_offset);

							printf_buf_ascii_16(stream + t_offset,i_offset-t_offset,"\nin gap:\n");
*/
							
							t_offset = i_offset;

							while(i_offset < size)
							{								
								clickplc_ckp_int16_dec(&stream[i_offset+0],&tword);

								if (tword == scr->magic_word)
								{
									is_found_magic = 1;
									break;
								}
								else
								{
									if (memcmp(stream+i_offset,CPKSCRMagicSign22,CPK_MAGICSIGN22_SIZE)==0)
									{
										printf("### 0000..\n");
										i_offset += CPK_MAGICSIGN22_SIZE;
										
										clickplc_ckp_int16_dec(&stream[i_offset+0],&tword);

										if (tword == 0x0000)
										{
											i_offset += 2;

											printf("\t\t\t\t\t\t\tNOP\n");

											goto __next_rung_by_nop;
										}
									}
									else
									{
										if (memcmp(stream+i_offset,CPKSCRMagicSign,CPK_MAGICSIGN_SIZE)==0)
										{
											printf("### MAGIC..\n");
											i_offset += CPK_MAGICSIGN_SIZE - 1;
										}
										else
										{
											printf("looking for zerosign: unknown = %02x\n",stream[i_offset + 0]);
										}
									}

								}

								i_offset++;
							};

							//printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t%d bytes shift\n",i_offset-t_offset);
							//printf_buf_ascii_16(stream + t_offset,i_offset-t_offset,"\nin gap:\n");

							//if (tword == scr->magic_word || tword == 0x0000)
							if(is_found_magic)
							{
								i_offset -= 2;

								clickplc_ckp_int16_dec(&stream[i_offset+0],&rung->n_ops);
								i_offset += 2;

								if (rung->n_ops > 0)
								{
									int i_op;

									rung->magic_word = scr->magic_word;
									i_offset += 2;

									rung->unknown4_bytes[0] = stream[i_offset + 0];
									rung->unknown4_bytes[1] = stream[i_offset + 1];
									i_offset += 2;
									
									printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t%d operation(s)\n",rung->n_ops);

									////////////////////////////////////////////////////////////////////////////////////////////////////
									//////////////////////////////OPERATIONS////////////////////////////////////////////////////////////
									////////////////////////////////////////////////////////////////////////////////////////////////////
									//if (rung->ops)	free(rung->ops);
									rung->ops = (SCR_Operation*)calloc(rung->n_ops,sizeof(SCR_Operation));

									for (i_op = 0; i_op < rung->n_ops; i_op++)
									{										
										SCR_Operation* op;
										int op_scr_offset;
										unsigned short op_scr_offset_dec;
										int op_name_scr_offset;
																														
										op = &rung->ops[i_op];

										//////////////////////////////////////////////////////////////////////////
										op_scr_offset = i_offset;
										//////////////////////////////////////////////////////////////////////////
										
										//printf_buf_hex_N(stream+i_offset,128,"Next bytes:",128);
										
										/*
										unsigned char		op_pos_row;	//[0]: 1,2,3...255
										unsigned char		op_pos_col;	//[1]: 0,1,2,...31
										//[2,3]: ??? 00,00
										unsigned short		op_index;	//[4,5]
										//[6,7]: ??? 00,00
										*/

										op->op_pos_row = stream[i_offset + 0];
										op->op_pos_col = stream[i_offset + 1];
										
										i_offset += 2;

										op->unknown1_bytes[0] = stream[i_offset + 0];
										op->unknown1_bytes[1] = stream[i_offset + 1];
										i_offset += 2;
										
										clickplc_ckp_int16_dec(&stream[i_offset+0],&op->op_index);
										i_offset += 2;

										op->unknown2_bytes[0] = stream[i_offset + 0];
										op->unknown2_bytes[1] = stream[i_offset + 1];
										i_offset += 2;
										
										//////////////////////////////////////////////////////////////////////////
										op_name_scr_offset = i_offset;
										//////////////////////////////////////////////////////////////////////////
										
										if(is_pascal_string(stream + i_offset,text,&n_bytes))
										{
											unsigned short opcode;
											
											i_offset += n_bytes;											
											op->scr_opcode_offset = i_offset;

											printf("\t\t\t[2]: %02x %02x   ",	op->unknown1_bytes[0],op->unknown1_bytes[1]);
											printf("[2]: %02x %02x\n",			op->unknown2_bytes[0],op->unknown2_bytes[1]);																					
											
											clickplc_ckp_int16_dec(&stream[i_offset+0],&opcode);
											i_offset += 2;
											
											if (is_opcode(opcode,&op->opid))
											{
												printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++ $%03d @%04x [%d:%d]\t",
													op->op_index,
													opcode,
													op->op_pos_row,
													op->op_pos_col);

												//////////////////////////////////////////////////////////////////////////
												//////////////////////////PROCESS OPERATION///////////////////////////////
												//////////////////////////////////////////////////////////////////////////

												if (strcmp(text,SCR_Ops_DataBase[op->opid].enc_string))
												{
													printf("%s != %s\n",text,SCR_Ops_DataBase[op->opid].enc_string);
												}

												//printf_buf_hex_16(stream+i_offset,64,"Next bytes:");

												//printf("op_scr_offset:= %04x\n",op_scr_offset);
												//printf_buf_hex_16(stream+op_scr_offset,64,"Op 64 bytes:");
																								
												if (CLICKPLC_SCR_Obj_Decoders[op->opid]!=0)
												{													
													op->body = clickplc_scr_object_calloc(op->opid,&op->body_sizeof);
													
													//stream_after_opcode
													op->body_enc_size = CLICKPLC_SCR_Obj_Decoders[op->opid](stream+i_offset,op->body,&op_scr_offset_dec);
													i_offset += op->body_enc_size;

													op->body_enc_size += n_bytes + 2;
													printf("obj_enc_size: %d bytes\n",op->body_enc_size);													

													//printf("op_scr_offset_dec:= %04x\n",op_scr_offset_dec);
													//printf_buf_hex_16(stream+op_scr_offset_dec,64,"Op 64 bytes:");
																										
													clickplc_scr_object_printf(op->opid,op->body);

													if(CLICKPLC_SCR_Obj_Encoders[op->opid])
													{														
														int enc_size;
														unsigned char enc_object[SCR_OBJECT_MAX_ENC_SIZE];
														SCR_OpBodyAlways *op_info;
														
														op_info = (SCR_OpBodyAlways*)op->body;
														op_info->scr_offset = op_name_scr_offset;

														//stream_before_opname
														enc_size = CLICKPLC_SCR_Obj_Encoders[op->opid](op->body,enc_object);

														if (	enc_size != op->body_enc_size 
															||	memcmp(enc_object,stream+op_name_scr_offset,enc_size))
														{
															printf("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!! OBJECT: ENC(%d bytes) != ORIG(%d bytes) !!!!!!!!!!!!!!!!!!!!!!!!!\n",
																enc_size,op->body_enc_size);
															printf_buf_hex(enc_object,enc_size,"\nENC \t");
															printf_buf_hex(stream+op_name_scr_offset,op->body_enc_size,"\nORIG\t");
														}
														else
														{
															printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tOK\n");
														}
													}

													clickplc_scr_object_free(op->opid,&op->body);
												}
												else
												{
													printf_buf_hex_16(stream+i_offset,64,"Not supported operation. First 64 bytes:");
/*
													{
														int tsize;
														int opstep;
														int opoffset;
																											
														switch(opcode)
														{
														case SCR_OPCODE_TMR:
															opoffset = (unsigned short)stream[i_offset+9]|((unsigned short)stream[i_offset+10]<<8);
															tsize = CKP_SCR_TMR_OPHEADER_SIZE;
															break;
														case SCR_OPCODE_CNT:
															opoffset = (unsigned short)stream[i_offset+10]|((unsigned short)stream[i_offset+11]<<8);
															tsize = CKP_SCR_CNT_OPHEADER_SIZE;
															break;
														case SCR_OPCODE_SR:
															opoffset = (unsigned short)stream[i_offset+10]|((unsigned short)stream[i_offset+11]<<8);
															tsize = CKP_SCR_SR_OPHEADER_SIZE;
															break;
														case SCR_OPCODE_DRUM:
															opoffset = (unsigned short)stream[i_offset+11]|((unsigned short)stream[i_offset+12]<<8);
															tsize = CKP_SCR_DRUM_OPHEADER_SIZE;
															break;
														default:
															opoffset = (unsigned short)stream[i_offset+8]|((unsigned short)stream[i_offset+9]<<8);
															tsize = CKP_SCR_OPHEADER_SIZE;
														}

														opstep = (unsigned short)stream[i_offset+2]|((unsigned short)stream[i_offset+3]<<8);

														//printf("op: [%8s]\t%04x\t...%5d\tstep: %4d\n\t\t\t",SCR_Ops_DataBase[opid].name,opcode,opoffset,opstep);

														printf_buf_hex(stream+i_offset,tsize,0);
														i_offset += tsize;

														printf("\n\t\t\t");
													}
*/
												}
											}
											else
											{
												printf("$%03d Wrong OpCode @%04x!!!\n",op->op_index,opcode);
											}
										}
										else
										{
											printf("\nProblem: can't find op-name!\n");
											printf_buf_hex_16(stream+i_offset,64,"...bytes:");
										}																				
									}
								}
								else
								{
									printf("\t\t\tNOP\n");
								}

							}
							else
							{
								printf("Can't find rung's magic word!\n");
							}
						}
						else
						{
							//printf("scr->n_magic_sign = %d\n",rung->n_magic_sign);
							{
								int n_bytes;
								char text[1025];
								
								if (is_pascal_string(stream + i_offset,text,&n_bytes))
								{
									printf("unknown string (enc in %d bytes): [%s]\n",n_bytes,text);
									i_offset += n_bytes;
								}
								else
								{
									printf("unknown = %02x\n",stream[i_offset + 0]);

									if (i_offset < size)
									{
										i_offset++;
										goto __read_magic_sign_size;
									}
								}
							}
						}

					}
					else
					{
						printf("rung->enc_comment_size = %d! > %d\n",rung->enc_comment_size,CKP_SCR_MAX_COMMENT_SIZE);
					}

					//break;//TE#STS: one rung only
__next_rung_by_nop:;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////

			if (scr->scr_id == 1)
			{
				scr->n_steps_mainonly = (unsigned short)stream[size-2]|((unsigned short)stream[size-1]<<8);

				printf("%4d step(s)\n",scr->n_steps_mainonly);
			}
			else
			{
				scr->n_steps_mainonly = 0;
				printf("\n");
			}					
		}
		else
		{
			printf("scr->n_padding_As = %d\n",scr->n_padding_As);
		}
	}
	else
	{
		printf("scr->name!!!\n");
	}
	return(i_offset);
}

void destroy_SCR(CLICKPLC_CKP_SCR* scr)
{
	if (scr)
	{
		
		if (scr->padding_As)
		{
			free(scr->padding_As);
			scr->padding_As = 0;
		}		

/*		
		if (scr->padding_0s)
		{
			free(scr->padding_0s);
			scr->padding_0s = 0;
		}
*/
		
		if (scr->rungs)
		{
			int i_rung;

			for(i_rung = 0; i_rung < scr->n_rungs; i_rung++)
			{
				if (scr->rungs[i_rung].enc_comment)
				{
					free(scr->rungs[i_rung].enc_comment);
					scr->rungs[i_rung].enc_comment = 0;
				}

				if (scr->rungs[i_rung].magic_sign)
				{
					free(scr->rungs[i_rung].magic_sign);
					scr->rungs[i_rung].magic_sign = 0;
				}

				if (scr->rungs[i_rung].padding_0s)
				{
					free(scr->rungs[i_rung].padding_0s);
					scr->rungs[i_rung].padding_0s = 0;
				}

				if (scr->rungs[i_rung].ops)
				{
					int i_op;

					for (i_op = 0; i_op < scr->rungs[i_rung].n_ops; i_op++)
					{
						if (scr->rungs[i_rung].ops[i_op].body)
						{
							free(scr->rungs[i_rung].ops[i_op].body);
							scr->rungs[i_rung].ops[i_op].body = 0;
						}
					}

					free(scr->rungs[i_rung].ops);
					scr->rungs[i_rung].ops = 0;
				}
			}
			
			free(scr->rungs);
			scr->rungs = 0;
		}
		
		//free(scr);
	}

	return;
}

/*
	case SCR_Op_ContactNO:	//"ContactNO"
	case SCR_Op_ContactNC:	//"ContactNO"
	case SCR_Op_Edge:			//"Edge"
	case SCR_Op_Compare:			//"Compare"
	case SCR_Op_Out:				//"Out"
	case SCR_Op_Set:				//"Out"
	case SCR_Op_Rst:				//"Out"
	case SCR_Op_Timer:			//"Tmr"
	case SCR_Op_Counter:			//"Cnt"
	case SCR_Op_Math:			//"Math"
	case SCR_Op_Drum:			//"Drum"
	case SCR_Op_ShiftReg:		//"SR"
	case SCR_Op_Copy:			//"Copy"
	case SCR_Op_Search:			//"Search"
	case SCR_Op_Call:			//"Call"
	case SCR_Op_Return:			//"Return"
	case SCR_Op_For:				//"For"
	case SCR_Op_Next:			//"Next"
	case SCR_Op_End:				//"End"
	case SCR_Op_RD:				//"RD"
	case SCR_Op_SD:				//"SD"
	case SCR_Op_NOP				//
*/

void*	clickplc_scr_object_calloc		(int object_id,int* pobject_sizeof)
{
	void* obj = 0;
	int objsizeof = sizeof(SCR_OpBodyAlways);

	switch(object_id)
	{
	case SCR_Op_ContactNO:	//"ContactNO"
	case SCR_Op_ContactNC:	//"ContactNO"
		objsizeof = sizeof(SCR_Operation_Contact);
		break;

	case SCR_Op_Edge:		//"Edge"
		objsizeof = sizeof(SCR_Operation_Edge);
		break;

	case SCR_Op_Compare:	//"Compare"
		objsizeof = sizeof(SCR_Operation_Compare);
		break;

	case SCR_Op_Out:		//"Out"
		objsizeof = sizeof(SCR_Operation_Out);
		break;

	case SCR_Op_Set:		//"Out"
	case SCR_Op_Rst:		//"Out"
		objsizeof = sizeof(SCR_Operation_SetRst);
		break;
	
	case SCR_Op_Timer:		//"Tmr"
		objsizeof = sizeof(SCR_Operation_Timer);
		break;

	case SCR_Op_Counter:	//"Cnt"
		objsizeof = sizeof(SCR_Operation_Counter);
		break;

	case SCR_Op_Math:		//"Math"
		objsizeof = sizeof(SCR_Operation_Math);
		break;

	case SCR_Op_Drum:		//"Drum"
		break;
	case SCR_Op_ShiftReg:	//"SR"
		objsizeof = sizeof(SCR_Operation_ShiftRegister);
		break;
	case SCR_Op_Copy:		//"Copy"
		objsizeof = sizeof(SCR_Operation_Copy);
		break;
	case SCR_Op_Search:		//"Search"
		objsizeof = sizeof(SCR_Operation_Search);
		break;
	case SCR_Op_Call:		//"Call"
		objsizeof = sizeof(SCR_Operation_Call);
		break;
	case SCR_Op_Return:		//"Return"
		objsizeof = sizeof(SCR_Operation_Return);
		break;
	case SCR_Op_For:		//"For"
		objsizeof = sizeof(SCR_Operation_For);
		break;
	case SCR_Op_Next:		//"Next"
		objsizeof = sizeof(SCR_Operation_Next);
		break;
	case SCR_Op_End:		//"End"
		objsizeof = sizeof(SCR_Operation_End);
		break;
	case SCR_Op_RD:			//"RD"
	case SCR_Op_SD:			//"SD"
		break;
	case SCR_Op_NOP:		//
		objsizeof = sizeof(SCR_Operation_NOP);
		break;
	default:
		//nothing
		objsizeof = 0;
		break;
	}
	if (objsizeof)
	{
		obj = calloc(1,objsizeof);
	}
	if(*pobject_sizeof) *pobject_sizeof = objsizeof;
	return(obj);
}

void	clickplc_scr_object_free		(int object_id,void** pobject)
{
	if (*pobject)
	{
		free(*pobject);
		*pobject = 0;
	}
	return;
}

void	clickplc_scr_object_printf		(int object_id,void* object)
{
	printf("\t\t\t\t\t\t");

	if (object)
	{
		SCR_OpBodyAlways* info = (SCR_OpBodyAlways*)object;

		printf("row[%2d] %3d step %04x offset \t\t\t%s",
			info->scr_pos_row,
			info->exe_step,
			info->scr_offset,
			SCR_Ops_DataBase[object_id].name
			);
	}

	switch(object_id)
	{
	case SCR_Op_Out:
		{
			SCR_Operation_Out* body = (SCR_Operation_Out*)object;
						
			if (body->is_range==0)
			{
				printf("[__%s%d__] %c%c\n",
					CLICK_EXE_RegDataBase[body->first_bit_reg_ref.reg_id].txtID,
					body->first_bit_reg_ref.number,
					body->is_one_shot? '!':' ',
					body->is_immediate? '*':' '
					);
			}
			else
			{
				printf("(__%s%d-%s%d__) %c%c\n",
					CLICK_EXE_RegDataBase[body->first_bit_reg_ref.reg_id].txtID,
					body->first_bit_reg_ref.number,
					CLICK_EXE_RegDataBase[body->last_bit_reg_ref.reg_id].txtID,
					body->last_bit_reg_ref.number,
					body->is_one_shot? '!':' ',
					body->is_immediate? '*':' '
					);
			}
		}														
		break;

	case SCR_Op_Set:
	case SCR_Op_Rst:
		{
			SCR_Operation_SetRst* body = (SCR_Operation_SetRst*)object;

			if (body->is_range==0)
			{
				printf("[__%s%d__] %c\n",
					CLICK_EXE_RegDataBase[body->first_bit_reg_ref.reg_id].txtID,
					body->first_bit_reg_ref.number,				
					body->is_immediate? '*':' '
					);
			}
			else
			{
				printf("(__%s%d-%s%d__) %c\n",
					CLICK_EXE_RegDataBase[body->first_bit_reg_ref.reg_id].txtID,
					body->first_bit_reg_ref.number,
					CLICK_EXE_RegDataBase[body->last_bit_reg_ref.reg_id].txtID,
					body->last_bit_reg_ref.number,					
					body->is_immediate? '*':' '
					);
			}
		}														
		break;

	case SCR_Op_Call:
		{
			SCR_Operation_Call* body = (SCR_Operation_Call*)object;

			printf("[__%s__] %d\n",
				body->calling_scr_name,
				body->file_list_index
				);
		}
		break;

	case SCR_Op_ContactNO:
	case SCR_Op_ContactNC:
		{
			SCR_Operation_Contact* body = (SCR_Operation_Contact*)object;

			printf("(__%s%d__) %c\n",
				CLICK_EXE_RegDataBase[body->bit_reg_ref.reg_id].txtID,
				body->bit_reg_ref.number,
				body->is_immediate? '*':' '
				);
		}
		break;

	case SCR_Op_Edge:
		{
			SCR_Operation_Edge* body = (SCR_Operation_Edge*)object;

			printf("(__%s%d__) %c\n",
				CLICK_EXE_RegDataBase[body->bit_reg_ref.reg_id].txtID,
				body->bit_reg_ref.number,
				body->is_down? '-':'+'
				);
		}
		break;
	case SCR_Op_Compare:
		{
			SCR_Operation_Compare* body = (SCR_Operation_Compare*)object;
			
			if (body->is_A_const)
			{
				printf("(%d",body->cmp_A_const);
			}
			else
			{
				printf("(%s%d",	CLICK_EXE_RegDataBase[body->cmp_A_reg_ref.reg_id].txtID,body->cmp_A_reg_ref.number);
			}

			switch(body->cond_type)
			{
			case SCR_CODE_SUBINSTR_COMPARE_EQUAL:
				printf(" == ");
				break;
			case SCR_CODE_SUBINSTR_COMPARE_NOTEQUAL:
				printf(" != ");
				break;
			case SCR_CODE_SUBINSTR_COMPARE_MORE:
				printf(" > ");
				break;
			case SCR_CODE_SUBINSTR_COMPARE_LESS:
				printf(" < ");
				break;
			case SCR_CODE_SUBINSTR_COMPARE_MOREOREQUAL:
				printf(" >= ");
				break;
			case SCR_CODE_SUBINSTR_COMPARE_LESSOREQUAL:
				printf(" <= ");
				break;
			}

			if (body->is_B_const)
			{
				printf("%d)\n",body->cmp_B_const);
			}
			else
			{
				printf("%s%d)\n",	CLICK_EXE_RegDataBase[body->cmp_B_reg_ref.reg_id].txtID,body->cmp_B_reg_ref.number);
			}

		}
		break;
	case SCR_Op_End:
	case SCR_Op_Return:
	case SCR_Op_Next:
		printf("\n");
		break;

	case SCR_Op_For:
		{
			SCR_Operation_For* body = (SCR_Operation_For*)object;
			
			if (body->is_variable)
			{
				printf("[%s%d cycles]",CLICK_EXE_RegDataBase[body->variable_cycles_ref.reg_id].txtID,body->variable_cycles_ref.number);
			}
			else
			{
				printf("[%d cycles]",body->const_cycles);
			}
			printf(" %c\n",body->is_one_shot? '!':' ');
		}		
		break;

	case SCR_Op_Copy:
		{
			SCR_Operation_Copy* body = (SCR_Operation_Copy*)object;
			
			printf(" %c\n",body->is_one_shot? '!':' ');
		}		
		break;

	case SCR_Op_ShiftReg:
		{
			SCR_Operation_ShiftRegister* body = (SCR_Operation_ShiftRegister*)object;

			printf("(__%s%d ... %s%d__)\n",
				CLICK_EXE_RegDataBase[body->start_ref.reg_id].txtID,
				body->start_ref.number,
				CLICK_EXE_RegDataBase[body->end_ref.reg_id].txtID,
				body->end_ref.number
				);
		}
		break;
	case SCR_Op_Counter:
		{
			SCR_Operation_Counter* body = (SCR_Operation_Counter*)object;

			printf("(__%s%d[%s%d]__) sp: ",
				CLICK_EXE_RegDataBase[body->out_complete_bit_ref.reg_id].txtID,
				body->out_complete_bit_ref.number,
				CLICK_EXE_RegDataBase[body->counter_reg_ref.reg_id].txtID,
				body->counter_reg_ref.number				
				);
			if(body->is_sp_const_number)
			{
				printf("%d\n",body->sp_const_number);
			}
			else
			{
				printf("%s%d\n",CLICK_EXE_RegDataBase[body->sp_reg_ref.reg_id].txtID,	body->sp_reg_ref.number);
			}
		}
		break;

	case SCR_Op_Timer:
		{
			SCR_Operation_Timer* body = (SCR_Operation_Timer*)object;

			printf("(__%s%d[%s%d]__) sp: ",
				CLICK_EXE_RegDataBase[body->timer_bit_ref.reg_id].txtID,
				body->timer_bit_ref.number,
				CLICK_EXE_RegDataBase[body->counter_reg_ref.reg_id].txtID,
				body->counter_reg_ref.number
				);
			if(body->is_sp_const_number)
			{
				printf("%d, ",body->sp_const_number);
			}
			else
			{
				printf("%s%d, ",CLICK_EXE_RegDataBase[body->sp_reg_ref.reg_id].txtID,	body->sp_reg_ref.number);
			}

			switch(body->units)
			{
			case TIMER_UNITS_MS:	//ms		N/A
				printf("ms\t");
				break;
			case TIMER_UNITS_SEC:	//sec		f9 21 01
				printf("sec\t");
				break;
			case TIMER_UNITS_MIN:	//min		f9 21 02
				printf("min\t");
				break;
			case TIMER_UNITS_HOUR:	//hrs		f9 21 03
				printf("hour(s)\t");
				break;
			case TIMER_UNITS_DAY:	//days		f9 21 04
				printf("day(s)\t");
				break;
			}

			switch(body->flags)
			{
			case 0x00://ON NotRetained
				printf("ON NotRet.\n");
				break;
			case (0x00|TIMER_FLAGS_RETAINED):	//ON + Retained
				printf("ON Ret.\n");
				break;
			case (0x00|TIMER_FLAGS_OFFDELAY):	//OFF + Not Retained
				printf("OFF NotRet.\n");
				break;
			}
		}
		break;

	case SCR_Op_Search:
		{
			SCR_Operation_Search* body = (SCR_Operation_Search*)object;

			printf("[%s%d...%s%d] for: ",
				CLICK_EXE_RegDataBase[body->search_range_first_reg_ref.reg_id].txtID,
				body->search_range_first_reg_ref.number,
				CLICK_EXE_RegDataBase[body->search_range_last_reg_ref.reg_id].txtID,
				body->search_range_last_reg_ref.number
				);
						
			switch(body->search_condition)
			{
			case EXE_CODE_INSTRUCTION_SEARCH_EQUAL:
				printf(" == ");
				break;
			case EXE_CODE_INSTRUCTION_SEARCH_NOTEQUAL:
				printf(" != ");
				break;
			case EXE_CODE_INSTRUCTION_SEARCH_MORE:
				printf(" > ");
				break;
			case EXE_CODE_INSTRUCTION_SEARCH_LESS:
				printf(" < ");
				break;
			case EXE_CODE_INSTRUCTION_SEARCH_MOREOREQUAL:
				printf(" >= ");
				break;
			case EXE_CODE_INSTRUCTION_SEARCH_LESSOREQUAL:
				printf(" <= ");
				break;
			}

			switch(body->searching_container)
			{
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_REGISTER:
				printf("%s%d",CLICK_EXE_RegDataBase[body->searching_reg_ref.reg_id].txtID,body->searching_reg_ref.number);
				break;

			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC16bit:
				printf("%d",body->searching_const_int16);
				break;

			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC32bit:
				printf("%d",body->searching_const_int32);
				break;
				
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX8:
				printf("$%02X",(unsigned char)body->searching_const_hex16bit);
				break;
				
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX16:
				printf("%04Xh",body->searching_const_hex16bit);
				break;

			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_FLT32:
				printf("%e",body->searching_const_flt32);
				break;

			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_ASCIISTR:
				printf("%s",body->searching_string);
				break;
			}
			if (body->is_continuous_search)
			{
				printf(" Cont.Search=ON");
			}
			if (body->is_one_shot)
			{
				printf(" !");
			}
			printf("\n");
		}
		break;
		
	case SCR_Op_Math:
		{
			SCR_Operation_Math* body = (SCR_Operation_Math*)object;

			printf("%c %s : [%s%d = %s], %d vars\n",
				(body->is_one_shot)? '!':' ',
				(body->is_hex)? "HEX":"Decimal",
				CLICK_EXE_RegDataBase[body->result_reg_ref.reg_id].txtID,body->result_reg_ref.number,
				body->formula,
				body->n_vars
				);
		}		
		break;
		
	case SCR_Op_NOP:		
		printf("\n");
		break;
		
	default:
		;
	}

	return;
}

int clickplc_scr_Call_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{	
	SCR_Operation_Call* body = (SCR_Operation_Call*)obj;

	unsigned char* stream = stream_after_opcode;

	int n_bytes;
	int offset;

	/////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Call);
	////////////////////////////////////////////////////	

	//19 32
	offset += 2;

	//file_list_index
	clickplc_ckp_int16_dec(&stream[offset],&body->file_list_index);
	offset += 2;

	//08 62
	offset += 2;

	if (is_pascal_string(&stream[offset],body->calling_scr_name,&n_bytes))
	{
		body->calling_scr_name_size = (n_bytes-2)/2;
		offset += n_bytes + 4;
	}
	else
	{
		offset = 0;//error
	}

	return(offset);
}

/*

01 1f 01 01 00 00 00 00 
[Call                ]	op: [    Call]	2723	... 6394	step:   40
00 00 28 00 00 00 01 01 fa 18 00 00 
19 32 03 00 
08 62 
[ConvertAnalogInputs ]
00 00 $$$ 00 00 

*/
int clickplc_scr_Call_enc(void* obj,unsigned char* stream_before_opname)
{	
	SCR_Operation_Call* body = (SCR_Operation_Call*)obj;

	unsigned char* stream = stream_before_opname;

	int n_bytes;
	int offset;
	int toffset;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Call].enc_string,
		SCR_Ops_DataBase[SCR_Op_Call].enc_string_length,
		&stream[0]);

	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Call].code,&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////

	//19 32
	stream[offset + 0] = 0x19;
	stream[offset + 1] = 0x32;
	offset += 2;

	//file_list_index
	clickplc_ckp_int16_enc(body->file_list_index,&stream[offset]);
	offset += 2;

	//08 62
	stream[offset + 0] = 0x08;
	stream[offset + 1] = 0x62;	
	offset += 2;

	n_bytes = enc_pascal_string(body->calling_scr_name,body->calling_scr_name_size,&stream[offset]);
	offset += n_bytes;

	//00 00
	stream[offset + 0] = 0x00;
	stream[offset + 1] = 0x00;
	offset += 2;

	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Call);

	//$$$
	stream[offset + 0] = 0x00;
	stream[offset + 1] = 0x00;
	offset += 2;

	return(offset);
}

int clickplc_scr_OUT_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{	
	SCR_Operation_Out* body = (SCR_Operation_Out*)obj;

	char text[SCR_REG_STRING_MAX_SIZE];
	int n_bytes;
	int offset;

	unsigned char* stream = stream_after_opcode;

	//by default:
	body->is_one_shot	= 0;
	body->is_immediate	= 0;
	body->is_range		= 0;

	/////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Call);
	////////////////////////////////////////////////////	

	if (stream[offset + 0] == 0x66 && stream[offset+1] == 0x60)
	{
		offset += 2;
		//first reg ref
		if (is_pascal_string(&stream[offset],text,&n_bytes))
		{
			if (n_bytes!=clickplc_reg_pascal_dec(&stream[offset],&body->first_bit_reg_ref))
			{
				offset = 0;
				goto __exit_point;
			}			
		}
		offset += n_bytes;
	}

	if (stream[offset + 0] == 0x67 && stream[offset + 1] == 0x60)
	{
		offset += 2;
		//second reg ref
		if (is_pascal_string(&stream[offset],text,&n_bytes))
		{
			if (n_bytes!=clickplc_reg_pascal_dec(&stream[offset],&body->last_bit_reg_ref))
			{
				offset = 0;
				goto __exit_point;
			}			
		}
		body->is_range = 1;
		offset += n_bytes;		
	}

	if (body->is_range==0)
	{
		//one

		if (stream[offset+0]==0x00 && stream[offset+1]==0x00)
		{	//simple
			//00 00 $$$ 00 00

			body->is_one_shot		= 0;
			body->is_immediate	= 0;

			offset += 4;
		}
		else
		{
			if (stream[offset+0]==0xf5 && stream[offset+1]==0x11)
			{	//imm
				//f5 11 18 32 05 20 00 00 $$$ 01 00 01 
				body->is_one_shot	 = 0;
				body->is_immediate = 1;
				offset += 11;
			}
			else
			{
				if (stream[offset+0]==0xf8 && stream[offset+1]==0x11)
				{
					if (stream[offset+2]==0x18 && stream[offset+3]==0x32)
					{	//1s
						//f8 11 18 32 0d 20 00 00 $$$ 01 00 01
						body->is_one_shot	 = 1;
						body->is_immediate = 0;
						offset += 11;
					}
					else
					{	//1s+imm
						//f8 11 f5 11 18 32 0e 20 00 00 $$$ 01 00 03 
						if (stream[offset+2]==0xf5 && stream[offset+3]==0x11)
						{
							body->is_one_shot	 = 1;
							body->is_immediate = 1;
							offset += 13;
						}
						else
						{
							offset = 0;							
						}
					}
				}
				else
				{
					offset = 0;					
				}
			}			
		}
	}
	else
	{
		//range

		if (stream[offset+0]==0x18 && stream[offset+1]==0x32)
		{	//simple
			//18 32 0f 20 00 00 $$$ 00 00 

			body->is_one_shot		= 0;
			body->is_immediate	= 0;
			offset += 8;
		}
		else
		{
			if (stream[offset+0]==0xf5 && stream[offset+1]==0x11)
			{	//imm
				//f5 11 18 32 10 20 00 00 $$$ 01 00 01 
				body->is_one_shot	 = 0;
				body->is_immediate = 1;
				offset += 11;
			}
			else
			{
				if (stream[offset+0]==0xf8 && stream[offset+1]==0x11)
				{
					if (stream[offset+2]==0x18 && stream[offset+3]==0x32)
					{	//1s
						//f8 11 18 32 11 20 00 00 $$$ 01 00 01 
						body->is_one_shot	 = 1;
						body->is_immediate = 0;
						offset += 11;
					}
					else
					{	//1s+imm
						//f8 11 f5 11 18 32 12 20 00 00 $$$ 01 00 03 
						if (stream[offset+2]==0xf5 && stream[offset+3]==0x11)
						{
							body->is_one_shot	 = 1;
							body->is_immediate = 1;
							offset += 13;
						}
						else
						{
							offset = 0;
						}
					}
				}
				else
				{
					offset = 0;
				}
			}
		}
	}
	*pscr_offset = body->info.scr_offset;
__exit_point:	
	return(offset);
}

int clickplc_scr_OUT_enc(void* obj,unsigned char* stream_at_opname)
{
	SCR_Operation_Out* body = (SCR_Operation_Out*)obj;

	int n_bytes;
	int offset;
	int toffset;

	unsigned char* stream = stream_at_opname;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Out].enc_string,
		SCR_Ops_DataBase[SCR_Op_Out].enc_string_length,
		&stream[0]);

	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Out].code,
		&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	//////////////////////////////////////////////////////////////////////////
	offset += SCR_INFO_ENC_SIZE;

	stream[offset + 0] = 0x66;
	stream[offset + 1] = 0x60;
	offset += 2;

	n_bytes = clickplc_reg_pascal_enc(&body->first_bit_reg_ref,&stream[offset]);
	offset += n_bytes;

	if (body->is_range==0)
	{
		//one

		if (body->is_one_shot==0)
		{
			if (body->is_immediate==0)
			{	//simple
				//00 00 $$$ 00 00
				stream[offset+0] = 0x00;
				stream[offset+1] = 0x00;
				stream[offset+2] = 0x00;//<----- $$$
				stream[offset+3] = 0x00;
				offset += 4;
			}
			else
			{	//imm
				//f5 11 18 32 05 20 00 00 $$$ 01 00 01 
				stream[offset+0] = 0xf5;
				stream[offset+1] = 0x11;
				stream[offset+2] = 0x18;
				stream[offset+3] = 0x32;
				stream[offset+4] = 0x05;
				stream[offset+5] = 0x20;
				stream[offset+6] = 0x00;
				stream[offset+7] = 0x00;
				stream[offset+8] = 0x01;//<----- $$$
				stream[offset+9] = 0x00;
				stream[offset+10]= 0x01;
				offset += 11; 
			}
		}
		else
		{
			if (body->is_immediate==0)
			{	//1s
				//f8 11 18 32 0d 20 00 00 $$$ 01 00 01
				stream[offset+0] = 0xf8;
				stream[offset+1] = 0x11;
				stream[offset+2] = 0x18;
				stream[offset+3] = 0x32;
				stream[offset+4] = 0x0d;
				stream[offset+5] = 0x20;
				stream[offset+6] = 0x00;
				stream[offset+7] = 0x00;
				stream[offset+8] = 0x01;//<----- $$$
				stream[offset+9] = 0x00;
				stream[offset+10]= 0x01;
				offset += 11;
			}
			else
			{	//1s+imm
				//f8 11 f5 11 18 32 0e 20 00 00 $$$ 01 00 03 
				stream[offset+0] = 0xf8;
				stream[offset+1] = 0x11;
				stream[offset+2] = 0xf5;
				stream[offset+3] = 0x11;
				stream[offset+4] = 0x18;
				stream[offset+5] = 0x32;
				stream[offset+6] = 0x0e;
				stream[offset+7] = 0x20;
				stream[offset+8] = 0x00;
				stream[offset+9] = 0x00;
				stream[offset+10]= 0x01;//<----- $$$
				stream[offset+11]= 0x00;
				stream[offset+12]= 0x03;
				offset += 13;
			}
		}
	}
	else
	{
		//range

		stream[offset + 0] = 0x67;
		stream[offset + 1] = 0x60;
		offset += 2;

		n_bytes = clickplc_reg_pascal_enc(&body->last_bit_reg_ref,&stream[offset]);

		offset += n_bytes;

		if (body->is_one_shot==0)
		{
			if (body->is_immediate==0)
			{	//simple
				//18 32 0f 20 00 00 $$$ 00 00 
				stream[offset+0] = 0x18;
				stream[offset+1] = 0x32;
				stream[offset+2] = 0x0f;
				stream[offset+3] = 0x20;
				stream[offset+4] = 0x00;
				stream[offset+5] = 0x00;
				stream[offset+6] = 0x00;//<----- $$$
				stream[offset+7] = 0x00;
				offset += 8;
			}
			else
			{	//imm
				//f5 11 18 32 10 20 00 00 $$$ 01 00 01 
				stream[offset+0] = 0xf5;
				stream[offset+1] = 0x11;
				stream[offset+2] = 0x18;
				stream[offset+3] = 0x32;
				stream[offset+4] = 0x10;
				stream[offset+5] = 0x20;
				stream[offset+6] = 0x00;
				stream[offset+7] = 0x00;
				stream[offset+8] = 0x01;//<----- $$$
				stream[offset+9] = 0x00;
				stream[offset+10]= 0x01;
				offset += 11; 
			}
		}
		else
		{
			if (body->is_immediate==0)
			{	//1s
				//f8 11 18 32 11 20 00 00 $$$ 01 00 01 
				stream[offset+0] = 0xf8;
				stream[offset+1] = 0x11;
				stream[offset+2] = 0x18;
				stream[offset+3] = 0x32;
				stream[offset+4] = 0x11;
				stream[offset+5] = 0x20;
				stream[offset+6] = 0x00;
				stream[offset+7] = 0x00;
				stream[offset+8] = 0x01;//<----- $$$
				stream[offset+9] = 0x00;
				stream[offset+10]= 0x01;
				offset += 11;
			}
			else
			{	//1s+imm
				//f8 11 f5 11 18 32 12 20 00 00 $$$ 01 00 03 
				stream[offset+0] = 0xf8;
				stream[offset+1] = 0x11;
				stream[offset+2] = 0xf5;
				stream[offset+3] = 0x11;
				stream[offset+4] = 0x18;
				stream[offset+5] = 0x32;
				stream[offset+6] = 0x12;
				stream[offset+7] = 0x20;
				stream[offset+8] = 0x00;
				stream[offset+9] = 0x00;
				stream[offset+10]= 0x01;//<----- $$$
				stream[offset+11]= 0x00;
				stream[offset+12]= 0x03;
				offset += 13;
			}
		}
	}

	body->info.scr_offset += offset - 2 - (body->is_immediate|body->is_one_shot);
	//printf("body->scr_offset:= %04x\n",body->info.scr_offset);
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Out);

	return(offset);
}

int clickplc_scr_Contact_partially_enc(void* obj,unsigned char* stream_after_opcode);

int clickplc_scr_Contact_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Contact* body = (SCR_Operation_Contact*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];

	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_ContactNO);
	////////////////////////////////////////////////////	

	//set defaults
	body->is_immediate = 0;

	//65 60	
	if (stream[offset+0]!=0x65 || stream[offset+1]!=0x60)
	{
		offset = 0;
		goto __exit_point;
	}

	offset += 2;

	if (is_pascal_string(&stream[offset],text,&n_bytes)==0)
	{
		offset = 0;
		goto __exit_point;
	}

	offset += n_bytes;

	if (clickplc_reg_cstr_dec(text,&body->bit_reg_ref)==0)
	{
		offset = 0;
		goto __exit_point;
	}

	//00 00 or f5 11 00 00
	if (stream[offset+0]==0xf5 && stream[offset+1]==0x11)
	{
		body->is_immediate = 1;
		offset += 2;
	}

	//00 00
	offset += 2;

	if (body->is_immediate)
	{
		//01 00 01
		offset += 3;
	}
	else
	{
		//00 00
		offset += 2;
	}

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_ContactNO_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Contact* body = (SCR_Operation_Contact*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_ContactNO].enc_string,
								SCR_Ops_DataBase[SCR_Op_ContactNO].enc_string_length,
								&stream[0]);

	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_ContactNO].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	
	offset += clickplc_scr_Contact_partially_enc(body,&stream[offset]);

	body->info.scr_offset += offset - (2 + body->is_immediate);	
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_ContactNO);

	return(offset);
}

int clickplc_scr_ContactNC_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Contact* body = (SCR_Operation_Contact*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_ContactNC].enc_string,
								SCR_Ops_DataBase[SCR_Op_ContactNC].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_ContactNC].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////

	offset += clickplc_scr_Contact_partially_enc(body,&stream[offset]);

	body->info.scr_offset += offset - (2 + body->is_immediate);
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_ContactNC);

	return(offset);
}


int clickplc_scr_Contact_partially_enc(void* obj,unsigned char* stream_after_opcode)
{
	SCR_Operation_Contact* body = (SCR_Operation_Contact*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;

	//65 60	
	stream[0] = 0x65;
	stream[1] = 0x60;
	offset = 2;

	n_bytes = clickplc_reg_pascal_enc(&body->bit_reg_ref,&stream[offset]);
	offset += n_bytes;

	if (body->is_immediate)
	{
		//f5 11 00 00
		stream[offset+0] = 0xf5;
		stream[offset+1] = 0x11;
		stream[offset+2] = 0x00;
		stream[offset+3] = 0x00;
		//$$$
		//01 00 01
		stream[offset+4] = 0x01;
		stream[offset+5] = 0x00;
		stream[offset+6] = 0x01;
		offset += 7;
	}
	else
	{
		//00 00 00 00
		stream[offset+0] = 0x00;
		stream[offset+1] = 0x00;
		//$$$
		stream[offset+2] = 0x00;
		stream[offset+3] = 0x00;
		offset += 4;
	}
	
	return(offset);
}

int clickplc_scr_Edge_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Edge* body = (SCR_Operation_Edge*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];

	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Edge);
	////////////////////////////////////////////////////	
	
	//65 60	
	if (stream[offset+0]!=0x65 || stream[offset+1]!=0x60)
	{
		offset = 0;
		goto __exit_point;
	}

	offset += 2;

	if (is_pascal_string(&stream[offset],text,&n_bytes)==0)
	{
		offset = 0;
		goto __exit_point;
	}

	offset += n_bytes;

	if (clickplc_reg_cstr_dec(text,&body->bit_reg_ref)==0)
	{
		offset = 0;
		goto __exit_point;
	}

	//00 00 or f6 21 01 00 00
	if (stream[offset+0]==0xf6 && stream[offset+1]==0x21 && stream[offset+2]==0x01)
	{
		body->is_down = 1;
		offset += 5;
	}
	else
	{
		//00 00
		body->is_down = 0;		
		offset += 2;	
	}

	//$$$
	//00 00
	offset += 2;	

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_Edge_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Edge* body = (SCR_Operation_Edge*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int n_bytes;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Edge].enc_string,
								SCR_Ops_DataBase[SCR_Op_Edge].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(	SCR_Ops_DataBase[SCR_Op_Edge].code,
										&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////

	//65 60	
	stream[offset+0] = 0x65;
	stream[offset+1] = 0x60;
	offset += 2;

	n_bytes = clickplc_reg_pascal_enc(&body->bit_reg_ref,&stream[offset]);
	offset += n_bytes;

	if (body->is_down)
	{
		//f6 21 01 00 00
		stream[offset+0] = 0xf6;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x01;
		stream[offset+3] = 0x00;
		stream[offset+4] = 0x00;
		offset += 5;
	}
	else
	{
		//00 00
		stream[offset+0] = 0x00;
		stream[offset+1] = 0x00;
		offset += 2;
	}

	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Edge);
	
	//$$$
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;
	
	return(offset);
}

int clickplc_scr_Compare_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Compare* body = (SCR_Operation_Compare*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
	int textsize;
	int cmp_const;
	char* donechar_ptr;
	
	body->is_A_const = 0;
	body->is_B_const = 0;

	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Compare);
	////////////////////////////////////////////////////	
	//66 60	
	if (stream[offset+0]!=0x66 || stream[offset+1]!=0x60)
	{
		printf("Not 0x66 && 0x60 !\n");
		offset = 0;
		goto __exit_point;
	}

	offset += 2;

	if (is_pascal_string(&stream[offset],text,&n_bytes)==0)
	{
		printf("A: Not pascal string !\n");
		offset = 0;
		goto __exit_point;
	}

	offset += n_bytes;

	textsize = (n_bytes/2) - 1;

	if (text[textsize-1]!='h')
	{
		//decimal or register
		if (text[0]>='0' && text[0]<='9')
		{
			//decimal			
			cmp_const = strtol(text,&donechar_ptr,10);
			if (donechar_ptr!=text)
			{
				body->is_A_const = 1;
				body->cmp_A_const = cmp_const;
			}
			else
			{
				//unknown: error!
				printf("A: can't strtol(10) !\n");
				offset = 0;
				goto __exit_point;
			}
		}
		else
		{
			if (clickplc_reg_cstr_dec(text,&body->cmp_A_reg_ref)!=0)
			{
				//register
				body->is_A_const = 0;				
			}
			else
			{
				//unknown: error!
				printf("A: can't reg dec (%s) !\n",text);
				
				offset = 0;
				goto __exit_point;
			}
		}
	}
	else
	{		
		//if (true == isxdigit((int)text[i]) ) for all i = 0...textsize-1;
		{
			cmp_const = strtol(text,&donechar_ptr,16);
			if (donechar_ptr!=text)
			{
				body->is_A_const = 1;
				body->cmp_A_const = cmp_const;
			}
			else
			{
				//unknown: error!
				printf("A: can't strtol(16) !\n");				
				offset = 0;
				goto __exit_point;
			}
		}
/*
		else
		{
			//unknown: error!
			printf("A: wrong hex !\n");
			offset = 0;
			goto __exit_point;
		}
*/
	}
	//////////////////////////////////////////////////////////////////////////
	//67 60	
	if (stream[offset+0]!=0x67 || stream[offset+1]!=0x60)
	{
		printf("Not 0x67 && 0x60 !\n");
		offset = 0;
		goto __exit_point;
	}

	offset += 2;

	if (is_pascal_string(&stream[offset],text,&n_bytes)==0)
	{
		printf("B: Not pascal string !\n");
		offset = 0;
		goto __exit_point;
	}

	offset += n_bytes;

	textsize = (n_bytes/2) - 1;

	if (text[textsize-1]!='h')
	{
		//decimal or register
		if (text[0]>='0' && text[0]<='9')
		{
			//decimal			
			cmp_const = strtol(text,&donechar_ptr,10);
			if (donechar_ptr!=text)
			{
				body->is_B_const = 1;
				body->cmp_B_const = cmp_const;
			}
			else
			{
				//unknown: error!
				printf("B: can't strtol(10) !\n");
				offset = 0;
				goto __exit_point;
			}
		}
		else
		{
			if (clickplc_reg_cstr_dec(text,&body->cmp_B_reg_ref)!=0)
			{
				//register
				body->is_B_const = 0;				
			}
			else
			{
				//unknown: error!
				offset = 0;
				goto __exit_point;
			}
		}
	}
	else
	{		
		//if (true == isxdigit((int)text[i]) ) for all i = 0...textsize-1;
		{
			cmp_const = strtol(text,&donechar_ptr,16);
			if (donechar_ptr!=text)
			{
				body->is_B_const = 1;
				body->cmp_B_const = cmp_const;
			}
			else
			{
				//unknown: error!
				printf("B: can't strtol(16) !\n");
				offset = 0;
				goto __exit_point;
			}
		}
/*		else
		{
			//unknown: error!
			printf("B: wrong hex !\n");
			offset = 0;
			goto __exit_point;
		}
*/
	}

	//00 00 or f7 21 01(02,03,04,05) 00 00, 
	if (stream[offset+0]==0x00 && stream[offset+1]==0x00)
	{
		body->cond_type = SCR_CODE_SUBINSTR_COMPARE_EQUAL;

		offset += 2;
	}
	else
	{
		if (stream[offset+0]==0xf7 && stream[offset+1]==0x21)
		{
			switch (stream[offset+2])
			{
			case 0x01:
				body->cond_type = SCR_CODE_SUBINSTR_COMPARE_NOTEQUAL;
				break;
			case 0x02:
				body->cond_type = SCR_CODE_SUBINSTR_COMPARE_MORE;
				break;
			case 0x03:
				body->cond_type = SCR_CODE_SUBINSTR_COMPARE_LESS;
				break;
			case 0x04:
				body->cond_type = SCR_CODE_SUBINSTR_COMPARE_MOREOREQUAL;
				break;
			case 0x05:
				body->cond_type = SCR_CODE_SUBINSTR_COMPARE_LESSOREQUAL;
				break;
			default:
				//unknown: error!
				printf("Unknown enc body->cond_type!= 0x%02x\n",stream[offset+2]);
				offset = 0;
				goto __exit_point;
			}
			offset += 5;
		}
		else
		{
			//unknown: error!
			printf("Not 0xf7 && 0x21 !\n");
			offset = 0;
			goto __exit_point;
		}
	}
		
	////////////////////////////////////////////////////	
	//$$$
	//00 00
	offset += 2;	

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_Compare_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Compare* body = (SCR_Operation_Compare*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
	int textsize;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Compare].enc_string,
		SCR_Ops_DataBase[SCR_Op_Compare].enc_string_length,
		&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Compare].code,
		&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	//66 60	
	stream[offset+0] = 0x66;
	stream[offset+1] = 0x60;
	offset += 2;

	if (body->is_A_const)
	{
		//TODO: hexadecimal if reg is DH or XD,YD
		//...

		//decimal
		textsize = sprintf(text,"%d",body->cmp_A_const);
		n_bytes = enc_pascal_string(text,textsize,&stream[offset]);
		offset += n_bytes;
	}
	else
	{
		n_bytes = clickplc_reg_pascal_enc(&body->cmp_A_reg_ref,&stream[offset]);
		offset += n_bytes;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//67 60	
	stream[offset+0] = 0x67;
	stream[offset+1] = 0x60;
	offset += 2;

	if (body->is_B_const)
	{
		//TODO: hexadecimal if reg is DH or XD,YD
		//...

		//decimal
		textsize = sprintf(text,"%d",body->cmp_B_const);
		n_bytes = enc_pascal_string(text,textsize,&stream[offset]);
		offset += n_bytes;
	}
	else
	{
		n_bytes = clickplc_reg_pascal_enc(&body->cmp_B_reg_ref,&stream[offset]);
		offset += n_bytes;
	}

	//00 00 or f7 21 01(02,03,04,05) 00 00, 

	switch (body->cond_type)
	{
	case SCR_CODE_SUBINSTR_COMPARE_EQUAL:
		stream[offset+0] = 0x00;
		stream[offset+1] = 0x00;
		offset += 2;
		break;

	case SCR_CODE_SUBINSTR_COMPARE_NOTEQUAL:
		stream[offset+0] = 0xf7;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x01;
		stream[offset+3] = 0x00;
		stream[offset+4] = 0x00;
		offset += 5;
		break;
	case SCR_CODE_SUBINSTR_COMPARE_MORE:
		stream[offset+0] = 0xf7;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x02;
		stream[offset+3] = 0x00;
		stream[offset+4] = 0x00;
		offset += 5;
		break;
	case SCR_CODE_SUBINSTR_COMPARE_LESS:
		stream[offset+0] = 0xf7;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x03;
		stream[offset+3] = 0x00;
		stream[offset+4] = 0x00;
		offset += 5;
		break;
	case SCR_CODE_SUBINSTR_COMPARE_MOREOREQUAL:
		stream[offset+0] = 0xf7;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x04;
		stream[offset+3] = 0x00;
		stream[offset+4] = 0x00;
		offset += 5;
		break;
	case SCR_CODE_SUBINSTR_COMPARE_LESSOREQUAL:
		stream[offset+0] = 0xf7;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x05;
		stream[offset+3] = 0x00;
		stream[offset+4] = 0x00;
		offset += 5;
		break;
	default:
		//unknown: error!
		printf("Unknown enc body->cond_type!= 0x%02x\n",body->cond_type);
		offset = 0;
		goto __exit_point;
	}
	
	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Compare);

	//$$$
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;

__exit_point:
	return(offset);
}

int clickplc_scr_EmptyBody_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset,int opid)
{
	SCR_OpBodyAlways* info = (SCR_OpBodyAlways*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;

	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,info,opid);
	////////////////////////////////////////////////////	
	//00 00
	offset += 2;
	////////////////////////////////////////////////////	
	//$$$
	//00 00
	offset += 2;	

	*pscr_offset = info->scr_offset;
	return(offset);
}

int clickplc_scr_EmptyBody_enc(void* obj,unsigned char* stream_before_opname,int opid)
{
	SCR_OpBodyAlways* info = (SCR_OpBodyAlways*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[opid].enc_string,
		SCR_Ops_DataBase[opid].enc_string_length,
		&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[opid].code,
		&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;
	//////////////////////////////////////////////////////////////////////////
	info->scr_offset += offset;
	clickplc_scr_info_enc(info,&stream[toffset],opid);

	//$$$
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;

	return(offset);
}

int clickplc_scr_End_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_End* body = (SCR_Operation_End*)obj;	
	int offset;
	offset = clickplc_scr_EmptyBody_dec(stream_after_opcode,&body->info,pscr_offset,SCR_Op_End);	
	return(offset);
}

int clickplc_scr_End_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_End* body = (SCR_Operation_End*)obj;
	int offset;
	
	offset = clickplc_scr_EmptyBody_enc(&body->info,stream_before_opname,SCR_Op_End);
	
	return(offset);
}

int clickplc_scr_Return_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Return* body = (SCR_Operation_Return*)obj;
	int offset;
	
	offset = clickplc_scr_EmptyBody_dec(stream_after_opcode,&body->info,pscr_offset,SCR_Op_Return);
	
	return(offset);
}

int clickplc_scr_Return_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Return* body = (SCR_Operation_Return*)obj;	
	int offset;
	
	offset = clickplc_scr_EmptyBody_enc(&body->info,stream_before_opname,SCR_Op_Return);

	return(offset);
}

int clickplc_scr_Next_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Next* body = (SCR_Operation_Next*)obj;
	int offset;
	
	offset = clickplc_scr_EmptyBody_dec(stream_after_opcode,&body->info,pscr_offset,SCR_Op_Next);
	
	return(offset);
}

int clickplc_scr_Next_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Next* body = (SCR_Operation_Next*)obj;	
	int offset;
		
	offset = clickplc_scr_EmptyBody_enc(&body->info,stream_before_opname,SCR_Op_Next);
	
	return(offset);
}

int clickplc_scr_For_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_For* body = (SCR_Operation_For*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
				
	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_For);
	////////////////////////////////////////////////////	
	//65 60
	if (stream[offset+0]==0x65 &&stream[offset+1]==0x60)
	{
		offset += 2;

		if (is_pascal_string(&stream[offset],text,&n_bytes))
		{
			offset += n_bytes;
						
			if (clickplc_reg_cstr_dec(text,&body->variable_cycles_ref)!=0)
			{
				body->is_variable = 1;				
			}
			else
			{
				int const_cycles;
				char* donechar_ptr;
				
				const_cycles = strtol(text,&donechar_ptr,10);
				if (donechar_ptr!=text)
				{
					body->is_variable = 0;	
					body->const_cycles = const_cycles;
				}
				else
				{
					//unknown: error!
					printf("For: can't strtol(10) !\n");
					offset = 0;
					goto __exit_point;
				}
			}
						
			if (stream[offset+0]==0xf8 && stream[offset+1]==0x11)
			{
				//f8 11 00 00 
				//$$$
				//01 00 01 
				body->is_one_shot = 1;
				offset += 7;
			}
			else
			{
				//00 00 
				//$$$			
				//00 00 
				body->is_one_shot = 0;
				offset += 4;
			}
		}
		else
		{
			offset = 0;
			goto __exit_point;
		}

	}
	else
	{
		offset = 0;
		goto __exit_point;
	}
	////////////////////////////////////////////////////

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_For_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_For* body = (SCR_Operation_For*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int n_bytes;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_For].enc_string,
								SCR_Ops_DataBase[SCR_Op_For].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_For].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	stream[offset+0] = 0x65;
	stream[offset+1] = 0x60;
	offset += 2;
	if (body->is_variable)
	{
		n_bytes = clickplc_reg_pascal_enc(&body->variable_cycles_ref,&stream[offset]);
		offset += n_bytes;
	}
	else
	{
		char text[SCR_REG_STRING_MAX_SIZE];
		
		n_bytes = sprintf(text,"%u",body->const_cycles);
		n_bytes = enc_pascal_string(text,n_bytes,&stream[offset]);
		offset += n_bytes;
	}
	
	if (body->is_one_shot)
	{
		//f8 11 00 00 
		stream[offset+0] = 0xf8;
		stream[offset+1] = 0x11;
		stream[offset+2] = 0x00;
		stream[offset+3] = 0x00;
				
		//$$$
		//01 00 01 
		stream[offset+4] = 0x01;
		stream[offset+5] = 0x00;
		stream[offset+6] = 0x01;
		offset += 7;
	}
	else
	{
		//00 00 
		stream[offset+0] = 0x00;
		stream[offset+1] = 0x00;
		//$$$
		//00 00 
		stream[offset+2] = 0x00;
		stream[offset+3] = 0x00;
		offset += 4;
	}
	
	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset - (2 + body->is_one_shot);
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_For);
	
	return(offset);
}

int clickplc_scr_SetRst_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_SetRst* body = (SCR_Operation_SetRst*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
		
	//by default:	
	body->is_immediate	= 0;
	body->is_range		= 0;
	
	/////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Set);
	////////////////////////////////////////////////////	
	
	if (stream[offset + 0] == 0x66 && stream[offset+1] == 0x60)
	{
		offset += 2;
		//first reg ref
		n_bytes = clickplc_reg_pascal_dec(&stream[offset],&body->first_bit_reg_ref);
		if (n_bytes <= 0)
		{
			offset = 0;
			goto __exit_point;
		}
		offset += n_bytes;
	}
	else
	{
		offset = 0;
		goto __exit_point;
	}
	
	if (stream[offset + 0] == 0x67 && stream[offset + 1] == 0x60)
	{
		offset += 2;
		//second reg ref
		n_bytes = clickplc_reg_pascal_dec(&stream[offset],&body->last_bit_reg_ref);
		if (n_bytes <= 0)
		{
			offset = 0;
			goto __exit_point;
		}
		body->is_range = 1;
		offset += n_bytes;
	}
	
	if (body->is_range==0)
	{
		//single
		// 66 60 
		// [...]
		// 18 32 03 20 00 00 
		// $$$
		// 00 00 
				
		//single imm(Y)
		// 66 60 
		// [Y...]
		// f5 11 18 32 07 20 00 00 
		// $$$
		// 01 00 01 
		
		if (stream[offset+0]==0x18 && stream[offset+1]==0x32)
		{	//simple
			// 18 32 03 20 00 00 
			// $$$
			// 00 00						
			body->is_immediate	= 0;			
			offset += 8;
		}
		else
		{
			if (stream[offset+0]==0xf5 && stream[offset+1]==0x11)
			{	//imm
				// f5 11 18 32 07 20 00 00 
				// $$$
				// 01 00 01 
								
				body->is_immediate = 1;
				offset += 11;
			}
			else
			{
				//error
				offset = 0;
			}			
		}
	}
	else
	{
		//range 
		// 66 60 
		// [...]
		// 67 60 
		// [...]
		
		// 18 32 15 20 00 00 
		// $$$
		// 00 00 
		
		//range imm(Y)
		// 66 60 
		// [...]
		// 67 60 
		// [...]
		
		// f5 11 18 32 16 20 00 00 
		// $$$
		// 01 00 01 
		
		if (stream[offset+0]==0x18 && stream[offset+1]==0x32)
		{	//range
			// 18 32 15 20 00 00 
			// $$$
			// 00 00 						
			body->is_immediate	= 0;
			offset += 8;
		}
		else
		{
			if (stream[offset+0]==0xf5 && stream[offset+1]==0x11)
			{	//range + imm
				// f5 11 18 32 16 20 00 00 
				// $$$
				// 01 00 01 				
				body->is_immediate = 1;
				offset += 11;
			}
			else
			{
				//error
				offset = 0;
			}
		}
	}
	*pscr_offset = body->info.scr_offset;
__exit_point:	
	return(offset);
}

int clickplc_scr_Set_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_SetRst* body = (SCR_Operation_SetRst*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int n_bytes;

	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Set].enc_string,
								SCR_Ops_DataBase[SCR_Op_Set].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Set].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	stream[offset+0] = 0x66;
	stream[offset+1] = 0x60;
	offset += 2;
	n_bytes = clickplc_reg_pascal_enc(&body->first_bit_reg_ref,&stream[offset]);
	offset += n_bytes;
	
	if (body->is_range==0)
	{
		//single
		// 66 60 
		// [...]
		// 18 32 03 20 00 00 
		// $$$
		// 00 00 
		
		//single imm(Y)
		// 66 60 
		// [Y...]
		// f5 11 18 32 07 20 00 00 
		// $$$
		// 01 00 01 

		if (body->is_immediate == 0)
		{
			//range 
			// 18 32 03 20 00 00 
			stream[offset+0] = 0x18;
			stream[offset+1] = 0x32;
			stream[offset+2] = 0x03;
			stream[offset+3] = 0x20;
			stream[offset+4] = 0x00;
			stream[offset+5] = 0x00;
			// $$$
			// 00 00 
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			offset += 8;
		}
		else
		{
			//range imm(Y)
			// f5 11 18 32 07 20 00 00 
			stream[offset+0] = 0xf5;
			stream[offset+1] = 0x11;
			stream[offset+2] = 0x18;
			stream[offset+3] = 0x32;
			stream[offset+4] = 0x07;
			stream[offset+5] = 0x20;
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			// $$$
			// 01 00 01 
			stream[offset+8] = 0x01;
			stream[offset+9] = 0x00;
			stream[offset+10] = 0x01;
			offset += 11;
		}
	}
	else
	{
		stream[offset+0] = 0x67;
		stream[offset+1] = 0x60;
		offset += 2;
		
		n_bytes = clickplc_reg_pascal_enc(&body->last_bit_reg_ref,&stream[offset]);
		offset += n_bytes;
		
		if (body->is_immediate == 0)
		{
			//range 
			// 18 32 15 20 00 00 
			stream[offset+0] = 0x18;
			stream[offset+1] = 0x32;
			stream[offset+2] = 0x15;
			stream[offset+3] = 0x20;
			stream[offset+4] = 0x00;
			stream[offset+5] = 0x00;
			// $$$
			// 00 00 
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			offset += 8;
		}
		else
		{
			//range imm(Y)
			// f5 11 18 32 16 20 00 00 
			stream[offset+0] = 0xf5;
			stream[offset+1] = 0x11;
			stream[offset+2] = 0x18;
			stream[offset+3] = 0x32;
			stream[offset+4] = 0x16;
			stream[offset+5] = 0x20;
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			// $$$
			// 01 00 01 
			stream[offset+8] = 0x01;
			stream[offset+9] = 0x00;
			stream[offset+10] = 0x01;
			offset += 11;
		}		
	}
	
	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset - (2 + body->is_immediate);
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Set);
	
	return(offset);
}

int clickplc_scr_Rst_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_SetRst* body = (SCR_Operation_SetRst*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int n_bytes;
	
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Rst].enc_string,
								SCR_Ops_DataBase[SCR_Op_Rst].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Rst].code,
		&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	stream[offset+0] = 0x66;
	stream[offset+1] = 0x60;
	offset += 2;
	n_bytes = clickplc_reg_pascal_enc(&body->first_bit_reg_ref,&stream[offset]);
	offset += n_bytes;
	
	if (body->is_range==0)
	{
		//single
		// 66 60 
		// [...]
		// 18 32 04 20 00 00 
		// $$$
		// 00 00 

		//single imm(Y)
		// 66 60 
		// [Y...]
		// f5 11 18 32 08 20 00 00
		// $$$
		// 01 00 01 

		if (body->is_immediate == 0)
		{
			//range 
			// 18 32 03 20 00 00 
			stream[offset+0] = 0x18;
			stream[offset+1] = 0x32;
			stream[offset+2] = 0x04;
			stream[offset+3] = 0x20;
			stream[offset+4] = 0x00;
			stream[offset+5] = 0x00;
			// $$$
			// 00 00 
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			offset += 8;
		}
		else
		{
			//range imm(Y)
			// f5 11 18 32 07 20 00 00 
			stream[offset+0] = 0xf5;
			stream[offset+1] = 0x11;
			stream[offset+2] = 0x18;
			stream[offset+3] = 0x32;
			stream[offset+4] = 0x08;
			stream[offset+5] = 0x20;
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			// $$$
			// 01 00 01 
			stream[offset+8] = 0x01;
			stream[offset+9] = 0x00;
			stream[offset+10] = 0x01;
			offset += 11;
		}
	}
	else
	{
		stream[offset+0] = 0x67;
		stream[offset+1] = 0x60;
		offset += 2;

		n_bytes = clickplc_reg_pascal_enc(&body->last_bit_reg_ref,&stream[offset]);
		offset += n_bytes;

		if (body->is_immediate == 0)
		{
			//range 
			// 18 32 1b 20 00 00
			stream[offset+0] = 0x18;
			stream[offset+1] = 0x32;
			stream[offset+2] = 0x1b;
			stream[offset+3] = 0x20;
			stream[offset+4] = 0x00;
			stream[offset+5] = 0x00;
			// $$$
			// 00 00 
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			offset += 8;
		}
		else
		{
			//range imm(Y)
			// f5 11 18 32 1c 20 00 00
			stream[offset+0] = 0xf5;
			stream[offset+1] = 0x11;
			stream[offset+2] = 0x18;
			stream[offset+3] = 0x32;
			stream[offset+4] = 0x1c;
			stream[offset+5] = 0x20;
			stream[offset+6] = 0x00;
			stream[offset+7] = 0x00;
			// $$$
			// 01 00 01 
			stream[offset+8] = 0x01;
			stream[offset+9] = 0x00;
			stream[offset+10] = 0x01;
			offset += 11;
		}		
	}

	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset - (2 + body->is_immediate);
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Rst);

	return(offset);
}

//returns: size of encoded data (bytes):
int clickplc_ckp_encode_parameter_as_pascal_str(unsigned char*	stream,				//IN/OUT
												int				prm_datatype,		//IN
												void*			prm0,				//IN
												void*			prm1,				//IN
												unsigned char*	orig_text,			//IN
												unsigned short	orig_text_size,		//IN
												unsigned short*	porig_text_size		//OUT
												)
{
	int encsize = 0;
	
	switch(prm_datatype)
	{	
	case CODE_DATATYPE_REG:
		encsize = clickplc_reg_pascal_enc((EXE_Register*)prm0,stream);
		break;

	case CODE_DATATYPE_REGBYPTR:
		encsize = clickplc_regbyptr_pascal_enc((EXE_Register*)prm0,(EXE_Register*)prm1,stream);
		break;

	case CODE_DATATYPE_CONSTSTR:
		encsize = enc_pascal_string((char*)prm0,*((unsigned short*)prm1),stream);
		break;

	case CODE_DATATYPE_UNKNOWN://error -> return (encsize = 0);
		break;

	default:
		if (orig_text_size > 0)
		{			
			encsize = enc_pascal_string((char*)orig_text,orig_text_size,stream);
		}
		else
		{
			char ascii_string[CODE_COPY_ASCII_MAX+1];
			int ascii_string_size;

			switch (prm_datatype)
			{
			case CODE_DATATYPE_CONSTDEC16b:
				ascii_string_size = sprintf(ascii_string,"%d",(int)((short)*((short*)prm0)));
				break;

			case CODE_DATATYPE_CONSTDEC32b:
				ascii_string_size = sprintf(ascii_string,"%d",(int)((int)*((int*)prm0)));
				break;

			case CODE_DATATYPE_CONSTHEX8:
				ascii_string_size = sprintf(ascii_string,"$%02X",(unsigned int)((unsigned short)*((unsigned short*)prm0)));
				break;

			case CODE_DATATYPE_CONSTHEX16:
				ascii_string_size = sprintf(ascii_string,"%Xh",(unsigned int)((unsigned short)*((unsigned short*)prm0)));
				break;

			case CODE_DATATYPE_CONSTHEX32:
				ascii_string_size = sprintf(ascii_string,"%Xh",(unsigned int)((unsigned int)*((unsigned int*)prm0)));
				break;

			case CODE_DATATYPE_CONSTFLT32:
				//ascii_string_size = sprintf(ascii_string,"%f",(float)((float)*((float*)prm0)));
				ascii_string_size = sprintf(ascii_string,"%E",(float)((float)*((float*)prm0)));
				break;
			}

			if (ascii_string_size > 0)
			{
				//save the planet!
				if (orig_text)
				{
					memcpy(orig_text,ascii_string,ascii_string_size);
					orig_text[ascii_string_size] = 0x00;
					if(porig_text_size) *porig_text_size = (unsigned short)ascii_string_size;
				}

				//encode as pascal string
				encsize = enc_pascal_string(ascii_string,ascii_string_size,stream);
			}
		}
	}

	return(encsize);
}

//returns: type of data; writes parsed&decoded data (according to type)
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
	int				const_str_lenlim,	//OUT
	int*			pconst_strlen		//OUT
	)
{
	int data_type = CODE_DATATYPE_UNKNOWN;
	unsigned char* str = (unsigned char*)cstr_string;
	
	//try to define type of argument
	//reg, reg[pointer], int16, int32, hex16, flt32, text

	switch(str[0])
	{
	case '"'://text
		if ( (str_len-2) <= const_str_lenlim)
		{
			data_type = CODE_DATATYPE_CONSTSTR;
			memcpy(const_str,str,str_len);
			*pconst_strlen = str_len;
		}
		else
		{
			printf("Const string too big (%d > %d) !!!\n",str_len-2,const_str_lenlim);
		}
		break;

	case '$'://hex8
		if(str_len > 1 && str_len < 4)//2,3 - ok
		{
			int temp;
			char* donechar_ptr = 0;			

			//set ending null to be sure (before string conversion)
			if(str[str_len] != 0) str[str_len] = 0;

			temp = strtol((char*)str+1,&donechar_ptr,16);
			if (donechar_ptr!=(char*)str+1)
			{
				data_type = CODE_DATATYPE_CONSTHEX8;
				*pconst_hex8bit = (unsigned char)temp;
			}
			else
			{
				//unknown: error!
				printf("clickplc_ckp_str_parse: can't strtol(16)-> hex8 !\n");
			}			
		}
		else
		{
			printf("clickplc_ckp_str_parse -> $-hex8 string size: %d-byte(s) ! must be 2 or 3 bytes.\n",str_len);
		}
		break;

	default://hex16, hex32, reg, reg[pointer], int16, int32, flt32

		if (str[str_len-1] == 'h')
		{//hex16 or hex32
			int temp;
			char* donechar_ptr = 0;

			//set ending null to be sure (before string conversion)
			if(str[str_len] != 0) str[str_len] = 0;

			temp = strtol((char*)str,&donechar_ptr,16);
			if (donechar_ptr!=(char*)str)
			{				
				if (temp >= 0)
				{
					if (temp > 32767)
					{
						*pconst_hex32bit = (unsigned int)temp;
						data_type = CODE_DATATYPE_CONSTHEX32;
					}
					else
					{
						*pconst_hex16bit = (unsigned short)temp;
						data_type = CODE_DATATYPE_CONSTHEX16;
					}
				}
				else
				{
					if (temp < -32768)
					{
						*pconst_hex32bit = (unsigned int)temp;
						data_type = CODE_DATATYPE_CONSTHEX32;
					}
					else
					{
						*pconst_hex16bit = (unsigned short)temp;
						data_type = CODE_DATATYPE_CONSTHEX16;
					}
				}				
			}
			else
			{
				//unknown: error!
				printf("clickplc_ckp_str_parse: can't strtol(16) !\n");
			}
		}
		else
		{//flt32,int16,int32 or reg,reg[ptr]
			if( (str[0]>='0' && str[0]<='9') || str[0]=='.' || str[0]=='-' || str[0]=='+' )
			{

				int i;
				int is_flt = 0;

				for (i = 0; i < str_len; i++)
				{
					if (str[i]=='.' || str[i]=='e' || str[i]=='E')
					{
						is_flt = 1;
						break;
					}
				}

				//set ending null to be sure (before string conversion)
				if(str[str_len] != 0) str[str_len] = 0;

				if (is_flt)
				{//flt32					
					*pconst_flt32 = (float)atof((const char*)str);
					data_type = CODE_DATATYPE_CONSTFLT32;
				}
				else
				{//int16, int32
					long temp;
					char* donechar_ptr = 0;

					temp = strtol((char*)str,&donechar_ptr,10);
					if (donechar_ptr!=(char*)str)
					{				
						if (temp >= 0)
						{
							if (temp > 32767)
							{
								*pconst_int32 = temp;
								data_type = CODE_DATATYPE_CONSTDEC32b;
							}
							else
							{
								*pconst_int16 = (short)temp;
								data_type = CODE_DATATYPE_CONSTDEC16b;
							}
						}
						else
						{
							if (temp < -32768)
							{
								*pconst_int32 = temp;
								data_type = CODE_DATATYPE_CONSTDEC32b;
							}
							else
							{
								*pconst_int16 = (short)temp;
								data_type = CODE_DATATYPE_CONSTDEC16b;
							}
						}
					}
					else
					{
						//unknown: error!
						printf("clickplc_ckp_str_parse: can't strtol(10) !\n");
					}
				}				
			}
			else
			{//reg, reg[pointer]							
				int scope_left;
				int scope_right;

				scope_left = find_code_offset(str,str_len,0x5b);//[
				scope_right= find_code_offset(str,str_len,0x5d);//]

				if (scope_left != CODE_NOT_EXIST)
				{//reg[pointer]
					if (scope_right != CODE_NOT_EXIST)
					{
						if (clickplc_regbyptr_cstr_dec((char*)str,preg,ptrreg))
						{
							data_type = CODE_DATATYPE_REGBYPTR;
						}
						else
						{
							printf("Can't decode reg[ptrreg] from [%s] !!!\n",str);
						}
					}
					else
					{
						printf("No closing ] in copy argument [%s] !!!\n",str);
					}
				}
				else
				{//reg
					if (clickplc_reg_cstr_dec((char*)str,preg))
					{
						data_type = CODE_DATATYPE_REG;
					}
					else
					{
						printf("Unknown copy argument: [%s] !!!\n",str);
					}
				}
			}
		}
	}

	return(data_type);
}

#define SCR_COPY_MARKER_SOURCE_1 0x6074
#define SCR_COPY_MARKER_SOURCE_2 0x6075
#define SCR_COPY_MARKER_DESTIN_1 0x6076
#define SCR_COPY_MARKER_DESTIN_2 0x6077

int clickplc_scr_Copy_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Copy* body = (SCR_Operation_Copy*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
	unsigned short marker;
	int goon;

	//defaults
	body->is_src_addr_by_pointer = 0;
	body->is_dst_addr_by_pointer = 0;
	body->is_one_shot = 0;
	body->ascii_string_size = 0;
	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Copy);
	////////////////////////////////////////////////////

	//parse and decode {param-marker + pascal-string}:
	goon = 1;
	do
	{
		clickplc_ckp_int16_dec(&stream[offset],&marker);

		if (marker >= SCR_COPY_MARKER_SOURCE_1 && marker<= SCR_COPY_MARKER_DESTIN_2)
		{
			offset += 2;
			
			if (is_pascal_string(&stream[offset],text,&n_bytes))
			{				
				int textsize = (n_bytes-2)/2;
				int data_type;
				
				EXE_Register reg;
				EXE_Register ptrreg;
				short const_int16;
				unsigned char const_hex8bit;
				unsigned short const_hex16bit;
				unsigned int const_hex32bit;
				int const_int32;
				float const_flt32;
				char ascii_str[CODE_COPY_ASCII_MAX+1];
				int ascii_strlen;
				
				offset += n_bytes;
				
				//try to define type of argument
				//reg, reg[pointer], int16, int32, hex16, text
				data_type = clickplc_ckp_str_parse(	text,textsize,
													&reg,&ptrreg,
													&const_hex8bit,&const_hex16bit,&const_hex32bit,
													&const_int16,&const_int32,&const_flt32,
													ascii_str,CODE_COPY_ASCII_MAX,&ascii_strlen);

				switch(data_type)
				{				
				case CODE_DATATYPE_REG:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_REG;
						memcpy(&body->src_reg_start_ref,&reg,sizeof(EXE_Register));
						break;

					case SCR_COPY_MARKER_SOURCE_2:
						body->src_type = CODE_DATATYPE_REG;
						memcpy(&body->src_reg_end_ref,&reg,sizeof(EXE_Register));
						break;

					case SCR_COPY_MARKER_DESTIN_1:
						memcpy(&body->dst_reg_start_ref,&reg,sizeof(EXE_Register));
						break;

					case SCR_COPY_MARKER_DESTIN_2:
						memcpy(&body->dst_reg_end_ref,&reg,sizeof(EXE_Register));
						break;

					default:
						printf("Unknown copy argument marker: %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_REGBYPTR:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_REGBYPTR;
						body->is_src_addr_by_pointer = 1;
						memcpy(&body->src_reg_start_ref,&reg,sizeof(EXE_Register));
						memcpy(&body->src_pointer_reg_ref,&ptrreg,sizeof(EXE_Register));
						break;

					case SCR_COPY_MARKER_DESTIN_1:											
						body->is_dst_addr_by_pointer = 1;
						memcpy(&body->dst_reg_start_ref,&reg,sizeof(EXE_Register));
						memcpy(&body->dst_pointer_reg_ref,&ptrreg,sizeof(EXE_Register));
						break;

					case SCR_COPY_MARKER_SOURCE_2:
					case SCR_COPY_MARKER_DESTIN_2:

						printf("Unsupported reg[ptrreg] at marker %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;

					default:
						printf("Unknown copy argument marker: %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTDEC16b:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_CONSTDEC16b;
						body->src_const_int16 = const_int16;

						body->ascii_string_size = textsize;
						memcpy(body->ascii_string,text,textsize);
						body->ascii_string[textsize] = 0x00;						
						break;

					case SCR_COPY_MARKER_SOURCE_2:
					case SCR_COPY_MARKER_DESTIN_1:
					case SCR_COPY_MARKER_DESTIN_2:
					default:
						printf("Unsupported [int16] at marker %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTDEC32b:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_CONSTDEC32b;
						body->src_const_int32 = const_int32;

						body->ascii_string_size = textsize;
						memcpy(body->ascii_string,text,textsize);
						body->ascii_string[textsize] = 0x00;						
						break;

					case SCR_COPY_MARKER_SOURCE_2:
					case SCR_COPY_MARKER_DESTIN_1:
					case SCR_COPY_MARKER_DESTIN_2:
					default:
						printf("Unsupported [int32] at marker %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTHEX8:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_CONSTHEX8;

						//body->src_const_hex8bit = (unsigned short)const_hex8bit;
						body->src_const_hex16bit = (unsigned short)const_hex8bit;

						body->ascii_string_size = textsize;
						memcpy(body->ascii_string,text,textsize);
						body->ascii_string[textsize] = 0x00;
						break;

					case SCR_COPY_MARKER_SOURCE_2:
					case SCR_COPY_MARKER_DESTIN_1:
					case SCR_COPY_MARKER_DESTIN_2:
					default:
						printf("Unsupported [hex8bit] at marker %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTHEX16:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_CONSTHEX16;
						body->src_const_hex16bit = const_hex16bit;

						body->ascii_string_size = textsize;
						memcpy(body->ascii_string,text,textsize);
						body->ascii_string[textsize] = 0x00;						
						break;

					case SCR_COPY_MARKER_SOURCE_2:
					case SCR_COPY_MARKER_DESTIN_1:
					case SCR_COPY_MARKER_DESTIN_2:
					default:
						printf("Unsupported [hex16bit] at marker %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTHEX32:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_CONSTHEX32;
						body->src_const_hex32bit = const_hex32bit;

						body->ascii_string_size = textsize;
						memcpy(body->ascii_string,text,textsize);
						body->ascii_string[textsize] = 0x00;						
						break;

					case SCR_COPY_MARKER_SOURCE_2:
					case SCR_COPY_MARKER_DESTIN_1:
					case SCR_COPY_MARKER_DESTIN_2:
					default:
						printf("Unsupported [hex32bit] at marker %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTFLT32:

					switch(marker)
					{
					case SCR_COPY_MARKER_SOURCE_1:
						body->src_type = CODE_DATATYPE_CONSTFLT32;
						body->src_const_flt32 = const_flt32;

						body->ascii_string_size = textsize;
						memcpy(body->ascii_string,text,textsize);
						body->ascii_string[textsize] = 0x00;
						break;

					case SCR_COPY_MARKER_SOURCE_2:
					case SCR_COPY_MARKER_DESTIN_1:
					case SCR_COPY_MARKER_DESTIN_2:
					default:
						printf("Unsupported [flt32bit] at marker %04x !!!\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTSTR:

					if (marker == SCR_COPY_MARKER_SOURCE_1)
					{
						body->src_type = CODE_DATATYPE_CONSTSTR;
						body->ascii_string_size = ascii_strlen;
						memcpy(body->ascii_string,ascii_str,ascii_strlen);
						body->ascii_string[ascii_strlen] = 0x00;
					}
					else
					{
						printf("String at wrong place for Copy !!!\n");
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_UNKNOWN:
				default:
					printf("Can't define data type (encoded in string).\n");
					offset = 0;
					goto __exit_point;
				}
			}
			else
			{
				offset = 0;
				goto __exit_point;
			}
		}
		else
		{
			goon = 0;//exit from loop			
		}

	}while(goon);

	//defaults
	body->is_one_shot = 0;
	body->int2txt_not_suppress_zero_flt2txt_exponent_form = 0;

	if (marker==0x0000)
	{
		offset += 2;
		body->type = CODE_COPY_SINGLE;
	}
	else
	{
		//parse and decode options (int2txt_not_suppress_zero_flt2txt_exponent_form) 0 - suppress zero, 1 - not suppress zero; 0 - real number, 1 - exponent form
		//and define finally type of Copy (Single,Block,Fill,Pack,Unpack):
		goon = 1;
		do 
		{						
			switch(marker)
			{
			case 0x0000:
				offset += 2;
				goon = 0;//normal exit from loop
				break;

			case 0x2206:
				offset += 2;
				body->type = CODE_COPY_SINGLE;
				body->int2txt_not_suppress_zero_flt2txt_exponent_form = 0;

				switch(stream[offset])
				{
				case 0x01:	//Single + Not suppress: TXT3 <- 327Fh, +1s
							//Single + Exp.: TXT5 <- DF7,	+1s
					//go on...
					break;
				default:
					printf("Unknown byte [%02x] after prm-marker [%04x] !!!\n",stream[offset],marker);
					offset = 0;
					goto __exit_point;					
				}
				offset++;
				break;

			case 0x2227:
				offset += 2;
				body->type = CODE_COPY_SINGLE;
				
				switch (stream[offset])
				{
				case 0x01://Single + Suppress: TXT3 <- 321Fh, Single + Suppress: TXT4 <- 123456
					body->int2txt_not_suppress_zero_flt2txt_exponent_form = 0;
					break;

				case 0x02://Single + Not suppress: TXT3 <- 327Fh
					body->int2txt_not_suppress_zero_flt2txt_exponent_form = 1;
					break;

				case 0x03://Single + Real: TXT5 <- DF7, TXT7 <- 1.23456
					body->int2txt_not_suppress_zero_flt2txt_exponent_form = 0;
					break;

				case 0x04://Single + Exp.: TXT5 <- DF7, TXT7 <- 1.23456
					body->int2txt_not_suppress_zero_flt2txt_exponent_form = 1;
					break;
				default:
					printf("Unknown byte [%02x] after prm-marker [%04x] !!!\n",stream[offset],marker);
					offset = 0;
					goto __exit_point;
				}
				offset++;

				break;

			case 0x2223:
				offset += 2;
				switch (stream[offset])
				{
				case 0x01:
					body->type = CODE_COPY_BLOCK;
					break;
				case 0x02:
					body->type = CODE_COPY_FILL;
					break;
				case 0x03:
					body->type = CODE_COPY_PACK;
					break;
				case 0x04:
					body->type = CODE_COPY_UNPACK;
					break;
				default:
					printf("Unknown byte [%02x] after prm-marker [%04x] !!!\n",stream[offset],marker);
					offset = 0;
					goto __exit_point;
				}
				offset++;
				break;

			case 0x11f8:
				offset += 2;
				body->is_one_shot = 1;
				//any: go on...
				break;

			default:
				printf("Unknown prm-marker [%04x] !!!\n",marker);
				offset = 0;
				goto __exit_point;
			}
			
			if(goon) clickplc_ckp_int16_dec(&stream[offset],&marker);

		} while (goon);
	}
	
	////////////////////////////////////////////////////	
	//$$$
	if (body->is_one_shot == 0)
	{
		//00 00
		if (	stream[offset+0]==0x00
			&&	stream[offset+1]==0x00)
		{
			offset += 2;
			*pscr_offset = body->info.scr_offset;
		}
		else
		{
			printf("Unknown bytes [%02x,%02x] at $$$ !!!\n",stream[offset+0],stream[offset+1]);
			//error
			offset = 0;
		}
	}
	else
	{
		if (	stream[offset+0]==0x01
			&&	stream[offset+1]==0x00
			&&	stream[offset+2]==0x01
			)
		{
			offset += 3;
			*pscr_offset = body->info.scr_offset;
		}
		else
		{
			printf("Unknown bytes [%02x,%02x,%02x] at $$$ !!!\n",stream[offset+0],stream[offset+1],stream[offset+2]);
			//error
			offset = 0;
		}
	}

__exit_point:
	return(offset);
}

#define COPY_SINGLE_SIMPLE	0
#define COPY_SINGLE_INT2TXT 1
#define COPY_SINGLE_FLT2TXT 2

int clickplc_ckp_scr_single_options_enc(unsigned char* stream,int method,int not_suppress_or_exponent,int is_one_shot)
{
	int offset = 0;

	switch(method)
	{
	case COPY_SINGLE_SIMPLE:
		if (is_one_shot)
		{
			stream[offset + 0] = 0xf8;
			stream[offset + 1] = 0x11;
			offset += 2;
		}
		stream[offset + 0] = 0x00;
		stream[offset + 1] = 0x00;
		offset += 2;
		break;

	case COPY_SINGLE_INT2TXT:
	case COPY_SINGLE_FLT2TXT:

		method -= COPY_SINGLE_INT2TXT;
		method *= 2;
		method++;

		if (not_suppress_or_exponent==0)
		{
			// 							Single + Suppress: TXT7 <- DD4, TXT4 <- 123456, TXT3 <- 321Fh
			// 								27 22 01 00 00					+	00 00		
			// 								f8 11 27 22 01 00 00			+	01 00 01

			// 							Single + Real: TXT5 <- DF7, TXT7 <- 1.23456
			//								27 22 03 00 00					+	00 00
			// 								f8 11 27 22 03 00 00			+	01 00 01

			if (is_one_shot)
			{
				stream[offset + 0] = 0xf8;
				stream[offset + 1] = 0x11;
				offset += 2;
			}
			stream[offset + 0] = 0x27;
			stream[offset + 1] = 0x22;
			stream[offset + 2] = (unsigned char)method;
			stream[offset + 3] = 0x00;
			stream[offset + 4] = 0x00;
			offset += 5;
		}
		else
		{			
			method++;

			// 							Single + Not suppress: TXT7 <- DD4, TXT3 <- 327Fh
			// 								06 22 01 27 22 02 00 00			+	00 00
			// 								06 22 01 f8 11 27 22 02 00 00	+	01 00 01

			// 							Single + Exp.: TXT5 <- DF7, TXT7 <- 1.23456
			// 								06 22 01 27 22 04 00 00			+	00 00
			// 								06 22 01 f8 11 27 22 04 00 00	+	01 00 01
			
			stream[offset + 0] = 0x06;
			stream[offset + 1] = 0x22;
			stream[offset + 2] = 0x01;
			offset += 3;
			if (is_one_shot)
			{
				stream[offset + 0] = 0xf8;
				stream[offset + 1] = 0x11;
				offset += 2;
			}
			stream[offset + 0] = 0x27;
			stream[offset + 1] = 0x22;
			stream[offset + 2] = (unsigned char)method;
			stream[offset + 3] = 0x00;
			stream[offset + 4] = 0x00;
			offset += 5;
		}
		break;
	}
	
	return(offset);
}

int clickplc_scr_Copy_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Copy* body = (SCR_Operation_Copy*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;

	int marker;
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Copy].enc_string,
								SCR_Ops_DataBase[SCR_Op_Copy].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Copy].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	
	//{param-marker + pascal-string}:

	marker = SCR_COPY_MARKER_SOURCE_1;
	do 
	{
		void* prm_ptr0 = 0;
		void* prm_ptr1 = 0;
		unsigned char* orig_text = 0;
		unsigned short orig_text_size = 0;
		unsigned short* porig_text_size = 0;
		int data_type = CODE_DATATYPE_UNKNOWN;

		switch(marker)
		{
		case SCR_COPY_MARKER_SOURCE_1:
			/*
				single:		reg, reg[pointer], int16, int32, hex16, flt32, text
				pack:		reg
				unpack:		reg (DH only)
				fill:		reg, int16, int32, hex16, flt32, text
				block:		reg
			*/
			switch(body->src_type)
			{		
			case CODE_DATATYPE_REG:
				prm_ptr0 = (void*)&body->src_reg_start_ref;
				break;

			case CODE_DATATYPE_REGBYPTR:
				prm_ptr0 = (void*)&body->src_reg_start_ref;
				prm_ptr1 = (void*)&body->src_pointer_reg_ref;
				break;

			case CODE_DATATYPE_CONSTDEC16b:
				prm_ptr0 = (void*)&body->src_const_int16;
				if (body->ascii_string_size > 0)
				{					
					orig_text_size = body->ascii_string_size;
				}
				orig_text = body->ascii_string;
				porig_text_size = &body->ascii_string_size;
				break;

			case CODE_DATATYPE_CONSTDEC32b:
				prm_ptr0 = (void*)&body->src_const_int32;
				if (body->ascii_string_size > 0)
				{					
					orig_text_size = body->ascii_string_size;
				}
				orig_text = body->ascii_string;
				porig_text_size = &body->ascii_string_size;
				break;

			case CODE_DATATYPE_CONSTHEX8:
				prm_ptr0 = (void*)&body->src_const_hex16bit;
				if (body->ascii_string_size > 0)
				{					
					orig_text_size = body->ascii_string_size;
				}
				orig_text = body->ascii_string;
				porig_text_size = &body->ascii_string_size;
				break;

			case CODE_DATATYPE_CONSTHEX16:
				prm_ptr0 = (void*)&body->src_const_hex16bit;
				if (body->ascii_string_size > 0)
				{					
					orig_text_size = body->ascii_string_size;
				}
				orig_text = body->ascii_string;
				porig_text_size = &body->ascii_string_size;
				break;

			case CODE_DATATYPE_CONSTHEX32:

				prm_ptr0 = (void*)&body->src_const_hex32bit;
				if (body->ascii_string_size > 0)
				{					
					orig_text_size = body->ascii_string_size;
				}
				orig_text = body->ascii_string;
				porig_text_size = &body->ascii_string_size;

				break;

			case CODE_DATATYPE_CONSTFLT32:
				prm_ptr0 = (void*)&body->src_const_flt32;
				if (body->ascii_string_size > 0)
				{					
					orig_text_size = body->ascii_string_size;
				}
				orig_text = body->ascii_string;
				porig_text_size = &body->ascii_string_size;
				break;

			case CODE_DATATYPE_CONSTSTR:
				prm_ptr0 = (void*)body->ascii_string;
				prm_ptr1 = (void*)&body->ascii_string_size;
				break;

			case CODE_DATATYPE_UNKNOWN:
			default:
				//wrong data type!!!
				goto __exit_point;
			}
			data_type = body->src_type;
			break;

		case SCR_COPY_MARKER_SOURCE_2:
			/*
				single:		---
				pack:		reg
				unpack:		---
				fill:		---
				block:		reg
			*/
			switch(body->type)
			{
			case CODE_COPY_BLOCK:
			case CODE_COPY_PACK:
				prm_ptr0 = (void*)&body->src_reg_end_ref;
				data_type = CODE_DATATYPE_REG;
				break;
			}			
			break;

		case SCR_COPY_MARKER_DESTIN_1:
			/*
				single:		reg, reg[pointer]
				pack:		reg
				unpack:		reg (Y,C only)
				fill:		reg
				block:		reg
			*/
			if (body->is_dst_addr_by_pointer==0)
			{
				prm_ptr0 = (void*)&body->dst_reg_start_ref;
				data_type = CODE_DATATYPE_REG;
			}
			else
			{
				prm_ptr0 = (void*)&body->dst_reg_start_ref;
				prm_ptr1 = (void*)&body->dst_pointer_reg_ref;
				data_type = CODE_DATATYPE_REGBYPTR;
			}
			break;

		case SCR_COPY_MARKER_DESTIN_2:
			/*
				single:		reg (only if src == const str)
				pack:		--- 
				unpack:		reg (Y,C only)
				fill:		reg
				block:		reg
			*/
			switch(body->type)
			{
			case CODE_COPY_SINGLE:
				if (body->src_type == CODE_DATATYPE_CONSTSTR)
				{
					if (body->is_dst_addr_by_pointer==0)
					{
						prm_ptr0 = (void*)&body->dst_reg_end_ref;
						data_type = CODE_DATATYPE_REG;
					}
				}
				break;
			case CODE_COPY_BLOCK:
			case CODE_COPY_FILL:
			case CODE_COPY_UNPACK:
				if (body->is_dst_addr_by_pointer==0)
				{
					prm_ptr0 = (void*)&body->dst_reg_end_ref;
					data_type = CODE_DATATYPE_REG;
				}
				break;

			case CODE_COPY_PACK:
				break;
			}
			break;
		}
		//encode if any
		if (prm_ptr0!=0)
		{
			//marker
			offset += clickplc_ckp_int16_enc(marker,&stream[offset]);
			//parameter
			offset += clickplc_ckp_encode_parameter_as_pascal_str(&stream[offset],data_type,prm_ptr0,prm_ptr1,orig_text,orig_text_size,porig_text_size);
		}
		//goon!
		marker++;
	} while (marker <= SCR_COPY_MARKER_DESTIN_2);
	

	//options (int2txt_not_suppress_zero_flt2txt_exponent_form) 0 - suppress zero, 1 - not suppress zero; 0 - real number, 1 - exponent form
	switch(body->type)
	{
	case CODE_COPY_SINGLE:
		{
			int method;

			switch (body->dst_reg_start_ref.reg_id)
			{
			case CLICK_EXE_Register_TXT://converting something -> text

				switch (body->src_type)
				{			
				case CODE_DATATYPE_REG:
				case CODE_DATATYPE_REGBYPTR:

					switch (body->src_reg_start_ref.reg_id)
					{
					case CLICK_EXE_Register_TXT://text -> text
						method = COPY_SINGLE_SIMPLE;
						break;

						//float -> text
					case CLICK_EXE_Register_DF:
						method = COPY_SINGLE_FLT2TXT;
						break;

						//integer -> text
					default:						
						method = COPY_SINGLE_INT2TXT;
					}				
					break;

				case CODE_DATATYPE_CONSTDEC16b:
				case CODE_DATATYPE_CONSTDEC32b:
				case CODE_DATATYPE_CONSTHEX8:
				case CODE_DATATYPE_CONSTHEX16:
				case CODE_DATATYPE_CONSTHEX32:
					method = COPY_SINGLE_INT2TXT;
					break;

				case CODE_DATATYPE_CONSTFLT32:
					method = COPY_SINGLE_FLT2TXT;
					break;

				case CODE_DATATYPE_CONSTSTR://text -> text
					method = COPY_SINGLE_SIMPLE;
					break;

				case CODE_DATATYPE_UNKNOWN:
				default:
					offset = 0;
					goto __exit_point;
				}
				break;

			default: //something -> non-text registers

				switch (body->src_type)
				{			
				case CODE_DATATYPE_REG:
				case CODE_DATATYPE_REGBYPTR:
					method = COPY_SINGLE_SIMPLE;
					break;

				case CODE_DATATYPE_CONSTDEC16b:
				case CODE_DATATYPE_CONSTDEC32b:
				case CODE_DATATYPE_CONSTHEX8:
				case CODE_DATATYPE_CONSTHEX16:
				case CODE_DATATYPE_CONSTHEX32:
					method = COPY_SINGLE_SIMPLE;
					break;

				case CODE_DATATYPE_CONSTFLT32:
					method = COPY_SINGLE_SIMPLE;
					break;

				case CODE_DATATYPE_CONSTSTR:
					method = COPY_SINGLE_SIMPLE;
					break;

				case CODE_DATATYPE_UNKNOWN:
				default:
					offset = 0;
					goto __exit_point;
				}
			}

			offset += clickplc_ckp_scr_single_options_enc(&stream[offset],method,body->int2txt_not_suppress_zero_flt2txt_exponent_form,body->is_one_shot);
		}
		break;

	case CODE_COPY_BLOCK:
	case CODE_COPY_FILL:
	case CODE_COPY_PACK:
	case CODE_COPY_UNPACK:
		if (body->is_one_shot)
		{
			stream[offset + 0] = 0xf8;
			stream[offset + 1] = 0x11;
			offset += 2;
		}
		stream[offset + 0] = 0x23;
		stream[offset + 1] = 0x22;
		stream[offset + 2] = body->type - CODE_COPY_SINGLE;
		stream[offset + 3] = 0x00;
		stream[offset + 4] = 0x00;
		offset += 5;
	}

	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Copy);

	//$$$
	if (body->is_one_shot == 0)
	{
		//00 00
		stream[offset+0] = 0x00;
		stream[offset+1] = 0x00;
		offset += 2;
	}
	else
	{
		//01 00 01
		stream[offset+0] = 0x01;
		stream[offset+1] = 0x00;
		stream[offset+2] = 0x01;
		offset += 3;
	}

__exit_point:
	return(offset);
}

#define SCR_SHIFTREG_MARKER_START	0x6066
#define SCR_SHIFTREG_MARKER_END		0x6067

int clickplc_scr_ShiftReg_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_ShiftRegister* body = (SCR_Operation_ShiftRegister*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
	unsigned short marker;
		
	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_ShiftReg);
	////////////////////////////////////////////////////
	//parse and decode {param-marker + pascal-string}:
	for(marker = SCR_SHIFTREG_MARKER_START; marker <= SCR_SHIFTREG_MARKER_END; marker++)
	{
		unsigned short tmarker;

		clickplc_ckp_int16_dec(&stream[offset],&tmarker);

		if ( marker == tmarker )
		{
			offset += 2;

			if (is_pascal_string(&stream[offset],text,&n_bytes))
			{				
				int textsize = (n_bytes-2)/2;
				EXE_Register reg;

				offset += n_bytes;

				if (clickplc_reg_cstr_dec(text,&reg))
				{
					switch(marker)
					{
					case SCR_SHIFTREG_MARKER_START:
						memcpy(&body->start_ref,&reg,sizeof(EXE_Register));
						break;

					case SCR_SHIFTREG_MARKER_END:
						memcpy(&body->end_ref,&reg,sizeof(EXE_Register));
						break;
					}
				}
				else
				{
					printf("Cant' decode Reg from ShiftReg at marker [%04x].\n",tmarker);
					offset = 0;
					goto __exit_point;
				}
			}
			else
			{
				printf("Cant' decode pascal string in ShiftReg at marker [%04x].\n",tmarker);
				offset = 0;
				goto __exit_point;
			}
		}
		else
		{
			printf("Unknown marker in ShiftReg: [%04x].\n",tmarker);
			offset = 0;
			goto __exit_point;
		}
	}

	//00 00
	offset += 2;
	
	////////////////////////////////////////////////////	
	//$$$
	//00 00
	offset += 2;

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_ShiftReg_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_ShiftRegister* body = (SCR_Operation_ShiftRegister*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int n_bytes;
	unsigned short marker;
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_ShiftReg].enc_string,
								SCR_Ops_DataBase[SCR_Op_ShiftReg].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_ShiftReg].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE_SRCNT;
	//////////////////////////////////////////////////////////////////////////
	//encode {param-marker + pascal-string}:
	for(marker = SCR_SHIFTREG_MARKER_START; marker <= SCR_SHIFTREG_MARKER_END; marker++)
	{
		EXE_Register *preg;
		
		clickplc_ckp_int16_enc(marker,&stream[offset]);
		offset += 2;

		switch(marker)
		{
		case SCR_SHIFTREG_MARKER_START:
			preg = &body->start_ref;
			break;

		case SCR_SHIFTREG_MARKER_END:
			preg = &body->end_ref;
			break;
		}
		
		n_bytes = clickplc_reg_pascal_enc(preg,&stream[offset]);
		offset += n_bytes;
	}
	
	//00 00
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;

	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_ShiftReg);

	//$$$
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;

	return(offset);
}

#define SCR_COUNTER_MARKER_2_sp		0x606a	//sp  - const (int16 or 32), or reg (DSz or DDz)
#define SCR_COUNTER_MARKER_1_CTx	0x606b	//CTx - counter's name
#define SCR_COUNTER_MARKER_3_CTDx	0x606c	//CTDx- counter
#define SCR_COUNTER_MARKER_4_CTx	0x606d	//CTx - counter done bit reg

int clickplc_scr_Counter_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Counter* body = (SCR_Operation_Counter*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
	int i_marker;

	body->is_sp_const_number = 0;//sp is reg by default
	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Counter);
	////////////////////////////////////////////////////
	//parse and decode {param-marker + pascal-string}:
	for(i_marker = 0; i_marker < 4; i_marker++)
	{
		unsigned short marker;

		offset += clickplc_ckp_int16_dec(&stream[offset],&marker);		

		if (marker >= SCR_COUNTER_MARKER_2_sp && marker <= SCR_COUNTER_MARKER_4_CTx)
		{
			if (is_pascal_string(&stream[offset],text,&n_bytes))
			{				
				int textsize = (n_bytes-2)/2;
				int data_type;

				EXE_Register reg;
				EXE_Register ptrreg;
				short const_int16;
				unsigned char const_hex8bit;
				unsigned short const_hex16bit;
				unsigned int const_hex32bit;
				int const_int32;
				float const_flt32;
				char ascii_str[CODE_COPY_ASCII_MAX+1];
				int ascii_strlen;

				offset += n_bytes;

				//try to define type of argument
				//reg, reg[pointer], int16, int32, hex16, text
				data_type = clickplc_ckp_str_parse(	text,textsize,
													&reg,&ptrreg,
													&const_hex8bit,&const_hex16bit,&const_hex32bit,
													&const_int16,&const_int32,&const_flt32,
													ascii_str,CODE_COPY_ASCII_MAX,&ascii_strlen);
				switch(data_type)
				{				
				case CODE_DATATYPE_REG:
					{
						EXE_Register *preg = 0;

						switch(marker)
						{
						case SCR_COUNTER_MARKER_1_CTx:
							preg = &body->out_complete_bit_ref;
							break;
						case SCR_COUNTER_MARKER_4_CTx:
							//skip it
							break;

						case SCR_COUNTER_MARKER_2_sp:
							//body->is_sp_const_number = 0;
							preg = &body->sp_reg_ref;
							break;

						case SCR_COUNTER_MARKER_3_CTDx:
							preg = &body->counter_reg_ref;
							break;
						}
						if (preg)
						{
							memcpy(preg,&reg,sizeof(EXE_Register));
						}
					}
					break;

				case CODE_DATATYPE_CONSTDEC16b:
					if (marker == SCR_COUNTER_MARKER_2_sp)
					{
						body->is_sp_const_number = 1;
						body->sp_const_number = (int)const_int16;
					}
					else
					{
						printf("Unsupported [int16] (encoded in Cnt) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTDEC32b:
					if (marker == SCR_COUNTER_MARKER_2_sp)
					{
						body->is_sp_const_number = 1;
						body->sp_const_number = const_int32;
					}
					else
					{
						printf("Unsupported [int32] (encoded in Cnt) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_REGBYPTR:
				case CODE_DATATYPE_CONSTHEX8:
				case CODE_DATATYPE_CONSTHEX16:
				case CODE_DATATYPE_CONSTHEX32:
				case CODE_DATATYPE_CONSTFLT32:
				case CODE_DATATYPE_CONSTSTR:
					//unsupported
					printf("Unsupported data type (encoded in Cnt) at marker [%04x].\n",marker);
					offset = 0;
					goto __exit_point;
					break;

				case CODE_DATATYPE_UNKNOWN:
				default:
					printf("Can't define data type (encoded in Cnt) at marker [%04x].\n",marker);
					offset = 0;
					goto __exit_point;
				}
			}
			else
			{
				printf("Cant' decode pascal string in Cnt at marker [%04x].\n",marker);
				offset = 0;
				goto __exit_point;
			}
		}
		else
		{
			printf("Unknown marker in Cnt: [%04x].\n",marker);
			offset = 0;
			goto __exit_point;
		}
	}

	//	UP:			00 00
	//	DOWN:		fc 21 01 05 3a 00 00 00 00 01 00 2b 22 02 00 2c 22 ff ff	+ 00 00
	//	UP&DOWN:	fc 21 02 05 3a 00 00 09 22 01 00 0a 22 02 00 0b 22 ff ff	+ 00 00

	if (stream[offset + 0]==0x00 && stream[offset + 1]==0x00)
	{//UP
		body->count_type = COUNTER_COUNT_TYPE_UP;
		offset += 2;
	}
	else
	{
		if (stream[offset + 0]==0xfc && stream[offset + 1]==0x21)
		{
			switch (stream[offset + 2])
			{
			case 0x01://DOWN
				body->count_type = COUNTER_COUNT_TYPE_DOWN;
				offset += 21;
				break;

			case 0x02://UP&DOWN
				body->count_type = COUNTER_COUNT_TYPE_UPDOWN;
				offset += 21;
				break;

			default:
				printf("Unknown byte[2] after last parameter in Cnt: %02x\n",stream[offset + 2]);
			}
		}
		else
		{
			printf("Unknown bytes[0,1] after last parameter in Cnt: %02x,%02x\n",stream[offset + 0],stream[offset + 1]);
			offset = 0;
			goto __exit_point;
		}
	}

	////////////////////////////////////////////////////	
	//$$$
	//00 00
	offset += 2;

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_Counter_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Counter* body = (SCR_Operation_Counter*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int i_marker;
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Counter].enc_string,
								SCR_Ops_DataBase[SCR_Op_Counter].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Counter].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE_SRCNT;
	//////////////////////////////////////////////////////////////////////////
	//encode {param-marker + pascal-string}:
	for(i_marker = 0; i_marker < 4; i_marker++)
	{
		unsigned short marker;
		void* prm_ptr0 = 0;
		void* prm_ptr1 = 0;
		int data_type;

		//prm_ptr0 = 0; prm_ptr1 = 0;

		switch(i_marker)
		{
		case 0:
			marker = SCR_COUNTER_MARKER_1_CTx;
			prm_ptr0 = (void*)&body->out_complete_bit_ref;
			data_type = CODE_DATATYPE_REG;
			break;

		case 1:
			marker = SCR_COUNTER_MARKER_2_sp;
			if (body->is_sp_const_number == 0)
			{
				prm_ptr0 = (void*)&body->sp_reg_ref;
				data_type = CODE_DATATYPE_REG;
			}
			else
			{
				prm_ptr0 = (void*)&body->sp_const_number;
				data_type = CODE_DATATYPE_CONSTDEC32b;
			}
			break;

		case 2:
			marker = SCR_COUNTER_MARKER_3_CTDx;
			prm_ptr0 = (void*)&body->counter_reg_ref;
			data_type = CODE_DATATYPE_REG;
			break;

		case 3:
			marker = SCR_COUNTER_MARKER_4_CTx;
			prm_ptr0 = (void*)&body->out_complete_bit_ref;
			data_type = CODE_DATATYPE_REG;
			break;
		}
		
		//marker
		offset += clickplc_ckp_int16_enc(marker,&stream[offset]);
		//parameter
		offset += clickplc_ckp_encode_parameter_as_pascal_str(&stream[offset],data_type,prm_ptr0,prm_ptr1,0,0,0);
	}
	
	switch(body->count_type)
	{
	case COUNTER_COUNT_TYPE_UP://	UP:			00 00	
		stream[offset + 0] = 0x00;
		stream[offset + 1] = 0x00;
		offset += 2;
		break;

	case COUNTER_COUNT_TYPE_DOWN://	DOWN:			fc 21 01 05 3a 00 00 00 00 01 00 2b 22 02 00 2c 22 ff ff	+ 00 00
		
		stream[offset + 0] = 0xfc;	stream[offset + 1] = 0x21;

		stream[offset + 2] = 0x01;

		stream[offset + 3] = 0x05;	stream[offset + 4] = 0x3a;
		stream[offset + 5] = 0x00;	stream[offset + 6] = 0x00;

		stream[offset + 7] = 0x00;//*
		stream[offset + 8] = 0x00;//*

		stream[offset + 9] = 0x01;	stream[offset + 10] = 0x00;

		stream[offset + 11] = 0x2b;//*
		stream[offset + 12] = 0x22;
		stream[offset + 13] = 0x02;
		stream[offset + 14] = 0x00;
		stream[offset + 15] = 0x2c;//*
		stream[offset + 16] = 0x22;

		stream[offset + 17] = 0xff;	stream[offset + 18] = 0xff;
		stream[offset + 19] = 0x00;	stream[offset + 20] = 0x00;

		offset += 21;
		break;

	case COUNTER_COUNT_TYPE_UPDOWN://	UP&DOWN:	fc 21 02 05 3a 00 00 09 22 01 00 0a 22 02 00 0b 22 ff ff	+ 00 00

		stream[offset + 0] = 0xfc;	stream[offset + 1] = 0x21;

		stream[offset + 2] = 0x02;//*
		
		stream[offset + 3] = 0x05;	stream[offset + 4] = 0x3a;
		stream[offset + 5] = 0x00;	stream[offset + 6] = 0x00;
		
		stream[offset + 7] = 0x09;//*
		stream[offset + 8] = 0x22;//*
		
		stream[offset + 9] = 0x01;	stream[offset + 10] = 0x00;
		
		stream[offset + 11] = 0x0a;//*	
		stream[offset + 12] = 0x22;
		stream[offset + 13] = 0x02;
		stream[offset + 14] = 0x00;		
		stream[offset + 15] = 0x0b;//*	
		stream[offset + 16] = 0x22;

		stream[offset + 17] = 0xff;	stream[offset + 18] = 0xff;
		stream[offset + 19] = 0x00;	stream[offset + 20] = 0x00;

		offset += 21;
		break;

	default:
		printf("enc: Unknown type of Cnt: %d\n",body->count_type);
	}
	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Counter);

	//$$$
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;

	return(offset);
}

#define SCR_TIMER_MARKER_1_Tx	0x6068
#define SCR_TIMER_MARKER_3_TDx	0x6069
#define SCR_TIMER_MARKER_2_sp	0x606a

#define SCR_TIMER_UNIT_CODE		0x21f9
#define SCR_TIMER_UNIT_sec		0x01
#define SCR_TIMER_UNIT_min		0x02
#define SCR_TIMER_UNIT_hrs		0x03
#define SCR_TIMER_UNIT_day		0x04

int clickplc_scr_Timer_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Timer* body = (SCR_Operation_Timer*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
	int i_marker;
	unsigned short marker;

	body->is_sp_const_number = 0;//sp is reg by default
	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Timer);
	////////////////////////////////////////////////////
	//parse and decode {param-marker + pascal-string}:
	for(i_marker = 0; i_marker < 3; i_marker++)
	{
		offset += clickplc_ckp_int16_dec(&stream[offset],&marker);

		if (marker >= SCR_TIMER_MARKER_1_Tx && marker <= SCR_TIMER_MARKER_2_sp)
		{
			if (is_pascal_string(&stream[offset],text,&n_bytes))
			{				
				int textsize = (n_bytes-2)/2;
				int data_type;

				EXE_Register reg;
				EXE_Register ptrreg;
				short const_int16;
				unsigned char const_hex8bit;
				unsigned short const_hex16bit;
				unsigned int const_hex32bit;
				int const_int32;
				float const_flt32;
				char ascii_str[CODE_COPY_ASCII_MAX+1];
				int ascii_strlen;

				offset += n_bytes;

				//try to define type of argument
				//reg, reg[pointer], int16, int32, hex16, text
				data_type = clickplc_ckp_str_parse(	text,textsize,
													&reg,&ptrreg,
													&const_hex8bit,&const_hex16bit,&const_hex32bit,
													&const_int16,&const_int32,&const_flt32,
													ascii_str,CODE_COPY_ASCII_MAX,&ascii_strlen);
				switch(data_type)
				{				
				case CODE_DATATYPE_REG:
					{
						EXE_Register *preg = 0;

						switch(marker)
						{
						case SCR_TIMER_MARKER_1_Tx:
							preg = &body->timer_bit_ref;
							break;
						case SCR_TIMER_MARKER_2_sp:
							//body->is_sp_const_number = 0;
							preg = &body->sp_reg_ref;
							break;

						case SCR_TIMER_MARKER_3_TDx:
							preg = &body->counter_reg_ref;
							break;
						}
						if (preg)
						{
							memcpy(preg,&reg,sizeof(EXE_Register));
						}
					}
					break;

				case CODE_DATATYPE_CONSTDEC16b:
					
					if (marker == SCR_COUNTER_MARKER_2_sp)
					{
						body->is_sp_const_number = 1;
						body->sp_const_number = const_int16;
					}
					else
					{
						printf("Unsupported [int16] (encoded in Tmr) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTDEC32b:
					printf("Unsupported [int32] (encoded in Tmr) at marker [%04x].\n",marker);
					offset = 0;
					goto __exit_point;
					break;

				case CODE_DATATYPE_REGBYPTR:
				case CODE_DATATYPE_CONSTHEX8:
				case CODE_DATATYPE_CONSTHEX16:
				case CODE_DATATYPE_CONSTHEX32:
				case CODE_DATATYPE_CONSTFLT32:
				case CODE_DATATYPE_CONSTSTR:
					//unsupported
					printf("Unsupported data type (encoded in Tmr) at marker [%04x].\n",marker);
					offset = 0;
					goto __exit_point;
					break;

				case CODE_DATATYPE_UNKNOWN:
				default:
					printf("Can't define data type (encoded in Tmr) at marker [%04x].\n",marker);
					offset = 0;
					goto __exit_point;
				}
			}
			else
			{
				printf("Cant' decode pascal string in Tmr at marker [%04x].\n",marker);
				offset = 0;
				goto __exit_point;
			}
		}
		else
		{
			printf("Unknown marker in Tmr: [%04x].\n",marker);
			offset = 0;
			goto __exit_point;
		}
	}

	//units
	offset += clickplc_ckp_int16_dec(&stream[offset],&marker);
	switch(marker)
	{
	case 0x0000://ON + NotRetained !ms!
		
		body->flags = 0x00;
		body->units = TIMER_UNITS_MS;
		break;

	case 0x21fb://ON + Retained	!ms!
		body->flags = TIMER_FLAGS_RETAINED;			
		body->units = TIMER_UNITS_MS;
		offset += 17-2;
		break;

	case 0x21fa://OFF + Not Retained !ms!
		body->flags = TIMER_FLAGS_OFFDELAY;
		body->units = TIMER_UNITS_MS;
		offset += 13-2;
		break;

	case SCR_TIMER_UNIT_CODE:

		switch (stream[offset + 0])
		{
		case SCR_TIMER_UNIT_sec:body->units = TIMER_UNITS_SEC;
			break;
		case SCR_TIMER_UNIT_min:body->units = TIMER_UNITS_MIN;
			break;
		case SCR_TIMER_UNIT_hrs:body->units = TIMER_UNITS_HOUR;
			break;
		case SCR_TIMER_UNIT_day:body->units = TIMER_UNITS_DAY;
			break;
		default:
			printf("enc: Unknown type of units in Tmr: 0x%02x\n",stream[offset + 0]);
			offset = 0;
			goto __exit_point;
		}

		//step over units byte
		offset++;

		//options
		clickplc_ckp_int16_dec(&stream[offset],&marker);

		switch(marker)
		{
		case 0x3a05://ON + Not Retained
			body->flags = 0x00;			
			offset += 10;
			break;

		case 0x21fb://ON + Retained
			body->flags = TIMER_FLAGS_RETAINED;			
			offset += 17;
			break;

		case 0x21fa://OFF + Not Retained
			body->flags = TIMER_FLAGS_OFFDELAY;
			offset += 13;
			break;

		default:
			printf("enc: Unknown 2 bytes [in place of options1] in Tmr: %02x,%02x\n",stream[offset + 0],stream[offset + 1]);
			offset = 0;
			goto __exit_point;
		}
		break;

	default:
		printf("enc: Unknown marker [in place of options0] in Tmr: %04x\n",marker);
		offset = 0;
		goto __exit_point;
	}

	////////////////////////////////////////////////////
	//$$$
	//00 00
	offset += 2;

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_Timer_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Timer* body = (SCR_Operation_Timer*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int i_marker;
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Timer].enc_string,
								SCR_Ops_DataBase[SCR_Op_Timer].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Timer].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE_TMR;
	//////////////////////////////////////////////////////////////////////////
	//encode {param-marker + pascal-string}:
	for(i_marker = 0; i_marker < 3; i_marker++)
	{
		unsigned short marker;
		void* prm_ptr0 = 0;
		void* prm_ptr1 = 0;
		int data_type;

		//prm_ptr0 = 0; prm_ptr1 = 0;

		switch(i_marker)
		{
		case 0:
			marker = SCR_TIMER_MARKER_1_Tx;
			prm_ptr0 = (void*)&body->timer_bit_ref;
			data_type = CODE_DATATYPE_REG;
			break;

		case 1:
			marker = SCR_TIMER_MARKER_2_sp;
			if (body->is_sp_const_number == 0)
			{
				prm_ptr0 = (void*)&body->sp_reg_ref;
				data_type = CODE_DATATYPE_REG;
			}
			else
			{
				prm_ptr0 = (void*)&body->sp_const_number;
				data_type = CODE_DATATYPE_CONSTDEC16b;
			}
			break;

		case 2:
			marker = SCR_TIMER_MARKER_3_TDx;
			prm_ptr0 = (void*)&body->counter_reg_ref;
			data_type = CODE_DATATYPE_REG;
			break;
		}
		
		//marker
		offset += clickplc_ckp_int16_enc(marker,&stream[offset]);
		//parameter
		offset += clickplc_ckp_encode_parameter_as_pascal_str(&stream[offset],data_type,prm_ptr0,prm_ptr1,0,0,0);
	}
	
	//units
	switch(body->units)
	{
	case TIMER_UNITS_MS:	//ms		N/A
		break;
	case TIMER_UNITS_SEC:	//sec		f9 21 01
		stream[offset + 0] = 0xf9;
		stream[offset + 1] = 0x21;
		stream[offset + 2] = SCR_TIMER_UNIT_sec;
		offset += 3;
		break;
	case TIMER_UNITS_MIN:	//min		f9 21 02
		stream[offset + 0] = 0xf9;
		stream[offset + 1] = 0x21;
		stream[offset + 2] = SCR_TIMER_UNIT_min;
		offset += 3;
		break;
	case TIMER_UNITS_HOUR:	//hrs		f9 21 03
		stream[offset + 0] = 0xf9;
		stream[offset + 1] = 0x21;
		stream[offset + 2] = SCR_TIMER_UNIT_hrs;
		offset += 3;

		break;
	case TIMER_UNITS_DAY:	//days		f9 21 04
		stream[offset + 0] = 0xf9;
		stream[offset + 1] = 0x21;
		stream[offset + 2] = SCR_TIMER_UNIT_day;
		offset += 3;
		break;

	default:
		printf("enc: Unknown units of Tmr: %d\n",body->units);
		offset = 0;
		goto __exit_point;
	}

	//options
	switch(body->flags)
	{
	case (0x00|0x00):					//ON + Not Retained
		// ON + NotRetained	05 3a 00 00 25 22 ff ff 00 00							day
		// 					05 3a 00 00 1f 22 ff ff 00 00							hrs
		// 					05 3a 00 00 19 22 ff ff 00 00							min
		// 					05 3a 00 00 13 22 ff ff 00 00							sec
		// 					00 00													ms

		switch(body->units)
		{
		case TIMER_UNITS_MS:	//ms		N/A
			stream[offset+0] = 0x00;
			stream[offset+1] = 0x00;
			offset += 2;
			break;

		default:

			stream[offset+0] = 0x05;
			stream[offset+1] = 0x3a;
			stream[offset+2] = 0x00;
			stream[offset+3] = 0x00;
			
			stream[offset+5] = 0x22;
			stream[offset+6] = 0xff;
			stream[offset+7] = 0xff;
			stream[offset+8] = 0x00;
			stream[offset+9] = 0x00;

			switch(body->units)
			{
			case TIMER_UNITS_SEC:	//sec		f9 21 01
				stream[offset+4] = 0x13;
				break;
			case TIMER_UNITS_MIN:	//min		f9 21 02
				stream[offset+4] = 0x19;
				break;
			case TIMER_UNITS_HOUR:	//hrs		f9 21 03
				stream[offset+4] = 0x1f;
				break;
			case TIMER_UNITS_DAY:	//days		f9 21 04
				stream[offset+4] = 0x25;
				break;
			}

			offset += 10;
		}		
		break;

	case (0x00|TIMER_FLAGS_RETAINED):	//ON + Retained
		// ON + Retained	fb 21 01 05 3a 00 00 27 22 01 00 28 22 ff ff 00 00 		day
		// 					fb 21 01 05 3a 00 00 21 22 01 00 22 22 ff ff 00 00		hrs
		// 					fb 21 01 05 3a 00 00 1b 22 01 00 1c 22 ff ff 00 00 		min
		// 					fb 21 01 05 3a 00 00 15 22 01 00 16 22 ff ff 00 00		sec
		// 					fb 21 01 05 3a 00 00 0f 22 01 00 10 22 ff ff 00 00		ms

		stream[offset+0] = 0xfb;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x01;
		stream[offset+3] = 0x05;
		stream[offset+4] = 0x3a;
		stream[offset+5] = 0x00;
		stream[offset+6] = 0x00;

		stream[offset+8]  = 0x22;
		stream[offset+9]  = 0x01;
		stream[offset+10] = 0x00;

		stream[offset+12] = 0x22;
		stream[offset+13] = 0xff;
		stream[offset+14] = 0xff;
		stream[offset+15] = 0x00;
		stream[offset+16] = 0x00;

		switch(body->units)
		{
		case TIMER_UNITS_MS:	//ms		N/A
			stream[offset+7]  = 0x0f;
			stream[offset+11] = 0x10;
			break;
		case TIMER_UNITS_SEC:	//sec		f9 21 01
			stream[offset+7]  = 0x15;
			stream[offset+11] = 0x16;
			break;
		case TIMER_UNITS_MIN:	//min		f9 21 02
			stream[offset+7]  = 0x1b;
			stream[offset+11] = 0x1c;
			break;
		case TIMER_UNITS_HOUR:	//hrs		f9 21 03
			stream[offset+7]  = 0x21;
			stream[offset+11] = 0x22;
			break;
		case TIMER_UNITS_DAY:	//days		f9 21 04
			stream[offset+7]  = 0x27;
			stream[offset+11] = 0x28;
			break;
		}

		offset += 17;
		break;

	case (TIMER_FLAGS_OFFDELAY|0x00):	//ON + Not Retained
		// OFF (NotRetained)fa 21 01 05 3a 00 00 26 22 ff ff 00 00 					day
		// 					fa 21 01 05 3a 00 00 20 22 ff ff 00 00					hrs
		// 					fa 21 01 05 3a 00 00 1a 22 ff ff 00 00					min
		// 					fa 21 01 05 3a 00 00 14 22 ff ff 00 00					sec
		// 					fa 21 01 05 3a 00 00 0e 22 ff ff 00 00					ms 
		stream[offset+0] = 0xfa;
		stream[offset+1] = 0x21;
		stream[offset+2] = 0x01;
		stream[offset+3] = 0x05;
		stream[offset+4] = 0x3a;
		stream[offset+5] = 0x00;
		stream[offset+6] = 0x00;
			
		stream[offset+8]  = 0x22;
		stream[offset+9]  = 0xff;
		stream[offset+10] = 0xff;
		stream[offset+11] = 0x00;
		stream[offset+12] = 0x00;

		switch(body->units)
		{
		case TIMER_UNITS_MS:	//ms		N/A
			stream[offset+7] = 0x0e;
			break;
		case TIMER_UNITS_SEC:	//sec		f9 21 01
			stream[offset+7] = 0x14;
			break;
		case TIMER_UNITS_MIN:	//min		f9 21 02
			stream[offset+7] = 0x1a;
			break;
		case TIMER_UNITS_HOUR:	//hrs		f9 21 03			
			stream[offset+7] = 0x20;
			break;
		case TIMER_UNITS_DAY:	//days		f9 21 04			
			stream[offset+7] = 0x26;
			break;
		}

		offset += 13;
		break;

	case (TIMER_FLAGS_OFFDELAY|TIMER_FLAGS_RETAINED):	//ON + Retained -> ERROR
	default:
		
		printf("enc: unsupported [OFF + Retained ] Tmr!\n");
		offset = 0;
		goto __exit_point;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Timer);

	//$$$
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;
__exit_point:
	return(offset);
}

#define SCR_SEARCH_MARKER_1_searchin_item		0x6065
#define SCR_SEARCH_MARKER_2_search_range_first	0x6066
#define SCR_SEARCH_MARKER_3_search_range_last	0x6067
#define SCR_SEARCH_MARKER_4_result_of_search	0x6079
#define SCR_SEARCH_MARKER_5_found_reg_bit		0x607a

int clickplc_scr_Search_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Search* body = (SCR_Operation_Search*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;	
	int n_bytes;
	char text[SCR_REG_STRING_MAX_SIZE];
	int i_marker;
	unsigned short marker;

	//defaults
	body->is_one_shot			= 0;
	body->is_continuous_search	= 0;
	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Search);
	////////////////////////////////////////////////////	
	
	for (i_marker = 0; i_marker < 5; i_marker++)
	{		
		offset += clickplc_ckp_int16_dec(&stream[offset],&marker);

		if (	(marker >= SCR_SEARCH_MARKER_1_searchin_item && marker <= SCR_SEARCH_MARKER_3_search_range_last)
				||
				(marker >= SCR_SEARCH_MARKER_4_result_of_search && marker <= SCR_SEARCH_MARKER_5_found_reg_bit)
			)
		{
			if (is_pascal_string(&stream[offset],text,&n_bytes))
			{				
				int textsize = (n_bytes-2)/2;
				int data_type;

				EXE_Register reg;
				EXE_Register ptrreg;
				short const_int16;
				unsigned char const_hex8bit;
				unsigned short const_hex16bit;
				unsigned int const_hex32bit;
				int const_int32;
				float const_flt32;
				char ascii_str[CODE_COPY_ASCII_MAX+1];
				int ascii_strlen;

				offset += n_bytes;

				//try to define type of argument
				//reg, reg[pointer], int16, int32, flt32, hex16, text
				data_type = clickplc_ckp_str_parse(	text,textsize,
													&reg,&ptrreg,
													&const_hex8bit,&const_hex16bit,&const_hex32bit,
													&const_int16,&const_int32,&const_flt32,
													ascii_str,CODE_COPY_ASCII_MAX,&ascii_strlen);
				switch(data_type)
				{				
				case CODE_DATATYPE_REG:
					{
						EXE_Register *preg = 0;

						switch(marker)
						{
						case SCR_SEARCH_MARKER_1_searchin_item:
							preg = &body->searching_reg_ref;
							body->searching_container = EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_REGISTER;
							break;
						case SCR_SEARCH_MARKER_2_search_range_first:
							//body->is_sp_const_number = 0;
							preg = &body->search_range_first_reg_ref;
							break;

						case SCR_SEARCH_MARKER_3_search_range_last:
							preg = &body->search_range_last_reg_ref;
							break;
						case SCR_SEARCH_MARKER_4_result_of_search:
							preg = &body->result_reg_ref;
							break;
						case SCR_SEARCH_MARKER_5_found_reg_bit:
							preg = &body->found_bit_ref;
							break;
						}
						if (preg)
						{
							memcpy(preg,&reg,sizeof(EXE_Register));
						}
					}
					break;

				case CODE_DATATYPE_CONSTDEC16b:

					if (marker == SCR_SEARCH_MARKER_1_searchin_item)
					{						
						body->searching_const_int16 = const_int16;
						body->searching_container = EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC16bit;
					}
					else
					{
						printf("Unsupported [int16] (encoded in Search) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTDEC32b:
					if (marker == SCR_SEARCH_MARKER_1_searchin_item)
					{						
						body->searching_const_int32 = const_int32;
						body->searching_container = EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC32bit;
					}
					else
					{
						printf("Unsupported [int32] (encoded in Search) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTHEX8:
					if (marker == SCR_SEARCH_MARKER_1_searchin_item)
					{						
						body->searching_const_hex16bit = const_hex8bit;
						body->searching_container = EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX8;
					}
					else
					{
						printf("Unsupported [hex8] (encoded in Search) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTHEX16:
					if (marker == SCR_SEARCH_MARKER_1_searchin_item)
					{						
						body->searching_const_hex16bit = const_hex16bit;
						body->searching_container = EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX16;
					}
					else
					{
						printf("Unsupported [hex16] (encoded in Search) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTFLT32:
					if (marker == SCR_SEARCH_MARKER_1_searchin_item)
					{						
						body->searching_const_flt32 = const_flt32;
						body->searching_container = EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_FLT32;

						body->searching_string_size = textsize;
						memcpy(body->searching_string,text,textsize);
						body->searching_string[textsize] = 0x00;
					}
					else
					{
						printf("Unsupported [flt32] (encoded in Search) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTSTR:
					if (marker == SCR_SEARCH_MARKER_1_searchin_item)
					{						
						body->searching_container = EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_ASCIISTR;
						
						body->searching_string_size = ascii_strlen;
						memcpy(body->searching_string,ascii_str,ascii_strlen);
						body->searching_string[ascii_strlen] = 0x00;

					}
					else
					{
						printf("Unsupported [ascii-string] (encoded in Search) at marker [%04x].\n",marker);
						offset = 0;
						goto __exit_point;
					}
					break;

				case CODE_DATATYPE_CONSTHEX32:
				case CODE_DATATYPE_REGBYPTR:
				case CODE_DATATYPE_UNKNOWN:
				default:
					printf("Can't define data type (encoded in Search) at marker [%04x].\n",marker);
					offset = 0;
					goto __exit_point;
				}
			}
			else
			{
				printf("Cant' decode pascal string in Search at marker [%04x].\n",marker);
				offset = 0;
				goto __exit_point;
			}
		}
		else
		{
			printf("Unknown marker in Tmr: [%04x].\n",marker);
			offset = 0;
			goto __exit_point;
		}
	}

	//options
	//search condition

	offset += clickplc_ckp_int16_dec(&stream[offset],&marker);

	if (marker == 0x1277)
	{
		body->is_continuous_search = 1;		
		offset += clickplc_ckp_int16_dec(&stream[offset],&marker);
	}
// 	else
// 	{
// 		body->is_continuous_search = 0;
// 
// 	}

	if (marker == 0x11f8)
	{
		body->is_one_shot = 1;
		offset += clickplc_ckp_int16_dec(&stream[offset],&marker);
	}
// 	else
// 	{
// 		body->is_one_shot = 0;
// 	}

	switch(marker)
	{
	case 0x0000:
		body->search_condition = EXE_CODE_INSTRUCTION_SEARCH_EQUAL;
		break;

	case 0x21f7:
		{
			unsigned char condition_byte;

			condition_byte = stream[offset + 0];
			offset++;

			switch(condition_byte)
			{
			case 0x01:	body->search_condition = EXE_CODE_INSTRUCTION_SEARCH_NOTEQUAL;
				break;
			case 0x02:	body->search_condition = EXE_CODE_INSTRUCTION_SEARCH_MORE;
				break;
			case 0x03:	body->search_condition = EXE_CODE_INSTRUCTION_SEARCH_LESS;
				break;
			case 0x04:	body->search_condition = EXE_CODE_INSTRUCTION_SEARCH_MOREOREQUAL;
				break;
			case 0x05:	body->search_condition = EXE_CODE_INSTRUCTION_SEARCH_LESSOREQUAL;
				break;
			default:
				printf("Unknown condition byte in Search: [%02x].\n",condition_byte);
				offset = 0;
				goto __exit_point;
			}

			//0x00, 0x00;
			offset += 2;
		}
		break;

	default:
		printf("Unknown 2 condition bytes in Search: [%02x,%02x].\n",stream[offset - 2],stream[offset - 1]);
		offset = 0;
		goto __exit_point;
	}
	////////////////////////////////////////////////////	
	//$$$
	if(body->is_continuous_search == 0)
	{//OFF
		if (body->is_one_shot == 0)
		{
			//0x00, 0x00;
			offset += 2;
		}
		else
		{//1s
			//0x01,0x00,0x01;
			offset += 3;
		}
	}
	else
	{//ON
		if (body->is_one_shot == 0)
		{
			//0x01,0x00,0x01;
			offset += 3;
		}
		else
		{//1s
			//0x01,0x00,0x03;
			offset += 3;
		}
	}		
	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_Search_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Search* body = (SCR_Operation_Search*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int i_marker;
	
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Search].enc_string,
								SCR_Ops_DataBase[SCR_Op_Search].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Search].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	for (i_marker = 0; i_marker < 5; i_marker++)
	{
		unsigned short marker;
		
		void* prm_ptr0 = 0;
		void* prm_ptr1 = 0;
		unsigned char* orig_text = 0;
		unsigned short orig_text_size = 0;
		unsigned short* porig_text_size = 0;
		int data_type = CODE_DATATYPE_UNKNOWN;
		
		switch(i_marker)
		{
		case 0:
			
			marker = SCR_SEARCH_MARKER_1_searchin_item;
			
			switch(body->searching_container)
			{
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_REGISTER:
				prm_ptr0 = (void*)&body->searching_reg_ref;
				data_type = CODE_DATATYPE_REG;
				break;
				
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC16bit:
				prm_ptr0 = (void*)&body->searching_const_int16;
				data_type = CODE_DATATYPE_CONSTDEC16b;
				break;
				
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_DEC32bit:
				prm_ptr0 = (void*)&body->searching_const_int32;
				data_type = CODE_DATATYPE_CONSTDEC32b;
				break;

			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX8:
				prm_ptr0 = (void*)&body->searching_const_hex16bit;
				data_type = CODE_DATATYPE_CONSTHEX8;
				break;
				
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_HEX16:
				prm_ptr0 = (void*)&body->searching_const_hex16bit;
				data_type = CODE_DATATYPE_CONSTHEX16;
				break;
				
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_FLT32:
				prm_ptr0 = (void*)&body->searching_const_flt32;
				data_type = CODE_DATATYPE_CONSTFLT32;
				if (body->searching_string_size > 0)
				{					
					orig_text_size = body->searching_string_size;
				}
				orig_text = body->searching_string;
				porig_text_size = &body->searching_string_size;
				break;
				
			case EXE_CODE_INSTRUCTION_SEARCH_CONTAINER_ASCIISTR:
				prm_ptr0 = (void*)body->searching_string;
				prm_ptr1 = (void*)&body->searching_string_size;
				data_type = CODE_DATATYPE_CONSTSTR;
				break;
			}
			break;

		case 1:

			marker = SCR_SEARCH_MARKER_2_search_range_first;
			data_type = CODE_DATATYPE_REG;
			prm_ptr0 = (void*)&body->search_range_first_reg_ref;
			break;

		case 2:

			marker = SCR_SEARCH_MARKER_3_search_range_last;
			data_type = CODE_DATATYPE_REG;
			prm_ptr0 = (void*)&body->search_range_last_reg_ref;
			break;

		case 3:

			marker = SCR_SEARCH_MARKER_4_result_of_search;
			data_type = CODE_DATATYPE_REG;
			prm_ptr0 = (void*)&body->result_reg_ref;
			break;

		case 4:

			marker = SCR_SEARCH_MARKER_5_found_reg_bit;
			data_type = CODE_DATATYPE_REG;
			prm_ptr0 = (void*)&body->found_bit_ref;
			break;
		}
		
		//marker
		offset += clickplc_ckp_int16_enc(marker,&stream[offset]);
		//parameter
		offset += clickplc_ckp_encode_parameter_as_pascal_str(&stream[offset],data_type,prm_ptr0,prm_ptr1,
																orig_text,orig_text_size,porig_text_size);
	}
	
	//options
	if(body->is_continuous_search == 0)
	{//OFF
		if (body->is_one_shot == 1)
		{//1s
			stream[offset+0] = 0xf8;
			stream[offset+1] = 0x11;
			offset += 2;
		}
	}
	else
	{//ON

		stream[offset+0] = 0x77;
		stream[offset+1] = 0x12;
		offset += 2;

		if (body->is_one_shot == 1)
		{
			stream[offset+0] = 0xf8;
			stream[offset+1] = 0x11;
			offset += 2;
		}
	}		
	//search_condition
	switch(body->search_condition)
	{
	case EXE_CODE_INSTRUCTION_SEARCH_EQUAL:
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_NOTEQUAL:
		stream[offset+0] = 0xf7; stream[offset+1] = 0x21; stream[offset+2] = 0x01;
		offset += 3;
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_MORE:
		stream[offset+0] = 0xf7; stream[offset+1] = 0x21; stream[offset+2] = 0x02;
		offset += 3;
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_MOREOREQUAL:
		stream[offset+0] = 0xf7; stream[offset+1] = 0x21; stream[offset+2] = 0x04;
		offset += 3;
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_LESS:
		stream[offset+0] = 0xf7; stream[offset+1] = 0x21; stream[offset+2] = 0x03;
		offset += 3;
		break;
	case EXE_CODE_INSTRUCTION_SEARCH_LESSOREQUAL:
		stream[offset+0] = 0xf7; stream[offset+1] = 0x21; stream[offset+2] = 0x05;
		offset += 3;
		break;
	}	
	stream[offset+0] = 0x00;
	stream[offset+1] = 0x00;
	offset += 2;

	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Search);
	
	//$$$
	if(body->is_continuous_search == 0)
	{//OFF
		if (body->is_one_shot == 0)
		{
			stream[offset+0] = 0x00;
			stream[offset+1] = 0x00;
			offset += 2;
		}
		else
		{//1s
			stream[offset+0] = 0x01;
			stream[offset+1] = 0x00;
			stream[offset+2] = 0x01;
			offset += 3;
		}
	}
	else
	{//ON
		if (body->is_one_shot == 0)
		{
			stream[offset+0] = 0x01;
			stream[offset+1] = 0x00;
			stream[offset+2] = 0x01;
			offset += 3;
		}
		else
		{//1s
			stream[offset+0] = 0x01;
			stream[offset+1] = 0x00;
			stream[offset+2] = 0x03;
			offset += 3;
		}
	}	
	return(offset);
}

// #define CODE_DATATYPE_FORMULA		9
// #define CODE_DATATYPE_FORMULA_A		10
// #define CODE_DATATYPE_FORMULA_N		11
// #define CODE_DATATYPE_FORMULA_NA	12
//returns: type of data; writes parsed&decoded data (according to type)
int clickplc_ckp_math_decode(	char* cstr_string,int str_len,		//IN
								int				exp_datatype,		//IN
								EXE_Register*	preg,				//OUT
								unsigned char*	formula,			//OUT
								int				formula_lenlim,		//IN
								unsigned short*	pformula_len		//OUT
								)
{
	int data_type = CODE_DATATYPE_UNKNOWN;
	int encsize = 0;

	switch(exp_datatype)
	{
	case CODE_DATATYPE_REG:
		if(preg)
		{
			encsize = clickplc_reg_cstr_dec(cstr_string,preg);
		}
		break;

	case CODE_DATATYPE_FORMULA:		
	case CODE_DATATYPE_FORMULA_A:
	case CODE_DATATYPE_FORMULA_N:
	case CODE_DATATYPE_FORMULA_NA:
		
		if ( formula && (formula_lenlim > (str_len + 1)) )
		{
			memcpy(formula,cstr_string,str_len);
			formula[str_len] = 0x00;
			if(pformula_len) *pformula_len = str_len;
			//encsize = str_len;
		}		
		break;

	default:
		break;
	}


	return(data_type);
}

//returns: size of encoded data (bytes):
int clickplc_ckp_math_encode(		unsigned char*	stream,				//IN/OUT
									int				prm_datatype,		//IN
									void*			prm0,				//IN
									void*			prm1				//IN
									)
{
	int encsize = 0;

	switch(prm_datatype)
	{
	case CODE_DATATYPE_REG:
		encsize = clickplc_reg_pascal_enc((EXE_Register*)prm0,stream);
		break;

	case CODE_DATATYPE_FORMULA:		
	case CODE_DATATYPE_FORMULA_A:
	case CODE_DATATYPE_FORMULA_N:
	case CODE_DATATYPE_FORMULA_NA:
		encsize = enc_pascal_string((char*)prm0,(int)(*(unsigned short*)prm1),stream);
		break;

	default:
		break;
	}

	return(encsize);
}


// 0x6065		result reg
// 
// 0x11fe		hex
// 0x11f8		1s
// 0x61ff		formula with @
// 0x6228		original formula
// 0x6229		formula with #@
// 0x61fd		formula with #
// 
// 24,22,01	nicknames
// 
// 0x6888		start of address list
// 0x0000		first address
// 0x0001		second address
// ...
// ff ff		end of address list
// 
// 00 00
// $
// 01 00 01	hex
// 01 00 03	hex + 1s
// 01 00 01	decimal + 1s

#define SCR_MATH_MARKER_1_RESULTREG			0x6065
#define SCR_MATH_MARKER_2_IS_HEX			0x11fe
#define SCR_MATH_MARKER_3_IS_1SHOT			0x11f8
#define SCR_MATH_MARKER_4_FORMULA_A			0x61ff
#define SCR_MATH_MARKER_5_FORMULA			0x6228
#define SCR_MATH_MARKER_6_FORM_NA			0x6229
#define SCR_MATH_MARKER_7_FORM_N			0x61fd
#define SCR_MATH_MARKER_8_BY_NICK			0x2224
#define SCR_MATH_MARKER_9_ADDRLIST_START	0x6888
#define SCR_MATH_MARKER_10_ADDRLIST_END		0xffff

int clickplc_scr_Math_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	SCR_Operation_Math* body = (SCR_Operation_Math*)obj;
	unsigned char* stream = stream_after_opcode;
	int offset;
	char text[EXE_CODE_INSTRUCTION_MATH_ASCII_MAX];
	int with_vars;
	int is_last_was_done;
	int goon;

	body->is_hex = 0;
	body->is_one_shot = 0;
	body->is_vars_by_nicknames = 0;
	body->n_vars = 0;
	body->formula_size = 0;
	body->formula_a_size = 0;
	body->formula_n_size = 0;	
	body->formula_na_size = 0;

	////////////////////////////////////////////////////
	offset = clickplc_scr_info_dec(stream,&body->info,SCR_Op_Math);
	////////////////////////////////////////////////////	
	//
	//parameters: {marker;(value)}
	with_vars = 0;
	is_last_was_done = 0;
	goon = 1;
	do 
	{
		unsigned short marker;

		int data_type;

		EXE_Register*	preg;
		unsigned char*	formula;
		unsigned short*	pformula_len;
		
		data_type = CODE_DATATYPE_UNKNOWN;

		preg = 0;
		formula = 0;
		pformula_len = 0;
		
		//marker
		offset += clickplc_ckp_int16_dec(&stream[offset],&marker);

		//printf("Math marker: %04X\n",marker);

		switch(marker)
		{
		case SCR_MATH_MARKER_1_RESULTREG://result reg
			data_type = CODE_DATATYPE_REG;
			preg = &body->result_reg_ref;
			break;

		case SCR_MATH_MARKER_2_IS_HEX://is hex
			body->is_hex = 1;
			break;

		case SCR_MATH_MARKER_3_IS_1SHOT://is 1s
			body->is_one_shot = 1;
			break;

		case SCR_MATH_MARKER_4_FORMULA_A://formula with @
			data_type = CODE_DATATYPE_FORMULA_A;
			formula = body->formula_a;
			pformula_len = &body->formula_a_size;
			break;

		case SCR_MATH_MARKER_5_FORMULA://original formula			
			data_type = CODE_DATATYPE_FORMULA;
			formula = body->formula;
			pformula_len = &body->formula_size;
			break;

		case SCR_MATH_MARKER_6_FORM_NA://formula with #@
			data_type = CODE_DATATYPE_FORMULA_NA;			
			formula = body->formula_na;
			pformula_len = &body->formula_na_size;
			break;

		case SCR_MATH_MARKER_7_FORM_N://formula with #
			data_type = CODE_DATATYPE_FORMULA_N;			
			formula = body->formula_n;
			pformula_len = &body->formula_n_size;
			break;

		case SCR_MATH_MARKER_8_BY_NICK://is by nick names
			body->is_vars_by_nicknames = 1;
			//step over 01:
			offset++;
			break;

		case SCR_MATH_MARKER_9_ADDRLIST_START://address list			
			with_vars = 1;
			break;

		case SCR_MATH_MARKER_10_ADDRLIST_END:
			is_last_was_done = 1;
			break;

		default:

			//printf("Math unknown marker: %04X\n",marker);

			if (is_last_was_done)
			{
				goon = 0;
			}
			else
			{
				if (marker == 0x0000)
				{
					if (!with_vars)
					{
						goon = 0;
					}
				}
			}

			if (goon)
			{
				data_type = CODE_DATATYPE_REG;
				preg = body->vars + body->n_vars++;
			}
		}
		//value (if any)
		if (goon && data_type != CODE_DATATYPE_UNKNOWN)
		{
			int is_pascal;
			int n_bytes;
					
			is_pascal = is_pascal_string(&stream[offset],text,&n_bytes);
						
			if (is_pascal)
			{
				int textsize;
				int dec_data_type;

				textsize = (n_bytes-2)/2;
				offset += n_bytes;

				//parameter
				dec_data_type = clickplc_ckp_math_decode(	text,textsize,
															data_type,
															preg,
															formula,EXE_CODE_INSTRUCTION_MATH_ASCII_MAX,pformula_len);
			}
			else
			{
				printf("Math dec: non-pascal string at marker: %04X\n",marker);
				offset = 0;
				goto __exit_point;
			}
		}

	} while (goon);
	////////////////////////////////////////////////////	
	//$$$
	if (body->is_hex || body->is_one_shot)
	{
		offset += 3;
	}
	else
	{
		offset += 2;
	}

	*pscr_offset = body->info.scr_offset;
__exit_point:
	return(offset);
}

int clickplc_scr_Math_enc(void* obj,unsigned char* stream_before_opname)
{
	SCR_Operation_Math* body = (SCR_Operation_Math*)obj;
	unsigned char* stream = stream_before_opname;
	int offset;
	int toffset;
	int i_marker;
	int n_cycles;
	//////////////////////////////////////////////////////////////////////////
	//opname
	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Math].enc_string,
								SCR_Ops_DataBase[SCR_Op_Math].enc_string_length,
								&stream[0]);
	//opcode
	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Math].code,
									&stream[offset]);
	//////////////////////////////////////////////////////////////////////////
	toffset = offset;//info
	offset += SCR_INFO_ENC_SIZE;
	//////////////////////////////////////////////////////////////////////////
	//parameters: {marker;(value)}
	n_cycles = 9;
	for(i_marker = 0; i_marker < n_cycles; i_marker++)
	{
#define FUNNY_MARKER 0xAAAA
		unsigned short marker = FUNNY_MARKER;
		
		void* prm_ptr0 = 0;
		void* prm_ptr1 = 0;
		int data_type = CODE_DATATYPE_UNKNOWN;
		
		switch(i_marker)
		{
		case 0://result reg
			
			marker = SCR_MATH_MARKER_1_RESULTREG;
			data_type = CODE_DATATYPE_REG;
			prm_ptr0 = (void*)&body->result_reg_ref;
			break;

		case 1://is hex
			if (body->is_hex)
			{
				marker = SCR_MATH_MARKER_2_IS_HEX;
			}
			break;

		case 2://is 1s
			if (body->is_one_shot)
			{
				marker = SCR_MATH_MARKER_3_IS_1SHOT;
			}
			break;

		case 3://formula with @
			marker = SCR_MATH_MARKER_4_FORMULA_A;
			data_type = CODE_DATATYPE_FORMULA_A;
			prm_ptr0 = (void*)&body->formula_a;
			prm_ptr1 = (void*)&body->formula_a_size;
			break;

		case 4://original formula
			marker = SCR_MATH_MARKER_5_FORMULA;
			data_type = CODE_DATATYPE_FORMULA;
			prm_ptr0 = (void*)&body->formula;
			prm_ptr1 = (void*)&body->formula_size;
			break;

		case 5://formula with #@
			marker = SCR_MATH_MARKER_6_FORM_NA;
			data_type = CODE_DATATYPE_FORMULA_NA;
			prm_ptr0 = (void*)&body->formula_na;
			prm_ptr1 = (void*)&body->formula_na_size;
			break;

		case 6://formula with #
			marker = SCR_MATH_MARKER_7_FORM_N;
			data_type = CODE_DATATYPE_FORMULA_N;
			prm_ptr0 = (void*)&body->formula_n;
			prm_ptr1 = (void*)&body->formula_n_size;
			break;

		case 7://is by nick names
			if (body->is_vars_by_nicknames)
			{
				marker = SCR_MATH_MARKER_8_BY_NICK;
			}
			break;

		case 8://address list
			if (body->n_vars > 0)
			{
				n_cycles += body->n_vars + 1;
				marker = SCR_MATH_MARKER_9_ADDRLIST_START;
			}
			break;

		default:

			if(i_marker != (n_cycles-1) )
			{
				int index = i_marker - 9;

				//one of address list items
				marker		= index;
				data_type	= CODE_DATATYPE_REG;
				prm_ptr0	= (void*)&body->vars[index];
			}
			else
			{
				//end of address list
				marker = SCR_MATH_MARKER_10_ADDRLIST_END;
			}
		}

		if (marker != FUNNY_MARKER)
		{
			//marker
			offset += clickplc_ckp_int16_enc(marker,&stream[offset]);

			if (marker == SCR_MATH_MARKER_8_BY_NICK)
			{
				stream[offset + 0] = 0x01;
				offset++;
			}
		}

		if (data_type != CODE_DATATYPE_UNKNOWN)
		{
			//parameter
			offset += clickplc_ckp_math_encode(	&stream[offset],data_type,prm_ptr0,prm_ptr1);
		}
#undef FUNNY_MARKER
	}

	stream[offset + 0] = 0x00;
	stream[offset + 1] = 0x00;
	offset += 2;
	
	//////////////////////////////////////////////////////////////////////////
	body->info.scr_offset += offset;
	clickplc_scr_info_enc(&body->info,&stream[toffset],SCR_Op_Math);

// 	$
// 		01 00 01	hex
// 		01 00 03	hex + 1s
// 		01 00 01	decimal + 1s

	//$$$
	if (body->is_hex || body->is_one_shot)
	{
		stream[offset+0] = 0x01;
		stream[offset+1] = 0x00;
		stream[offset+2] = 0x01 + (((unsigned char)(body->is_hex && body->is_one_shot))<<1);
		offset += 3;
	}
	else
	{
		stream[offset+0] = 0x00;
		stream[offset+1] = 0x00;
		offset += 2;
	}
	return(offset);
}

int clickplc_scr_NOP_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
{
	return 0;
}

int clickplc_scr_NOP_enc(void* obj,unsigned char* stream_before_opname)
{
	return 0;
}

// int clickplc_scr_Compare_dec(unsigned char* stream_after_opcode,void* obj,unsigned short* pscr_offset)
// {
// 	SCR_Operation_Compare* body = (SCR_Operation_Edge*)obj;
// 	unsigned char* stream = stream_after_opcode;
// 	int offset;	
// 	int n_bytes;
// 	char text[SCR_REG_STRING_MAX_SIZE];
// 
// 	////////////////////////////////////////////////////
// 	offset = clickplc_scr_info_dec(stream,&body->info);
// 	////////////////////////////////////////////////////	
// 
// 	////////////////////////////////////////////////////	
// 	//$$$
// 	//00 00
// 	offset += 2;	
// 
// 	*pscr_offset = body->info.scr_offset;
// __exit_point:
// 	return(offset);
// }
// 
// int clickplc_scr_Compare_enc(void* obj,unsigned char* stream_before_opname)
// {
// 	SCR_Operation_Compare* body = (SCR_Operation_Compare*)obj;
// 	unsigned char* stream = stream_before_opname;
// 	int offset;
// 	int toffset;
// 	int n_bytes;
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	//opname
// 	offset = enc_pascal_string(	SCR_Ops_DataBase[SCR_Op_Compare].enc_string,
// 								SCR_Ops_DataBase[SCR_Op_Compare].enc_string_length,
// 								&stream[0]);
// 	//opcode
// 	offset += clickplc_ckp_int16_enc(SCR_Ops_DataBase[SCR_Op_Compare].code,
// 									&stream[offset]);
// 	//////////////////////////////////////////////////////////////////////////
// 	toffset = offset;//info
// 	offset += SCR_INFO_ENC_SIZE;
// 	//////////////////////////////////////////////////////////////////////////
// 
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	body->info.scr_offset += offset;
// 	clickplc_scr_info_enc(&body->info,&stream[toffset]);
// 
// 	//$$$
// 	stream[offset+0] = 0x00;
// 	stream[offset+1] = 0x00;
// 	offset += 2;
// 
// 	return(offset);
// }
