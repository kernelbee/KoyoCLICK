#include "clickplc_machine.h"
#include "clickplc_states.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

static int ConvertMessageToEvent[/*by message*/] =
{
	CLICKPLC_Event_GetValue,		//CLICKPLC_Msg_GetParam
	CLICKPLC_Event_SetValue,		//CLICKPLC_Msg_SetParam
	CLICKPLC_Event_HardwareInfo,	//CLICKPLC_Msg_GetHardwareInfo,
	CLICKPLC_Event_SoftwareInfo,	//CLICKPLC_Msg_GetSoftwareInfo,
	CLICKPLC_Event_EXE_Info,		//CLICKPLC_Msg_GetEXE_Info,
	CLICKPLC_Event_PRM_Info,		//CLICKPLC_Msg_GetPRM_Info,
	CLICKPLC_Event_PRJ_Info,		//CLICKPLC_Msg_GetPRJ_Info,
	CLICKPLC_Event_EXE_GetReady,	//CLICKPLC_Msg_GetReadyForEXE_Update,
	CLICKPLC_Event_PRM_GetReady,	//CLICKPLC_Msg_GetReadyForPRM_Update,
	CLICKPLC_Event_PRJ_GetReady,	//CLICKPLC_Msg_GetReadyForPRJ_Update,
	CLICKPLC_Event_EXE_AreYouReady,	//CLICKPLC_Msg_AreYouReadyForEXE_Update,
	CLICKPLC_Event_PRM_AreYouReady,	//CLICKPLC_Msg_AreYouReadyForPRM_Update,
	CLICKPLC_Event_PRJ_AreYouReady,	//CLICKPLC_Msg_AreYouReadyForPRJ_Update,
	CLICKPLC_Event_EXE_Write,		//CLICKPLC_Msg_EXE_Data,
	CLICKPLC_Event_PRM_Write,		//CLICKPLC_Msg_PRM_Data,
	CLICKPLC_Event_PRJ_Write,		//CLICKPLC_Msg_PRJ_Data,
	CLICKPLC_Event_DAT_Write,		//CLICKPLC_Msg_DAT_Data,
	CLICKPLC_Event_EndOfSession,	//CLICKPLC_Msg_Byby,
	CLICKPLC_Event_Unknown			//CLICKPLC_Msg_Unknown
};

static int NextStateOfMachine_Server[/*curr event*/] =
{
/*
	CLICKPLC_Event_TimeOut = 0,
	CLICKPLC_Event_HardwareInfo,
	CLICKPLC_Event_FirmwareInfo,
	CLICKPLC_Event_FirmwareRead,
	CLICKPLC_Event_FirmwareWrite,
	CLICKPLC_Event_SoftwareInfo,
	CLICKPLC_Event_EXE_Info,	
	CLICKPLC_Event_EXE_Read,
	CLICKPLC_Event_EXE_Write,
	CLICKPLC_Event_PRM_Info,
	CLICKPLC_Event_PRM_Read,
	CLICKPLC_Event_PRM_Write,
	CLICKPLC_Event_PRJ_Info,
	CLICKPLC_Event_PRJ_Read,
	CLICKPLC_Event_PRJ_Write,
	CLICKPLC_Event_DAT_Write,
	CLICKPLC_Event_Shutdown,
	CLICKPLC_Event_Error,
	CLICKPLC_Event_FromUpper,
	CLICKPLC_Event_EXE_GetReady,
	CLICKPLC_Event_PRM_GetReady,
	CLICKPLC_Event_PRJ_GetReady,
	CLICKPLC_Event_EXE_AreYouReady,
	CLICKPLC_Event_PRM_AreYouReady,
	CLICKPLC_Event_PRJ_AreYouReady,
	CLICKPLC_Event_EndOfSession,
	CLICKPLC_Event_GetValue,
	CLICKPLC_Event_SetValue,
	CLICKPLC_Event_Unknown
*/	
	
	CLICKPLC_State_Idle,				//CLICKPLC_Event_TimeOut
	CLICKPLC_State_Hardware_Info,		//CLICKPLC_Event_HardwareInfo
	CLICKPLC_State_Firmware_Info,		//CLICKPLC_Event_FirmwareInfo
	CLICKPLC_State_Firmware_Read,		//CLICKPLC_Event_FirmwareRead
	CLICKPLC_State_Firmware_Write,		//CLICKPLC_Event_FirmwareWrite
	CLICKPLC_State_Software_Info,		//CLICKPLC_Event_SoftwareInfo
	CLICKPLC_State_EXE_Info,			//CLICKPLC_Event_EXE_Info
	CLICKPLC_State_EXE_Read,			//CLICKPLC_Event_EXE_Read
	CLICKPLC_State_EXE_Write,			//CLICKPLC_Event_EXE_Write
	CLICKPLC_State_PRM_Info,			//CLICKPLC_Event_PRM_Info
	CLICKPLC_State_PRM_Read,			//CLICKPLC_Event_PRM_Read
	CLICKPLC_State_PRM_Write,			//CLICKPLC_Event_PRM_Write
	CLICKPLC_State_PRJ_Info,			//CLICKPLC_Event_PRJ_Info
	CLICKPLC_State_PRJ_Read,			//CLICKPLC_Event_PRJ_Read
	CLICKPLC_State_PRJ_Write,			//CLICKPLC_Event_PRJ_Write
	CLICKPLC_State_DAT_Write,			//CLICKPLC_Event_DAT_Write
	CLICKPLC_State_Shutdown,			//CLICKPLC_Event_Shutdown
	CLICKPLC_State_Idle,				//CLICKPLC_Event_Error
	CLICKPLC_State_Idle,				//CLICKPLC_Event_FromUpper
	CLICKPLC_State_EXE_GetReady,		//CLICKPLC_Event_EXE_GetReady
	CLICKPLC_State_PRM_GetReady,		//CLICKPLC_Event_PRM_GetReady
	CLICKPLC_State_PRJ_GetReady,		//CLICKPLC_Event_PRJ_GetReady
	CLICKPLC_State_EXE_AreYouReady,		//CLICKPLC_Event_EXE_AreYouReady
	CLICKPLC_State_PRM_AreYouReady,		//CLICKPLC_Event_PRM_AreYouReady
	CLICKPLC_State_PRJ_AreYouReady,		//CLICKPLC_Event_PRJ_AreYouReady
	CLICKPLC_State_EndOfSession,		//CLICKPLC_Event_EndOfSession
	CLICKPLC_State_GetValue,			//CLICKPLC_Event_GetValue
	CLICKPLC_State_SetValue,			//CLICKPLC_Event_SetValue
	CLICKPLC_State_Idle					//CLICKPLC_Event_Unknown
};

static int IsItResponsibleState[] = {
	0,	//CLICKPLC_State_Idle
	1,	//CLICKPLC_State_Hardware_Info,
	1,	//CLICKPLC_State_Firmware_Info,
	1,	//CLICKPLC_State_Firmware_Read,
	1,	//CLICKPLC_State_Firmware_Write,
	1,	//CLICKPLC_State_Software_Info,
	0,	//CLICKPLC_State_EXE_Info,	
	1,	//CLICKPLC_State_EXE_Read,
	1,	//CLICKPLC_State_EXE_Write,
	0,	//CLICKPLC_State_PRM_Info,
	1,	//CLICKPLC_State_PRM_Read,
	1,	//CLICKPLC_State_PRM_Write,
	1,	//CLICKPLC_State_PRJ_Info,
	1,	//CLICKPLC_State_PRJ_Read,
	1,	//CLICKPLC_State_PRJ_Write,
	1,	//CLICKPLC_State_DAT_Write
	1,	//CLICKPLC_State_Shutdown,
	1,	//CLICKPLC_State_EXE_GetReady,
	1,	//CLICKPLC_State_PRM_GetReady,
	1,	//CLICKPLC_State_PRJ_GetReady,
	1,	//CLICKPLC_State_EXE_AreYouReady,
	1,	//CLICKPLC_State_PRM_AreYouReady,
	1,	//CLICKPLC_State_PRJ_AreYouReady,
	1,	//CLICKPLC_State_EndOfSession,
	1,	//CLICKPLC_State_GetValue,
	1	//CLICKPLC_State_SetValue		
};

typedef struct tagClickPLCMachine
{
	/********************************/
	int				cputype;
	void*			staff;
	method_recv		rx_callback;
	int				rx_timeout_ms;
	method_send		tx_callback;
	int				tx_timeout_ms;
	int				tx_retries;
	/********************************/
	
	unsigned char**	membanks;
	int*			membank_sizes;
	
	int*			membank_rx_sizes_exp;	//expected
	int*			membank_rx_sizes_curr;	//current <= expected

	int*			membank_tx_sizes_exp;	//expected
	int*			membank_tx_sizes_curr;	//current <= expected

#define MAX_CLICKPLC_HARDWARE_INFO 256
#define MAX_CLICKPLC_SOFTWARE_INFO 256
#define MAX_CLICKPLC_PROJECT_INFO 256
	unsigned char	hardware_info[MAX_CLICKPLC_HARDWARE_INFO];
	unsigned char	software_info[MAX_CLICKPLC_SOFTWARE_INFO];
	unsigned char	project_info[MAX_CLICKPLC_PROJECT_INFO];

#define CLICKPLC_PROGRAM_STOPPED		0
#define CLICKPLC_PROGRAM_RUNNING		1
	int				run_or_stop;

	int				prev_state;
	int				curr_state;

	unsigned char	stream_i[20*MAX_CLICKPLC_ENC_MESSAGE_LENGTH];
	int				stream_i_len;

	CLICKPLCMessageDataContainer data_i;
	CLICKPLCMessageDataContainer data_o;
	
	int				message_o;
	unsigned char	stream_o[MAX_CLICKPLC_ENC_MESSAGE_LENGTH];
	int				stream_o_len;

	FILE*			fp_log;
	char			log_name[256];

}ClickPLCMachine;

void bg_logging(ClickPLCMachine* plc,char* prefix,char* logrecord);
void do_logging(ClickPLCMachine* plc,char* prefix,char* logrecord);

int get_ready_membank_rx	(ClickPLCMachine* plc,int membank,int rx_size_expected);
int update_membank_rx		(ClickPLCMachine* plc,int membank,int offset,unsigned char* data,int data_size);

void update_software_info(ClickPLCMachine* plc,int membank);

void clickplc_machine_reset_to_factory(ClickPLCMachine* machine);

ClickPLCMachine* create_clickplc_machine	(int cputype,int* perrcode);
void destroy_clickplc_machine				(ClickPLCMachine** pmachine);
int clickplc_get_event						(ClickPLCMachine* plc);


typedef int (*method_clickplc_machine_state)(ClickPLCMachine* machine);

int handler_CLICKPLC_State_Idle(ClickPLCMachine* plc);
int handler_CLICKPLC_State_Hardware_Info(ClickPLCMachine* plc);

int handler_CLICKPLC_State_Firmware_Info(ClickPLCMachine* plc);
int handler_CLICKPLC_State_Firmware_Read(ClickPLCMachine* plc);
int handler_CLICKPLC_State_Firmware_Write(ClickPLCMachine* plc);

int handler_CLICKPLC_State_Software_Info(ClickPLCMachine* plc);

int handler_CLICKPLC_State_EXE_Info(ClickPLCMachine* plc);
int handler_CLICKPLC_State_EXE_Read(ClickPLCMachine* plc);
int handler_CLICKPLC_State_EXE_Write(ClickPLCMachine* plc);

int handler_CLICKPLC_State_PRM_Info(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRM_Read(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRM_Write(ClickPLCMachine* plc);

int handler_CLICKPLC_State_PRJ_Info(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRJ_Read(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRJ_Write(ClickPLCMachine* plc);

int handler_CLICKPLC_State_DAT_Write(ClickPLCMachine* plc);

int handler_CLICKPLC_State_Shutdown(ClickPLCMachine* plc);

int handler_CLICKPLC_State_EXE_GetReady(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRM_GetReady(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRJ_GetReady(ClickPLCMachine* plc);
int handler_CLICKPLC_State_EXE_AreYouReady(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRM_AreYouReady(ClickPLCMachine* plc);
int handler_CLICKPLC_State_PRJ_AreYouReady(ClickPLCMachine* plc);
int handler_CLICKPLC_State_EndOfSession(ClickPLCMachine* plc);
int handler_CLICKPLC_State_GetValue(ClickPLCMachine* plc);
int handler_CLICKPLC_State_SetValue(ClickPLCMachine* plc);

static method_clickplc_machine_state MachineStateHandlers[] = 
{
	handler_CLICKPLC_State_Idle,
	handler_CLICKPLC_State_Hardware_Info,
	handler_CLICKPLC_State_Firmware_Info,
	handler_CLICKPLC_State_Firmware_Read,
	handler_CLICKPLC_State_Firmware_Write,
	handler_CLICKPLC_State_Software_Info,
	handler_CLICKPLC_State_EXE_Info,	
	handler_CLICKPLC_State_EXE_Read,
	handler_CLICKPLC_State_EXE_Write,
	handler_CLICKPLC_State_PRM_Info,
	handler_CLICKPLC_State_PRM_Read,
	handler_CLICKPLC_State_PRM_Write,
	handler_CLICKPLC_State_PRJ_Info,
	handler_CLICKPLC_State_PRJ_Read,
	handler_CLICKPLC_State_PRJ_Write,
	handler_CLICKPLC_State_DAT_Write,
	handler_CLICKPLC_State_Shutdown,
	handler_CLICKPLC_State_EXE_GetReady,
	handler_CLICKPLC_State_PRM_GetReady,
	handler_CLICKPLC_State_PRJ_GetReady,
	handler_CLICKPLC_State_EXE_AreYouReady,
	handler_CLICKPLC_State_PRM_AreYouReady,
	handler_CLICKPLC_State_PRJ_AreYouReady,
	handler_CLICKPLC_State_EndOfSession,
	handler_CLICKPLC_State_GetValue,
	handler_CLICKPLC_State_SetValue
};

int clickplc_machine	(	int				cputype,
							void*			staff,
							method_recv		rx_callback,
							int				rx_timeout_ms,
							method_send		tx_callback,
							int				tx_timeout_ms,
							int				tx_retries
							)
{
	int retcode = CLICKPLC_MACHINE_ERR_OK;//OK
	ClickPLCMachine* plc = 0;
	
	//allocate resources
	plc = create_clickplc_machine(cputype,&retcode);
	if (plc)
	{
		//save parameters
		plc->cputype		= cputype;
		
		plc->staff			= staff;

		plc->rx_callback	= rx_callback;
		plc->rx_timeout_ms	= rx_timeout_ms;

		plc->tx_callback	= tx_callback;
		plc->tx_timeout_ms	= tx_timeout_ms;
		plc->tx_retries		= tx_retries;

		plc->run_or_stop	= CLICKPLC_PROGRAM_STOPPED;

		//init FSM
		plc->prev_state = CLICKPLC_State_Shutdown;
		plc->curr_state = CLICKPLC_State_Idle;

		plc->stream_i_len	= 0;

		plc->stream_o_len	= 0;

		//setup descriptors of hardware, software...
		clickplc_machine_reset_to_factory(plc);
		
		//run FSM
		do 
		{
			int errcode;
			int curr_event;

			if (plc->prev_state != plc->curr_state)
			{
				printf("State: %s\n",get_clickplc_state_name(plc->curr_state));
				do_logging(plc,"State: ",get_clickplc_state_name(plc->curr_state));
			}

			errcode = MachineStateHandlers[plc->curr_state](plc);
			if(IsItResponsibleState[plc->curr_state])
			{
				int lwritedone;
				
				plc->stream_o_len = make_clickplc_response(plc->message_o,&plc->data_o,plc->stream_o);
				
				if (plc->stream_o_len)
				{
					if(plc->tx_callback(plc->staff,plc->stream_o,plc->stream_o_len,&lwritedone,plc->tx_timeout_ms))
					{
						//can't send message
						curr_event = CLICKPLC_Event_Error;
					}
				}
			}			

			if (errcode==CLICKPLC_MACHINE_ERR_OK)
			{
				curr_event = clickplc_get_event(plc);
			}
			else
			{				
				curr_event = CLICKPLC_Event_Error;
			}

			printf("\tEvent: %s\n",get_clickplc_event_name(curr_event));
			do_logging(plc,"\tEvent: ",get_clickplc_event_name(curr_event));

			plc->prev_state = plc->curr_state;
			plc->curr_state = NextStateOfMachine_Server[curr_event];
			
		} while (plc->curr_state != CLICKPLC_State_Shutdown);

		//free resources
		destroy_clickplc_machine(&plc);
	}

	return(retcode);
}

ClickPLCMachine* create_clickplc_machine	(int cputype,int* perrcode)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;//OK
	ClickPLCMachine* machine = 0;
	
	if (cputype < CLICKPLCDevice_DefaultCPU || cputype >= CLICKPLCDevice_Unknown)
	{
		errcode = CLICKPLC_MACHINE_ERR_CPUTYPE;
		goto __exit_point;
	}

	machine = (ClickPLCMachine*)malloc(sizeof(ClickPLCMachine));
	if (machine)
	{
		memset(machine,0x00,sizeof(ClickPLCMachine));
		
		machine->membank_sizes		= (int*)malloc(PLCMemBank_ALL*sizeof(int));
		machine->membank_rx_sizes_exp	= (int*)malloc(PLCMemBank_ALL*sizeof(int));
		machine->membank_rx_sizes_curr	= (int*)malloc(PLCMemBank_ALL*sizeof(int));
		machine->membank_tx_sizes_exp	= (int*)malloc(PLCMemBank_ALL*sizeof(int));
		machine->membank_tx_sizes_curr	= (int*)malloc(PLCMemBank_ALL*sizeof(int));
		machine->membanks			= (unsigned char**)malloc(PLCMemBank_ALL*sizeof(unsigned char*));

		switch (cputype)
		{		
		case CLICKPLCDevice_C001DD1D:
		case CLICKPLCDevice_C001DD2D:
		case CLICKPLCDevice_C002DD1D:
		case CLICKPLCDevice_C002DD2D:		
		case CLICKPLCDevice_DefaultCPU:
			{
				int i_bank;

				for(i_bank = PLCMemBank_EXE; i_bank < PLCMemBank_ALL; i_bank++)
				{
					int bank_size = 0;
					switch (i_bank)
					{
					case PLCMemBank_EXE:
						bank_size	= MEMBANK_C001DD1D_EXE_SIZE;
						break;
					case PLCMemBank_PRM:
						bank_size	= MEMBANK_C001DD1D_PRM_SIZE;
						break;
					case PLCMemBank_PRJ:
						bank_size	= MEMBANK_C001DD1D_PRJ_SIZE;
						break;
					case PLCMemBank_DAT:
						bank_size	= MEMBANK_C001DD1D_DAT_SIZE;
						break;
					}					
					machine->membank_sizes[i_bank]  = bank_size;
					if (bank_size)
					{
						machine->membanks[i_bank] = (unsigned char*)malloc(bank_size*sizeof(unsigned char));
						memset(machine->membanks[i_bank],0x00,bank_size*sizeof(unsigned char));
					}					
					else
					{
						machine->membanks[i_bank] = 0;
					}
					machine->membank_rx_sizes_exp[i_bank]  = 0;
					machine->membank_rx_sizes_curr[i_bank]  = 0;
					machine->membank_tx_sizes_exp[i_bank]  = 0;
					machine->membank_tx_sizes_curr[i_bank]  = 0;
				}
			}
			break;
		}
		bg_logging(machine,"START","PLC Instance created.");
	}
	else
	{
		errcode = CLICKPLC_MACHINE_ERR_MEMORY;
	}
__exit_point:
	*perrcode = errcode;
	return(machine);
}

void bg_logging(ClickPLCMachine* plc,char* prefix,char* logrecord)
{
	sprintf(plc->log_name,"commlog.txt");

	plc->fp_log = fopen(plc->log_name,"wb+");
	fprintf(plc->fp_log,"%s%s\n",prefix,logrecord);
	fflush(plc->fp_log);
	fclose(plc->fp_log);

	return;
}

void do_logging(ClickPLCMachine* plc,char* prefix,char* logrecord)
{
	sprintf(plc->log_name,"commlog.txt");

	plc->fp_log = fopen(plc->log_name,"a+");
	fprintf(plc->fp_log,"%s%s\n",prefix,logrecord);
	fflush(plc->fp_log);
	fclose(plc->fp_log);

	return;
}

void destroy_clickplc_machine				(ClickPLCMachine** pmachine)
{
	ClickPLCMachine* machine = *pmachine;

	if (machine)
	{		
		int i_bank;

		for(i_bank = PLCMemBank_EXE; i_bank < PLCMemBank_ALL; i_bank++)
		{			
			if(machine->membank_sizes)
			{
				free(machine->membank_sizes);
			}

			if(machine->membank_rx_sizes_exp)
			{
				free(machine->membank_rx_sizes_exp);
			}
			if(machine->membank_rx_sizes_curr)
			{
				free(machine->membank_rx_sizes_curr);
			}

			if(machine->membank_tx_sizes_exp)
			{
				free(machine->membank_tx_sizes_exp);
			}
			if(machine->membank_tx_sizes_curr)
			{
				free(machine->membank_tx_sizes_curr);
			}

			if (machine->membanks)
			{
				if (machine->membanks[i_bank])
				{
					free(machine->membanks[i_bank]);
				}
				free(machine->membanks);
			}
		}

		do_logging(machine,"BYBY","PLC Instance Destroyed.");

		free(machine);
		*pmachine = 0;
	}

	return;
}

int handler_CLICKPLC_State_Idle(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;
	//do nothing. just wait.
	return(errcode);
}
int handler_CLICKPLC_State_Hardware_Info(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;
		
	plc->data_o.requested_data_size		= plc->data_i.requested_data_size;
	plc->data_o.requested_data_offset	= plc->data_i.requested_data_offset;
	plc->data_o.requested_data			= plc->hardware_info + plc->data_o.requested_data_offset;

	plc->message_o = CLICKPLC_Msg_GetHardwareInfo;

	return(errcode);
}
int handler_CLICKPLC_State_Firmware_Info(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
handler_CLICKPLC_State_Firmware_Read(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
handler_CLICKPLC_State_Firmware_Write(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
handler_CLICKPLC_State_Software_Info(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	plc->data_o.requested_data_size		= 0x28;
	plc->data_o.requested_data_offset	= 0x00;
	plc->data_o.requested_data			= plc->software_info + plc->data_o.requested_data_offset;

	plc->message_o = CLICKPLC_Msg_GetSoftwareInfo;

	return(errcode);
}
handler_CLICKPLC_State_EXE_Info(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
handler_CLICKPLC_State_EXE_Read(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}

int update_membank_rx(ClickPLCMachine* plc,int membank,int offset,unsigned char* data,int data_size)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	if (plc->membank_rx_sizes_exp[membank]==0)
	{
		plc->membank_rx_sizes_exp[membank]	= plc->membank_sizes[membank];
		plc->membank_rx_sizes_curr[membank] = 0;
	}

	if (offset + data_size > plc->membank_rx_sizes_exp[membank])
	{
		errcode = CLICKPLC_MACHINE_ERR_MEMORY;
	}
	else
	{
		memcpy(	plc->membanks[membank]+offset,data,data_size);
		plc->membank_rx_sizes_curr[membank]+= data_size;

		//update software descriptor
		if (plc->membank_rx_sizes_curr[membank] >= 12)
		{			
			update_software_info(plc,membank);
		}

		//dump data to file
		{
			FILE *fp_o;
			char fname[256];
			char *banks[] = {"EXE","PRM","PRJ","DAT"};

			sprintf(fname,"membank_%s.dat",banks[membank]);

			fp_o = fopen(fname,"wb+");
			fwrite(plc->membanks[membank],1,plc->membank_rx_sizes_curr[membank],fp_o);
			fflush(fp_o);
			fclose(fp_o);
		}
	}

	return(errcode);
}

int handler_CLICKPLC_State_EXE_Write(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	errcode = update_membank_rx(plc,
								PLCMemBank_EXE,
								plc->data_i.requested_data_offset,
								plc->data_i.requested_data,
								plc->data_i.requested_data_size
								);

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;

	plc->message_o = CLICKPLC_Msg_EXE_Data;

	return(errcode);
}
int handler_CLICKPLC_State_PRM_Info(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
int handler_CLICKPLC_State_PRM_Read(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
int handler_CLICKPLC_State_PRM_Write(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	errcode = update_membank_rx(plc,
								PLCMemBank_PRM,
								plc->data_i.requested_data_offset,
								plc->data_i.requested_data,
								plc->data_i.requested_data_size
								);

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;

	plc->message_o = CLICKPLC_Msg_PRM_Data;

	return(errcode);
}
int handler_CLICKPLC_State_PRJ_Info(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	plc->data_o.requested_data_size		= plc->data_i.requested_data_size;
	plc->data_o.requested_data_offset	= plc->data_i.requested_data_offset;
	plc->data_o.requested_data			= plc->project_info + plc->data_o.requested_data_offset;

	plc->message_o = CLICKPLC_Msg_GetPRJ_Info;

	return(errcode);
}
int handler_CLICKPLC_State_PRJ_Read(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
int handler_CLICKPLC_State_PRJ_Write(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	errcode = update_membank_rx(plc,
								PLCMemBank_PRJ,
								plc->data_i.requested_data_offset,
								plc->data_i.requested_data,
								plc->data_i.requested_data_size
								);
	
	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;

	plc->message_o = CLICKPLC_Msg_PRJ_Data;
	
	return(errcode);
}
int handler_CLICKPLC_State_DAT_Write(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	errcode = update_membank_rx(plc,
								PLCMemBank_DAT,
								plc->data_i.requested_data_offset,
								plc->data_i.requested_data,
								plc->data_i.requested_data_size
								);

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;

	plc->message_o = CLICKPLC_Msg_DAT_Data;

	return(errcode);
}

int handler_CLICKPLC_State_Shutdown(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}

int get_ready_membank_rx(ClickPLCMachine* plc,int membank,int rx_size_expected)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	if (rx_size_expected > plc->membank_sizes[membank])
	{
		free(plc->membanks[membank]);
		plc->membanks[membank]		= (unsigned char*)malloc(rx_size_expected);
		memset(plc->membanks[membank],0x00,rx_size_expected);
		plc->membank_sizes[membank]	= rx_size_expected;
	}

	plc->membank_rx_sizes_exp[membank]	= rx_size_expected;
	plc->membank_rx_sizes_curr[membank] = 0;
	
	return(errcode);
}

int handler_CLICKPLC_State_EXE_GetReady(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	errcode = get_ready_membank_rx(plc,PLCMemBank_EXE,plc->data_i.requested_data_size);

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;
	plc->data_o.requested_data_bank		= PLCMemBank_EXE;

	plc->message_o = CLICKPLC_Msg_GetReadyForEXE_Update;
	
	return(errcode);
}

int handler_CLICKPLC_State_PRM_GetReady(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;
	
	errcode = get_ready_membank_rx(plc,PLCMemBank_PRM,plc->data_i.requested_data_size);

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;
	plc->data_o.requested_data_bank		= PLCMemBank_PRM;

	plc->message_o = CLICKPLC_Msg_GetReadyForPRM_Update;

	return(errcode);
}
int handler_CLICKPLC_State_PRJ_GetReady(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	errcode = get_ready_membank_rx(plc,PLCMemBank_PRJ,plc->data_i.requested_data_size);

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;
	plc->data_o.requested_data_bank		= PLCMemBank_PRJ;

	plc->message_o = CLICKPLC_Msg_GetReadyForPRJ_Update;
	
	return(errcode);
}
int handler_CLICKPLC_State_EXE_AreYouReady(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;
	plc->data_o.requested_data_bank		= PLCMemBank_EXE;
	plc->data_o.response_code			= CLICKPLC_RESPONSE_CODE_YES;

	plc->message_o = CLICKPLC_Msg_AreYouReadyForEXE_Update;
	
	return(errcode);
}
int handler_CLICKPLC_State_PRM_AreYouReady(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;
	plc->data_o.requested_data_bank		= PLCMemBank_PRM;
	plc->data_o.response_code			= CLICKPLC_RESPONSE_CODE_YES;

	plc->message_o = CLICKPLC_Msg_AreYouReadyForPRM_Update;

	return(errcode);
}
int handler_CLICKPLC_State_PRJ_AreYouReady(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;
	plc->data_o.requested_data_bank		= PLCMemBank_PRJ;
	plc->data_o.response_code			= CLICKPLC_RESPONSE_CODE_YES;

	plc->message_o = CLICKPLC_Msg_AreYouReadyForPRJ_Update;

	return(errcode);
}
int handler_CLICKPLC_State_EndOfSession(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	plc->data_o.requested_data_size		= 0;
	plc->data_o.requested_data_offset	= 0;
	plc->data_o.requested_data			= 0;

	plc->message_o = CLICKPLC_Msg_Byby;

	return(errcode);
}
int handler_CLICKPLC_State_GetValue(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}
int handler_CLICKPLC_State_SetValue(ClickPLCMachine* plc)
{
	int errcode = CLICKPLC_MACHINE_ERR_OK;

	return(errcode);
}


void clickplc_machine_reset_to_factory(ClickPLCMachine* machine)
{
	unsigned char* ptr;
	
	//////////////////////////////////////////////////////////////////////////
	ptr = machine->hardware_info;
	//e9 00 00 00 00 00 00 00 04 d9 48 07 c7 65 40 00 83 00 00 00 00 00 00 00

	//e9 00 00 00 00 00 00 00 36 91 44 06 d3 65 40 00 83 00 00 00 00 00 00 00

	ptr[0] = 0xe9;
	ptr[1] = 0x0;
	ptr[2] = 0x0;
	ptr[3] = 0x0;

	ptr[4] = 0x0;
	ptr[5] = 0x0;
	ptr[6] = 0x0;
	ptr[7] = 0x0;

	ptr[8]  = 0x36;
	ptr[9]  = 0x91;
	ptr[10] = 0x44;
	ptr[11] = 0x06;

	ptr[12] = 0xd3;
	ptr[13] = 0x65;
	ptr[14] = 0x40;
	ptr[15] = 0x0;

	ptr[16] = 0x83;
	ptr[17] = 0x0;
	ptr[18] = 0x0;
	ptr[19] = 0x0;

	ptr[20] = 0x0;
	ptr[21] = 0x0;
	ptr[22] = 0x0;
	ptr[23] = 0x0;

	//////////////////////////////////////////////////////////////////////////
	ptr = machine->software_info;
	/*
	e9 00 00 00 
	01 1f 02 00 
	ff ff ff ff 
	00 00 00 00 
	ff ff ff ff 
	00 00 00 00 
	ff ff ff ff	
	00 00 00 00 
	ff ff ff ff 
	00 00 00 00
	*/

	ptr[0] = 0xe9;	ptr[1] = 0x00;	ptr[2] = 0x00;	ptr[3] = 0x00;	
	ptr[4] = 0x01;	ptr[5] = 0x1f;	ptr[6] = 0x02;	ptr[7] = 0x00;	
	ptr[8] = 0xff;	ptr[9] = 0xff;	ptr[10]= 0xff;	ptr[11]= 0xff;
	
	ptr[12] = 0x00;	ptr[13] = 0x00;
	//sizeof(EXE) - 0x20
	ptr[14] = 0x00;	//hi
	ptr[15] = 0x00;	//lo
	
	ptr[16] = 0xff;	ptr[17] = 0xff;	ptr[18] = 0xff;	ptr[19] = 0xff;
	ptr[20] = 0x00;	ptr[21] = 0x00;	ptr[22] = 0x00;	ptr[23] = 0x00;	
	ptr[24] = 0xff;	ptr[25] = 0xff;	ptr[26] = 0xff;	ptr[27] = 0xff;
		
	ptr[28] = 0x00;	ptr[29] = 0x00;
	//sizeof(PRM) - 0x100:
	ptr[30] = 0x00;	//hi
	ptr[31] = 0x00;	//lo	

	ptr[32] = 0xff; ptr[33] = 0xff;	ptr[34] = 0xff;	ptr[35] = 0xff;

	ptr[36] = 0x00;	ptr[37] = 0x00;
	//sizeof(PRJ):
	ptr[38] = 0x00;	//hi
	ptr[39] = 0x00;	//lo

	//////////////////////////////////////////////////////////////////////////
	ptr = machine->project_info;
	/*
	ff ff ff ff ff ff ff ff ff ff ff ff 				ÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 	ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ
	ff ff ff ff			                               	ÿÿÿÿ
	*/
	memset(ptr,0xff,16*15);
	
	return;
}

	/*
	e9 00 00 00 
	01 1f 02 00 
	
	6c 6b 72 d9 header EXE[4,5,6,7]
	00 00 07 6c sizeof(EXE)
	
	aa 95 b7 75 header DAT[4,5,6,7]
	00 00 5f e0	sizeof(DAT)
	
	a8 96 de 88 header PRM[4,5,6,7]
	00 00 14 d8	sizeof(PRM)

	34 76 c6 13 header PRJ[4,5,6,7]
	00 00 22 80	sizeof(PRJ)
	*/

void update_software_info(ClickPLCMachine* plc,int membank)
{
	unsigned char *dst,*src;
	unsigned int size;
	
	dst = plc->software_info;

	dst[0] = 0xe9;	dst[1] = 0x00;	dst[2] = 0x00;	dst[3] = 0x00;
	dst[4] = 0x01;	dst[5] = 0x1f;	dst[6] = 0x02;	dst[7] = 0x00;
	
	src = plc->membanks[membank] + 4;

	switch(membank)
	{
	case PLCMemBank_EXE:
		dst = plc->software_info + 8;
		break;
	case PLCMemBank_PRM:
		dst = plc->software_info + 24;
		break;
	case PLCMemBank_PRJ:
		dst = plc->software_info + 32;
		break;
	case PLCMemBank_DAT:
		dst = plc->software_info + 16;
		break;
	}

	memcpy(dst,src,8);

	return;
}

int clickplc_get_event(ClickPLCMachine* plc)
{
	int new_event = CLICKPLC_Event_TimeOut;
	int goon;

	goon = 1;
	do
	{
		int stream_status;
		int	stream_done_len;
		int message_in;
				
		//printoutbuf("NOW",plc->stream,plc->stream_len);

		/* 2. port events control: try to find message in received byte-stream from port */
		stream_status = look_clickplc_request(	plc->stream_i,
												plc->stream_i_len,
												&message_in,
												&plc->data_i,
												&stream_done_len);

		//printf("stream_status = %d\n",stream_status);
		//printf("stream_done_len = %d\n",stream_done_len);

		switch(stream_status)
		{
		case STR_STATUS_WRONG_PARAMS:	//invalid params			
			goon = 0;
			break;

		case STR_STATUS_NOT_VALID_MSG:	//invalid message in stream;

			printoutbuf("NOT_VALID_MSG: ",plc->stream_i,plc->stream_i_len);

			plc->stream_i_len -= stream_done_len;
			if (plc->stream_i_len) memmove(plc->stream_i,plc->stream_i+stream_done_len,plc->stream_i_len);

		case STR_STATUS_NO_MSG:			//empty stream (no message);
		case STR_STATUS_NOT_WHOLE_MSG:	//not whole message in stream;
			{
				int temp;
				int ret_read = plc->rx_callback(plc->staff,plc->stream_i+plc->stream_i_len,&temp,plc->rx_timeout_ms,0);

				//printf("stream_len:= %d\n",plc->stream_len);

				switch(ret_read)
				{
				case RX_RESULT_OK:		/* OK */
					plc->stream_i_len += temp;
					//printf("< %d bytes\n",temp);
					//printoutbuf("NEW",stream,stream_len);
					break;						

				case RX_RESULT_PORTERROR:	/* Error */
					//OutputDebugString(_T("MCA: port Error"));
					printf("port Error\n");
					
					new_event = CLICKPLC_Event_Error;
					goon = 0;
					break;

				case RX_RESULT_MSGFROMUPPER:	/* Message from Upper Layer */
					/* 3. Upper Layer's events control */
					new_event = CLICKPLC_Event_FromUpper;
					goon = 0;
					break;

				case RX_RESULT_TIMEOUT:	/* TimeOut */					
					printf("Time-Out %d ms is over\n",plc->rx_timeout_ms);
					goon = 0;
					break;

				default:	/* Error */
					//OutputDebugString(_T("MCA: port unknown Error"));
					printf("port unknown Error\n");
					
					new_event = CLICKPLC_Event_Error;
					goon = 0;
				}
			}
			break;

		case STR_STATUS_VALID_MSG:		//valid message in stream					
			//{						
				//printoutbufhex("Rx",stream,stream_done_len);
				
			//}

			new_event = ConvertMessageToEvent[message_in];			
			printf("message [%d] -> event [%d %s]\n",message_in,new_event,get_clickplc_event_name(new_event));
			
			goon = 0;

			/* clean-up stream from done message */
			plc->stream_i_len -= stream_done_len;
			if (plc->stream_i_len) memmove(plc->stream_i,plc->stream_i+stream_done_len,plc->stream_i_len);
			break;
		}
	}while(goon);
	
	return(new_event);
}

