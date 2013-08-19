#ifndef __clickplc_states_h__
#define __clickplc_states_h__

#ifdef __cplusplus
extern "C" {
#endif

enum tagCLICKPLC_States
{
	CLICKPLC_State_Idle = 0,	
	CLICKPLC_State_Hardware_Info,	
	CLICKPLC_State_Firmware_Info,
	CLICKPLC_State_Firmware_Read,
	CLICKPLC_State_Firmware_Write,	
	CLICKPLC_State_Software_Info,
	CLICKPLC_State_EXE_Info,	
	CLICKPLC_State_EXE_Read,
	CLICKPLC_State_EXE_Write,
	CLICKPLC_State_PRM_Info,
	CLICKPLC_State_PRM_Read,
	CLICKPLC_State_PRM_Write,	
	CLICKPLC_State_PRJ_Info,
	CLICKPLC_State_PRJ_Read,
	CLICKPLC_State_PRJ_Write,
	CLICKPLC_State_DAT_Write,
	CLICKPLC_State_Shutdown,
	CLICKPLC_State_EXE_GetReady,
	CLICKPLC_State_PRM_GetReady,
	CLICKPLC_State_PRJ_GetReady,
	CLICKPLC_State_EXE_AreYouReady,
	CLICKPLC_State_PRM_AreYouReady,
	CLICKPLC_State_PRJ_AreYouReady,
	CLICKPLC_State_EndOfSession,
	CLICKPLC_State_GetValue,
	CLICKPLC_State_SetValue

}CLICKPLC_States;

enum tagCLICKPLC_Events
{
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
};

char* get_clickplc_state_name(int st);
char* get_clickplc_event_name(int ev);

#ifdef __cplusplus
}
#endif


#endif __clickplc_states_h__
