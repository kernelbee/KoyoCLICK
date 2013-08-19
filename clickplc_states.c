#include "clickplc_states.h"

char* get_clickplc_state_name(int st)
{
	char* name = "Unknown";

	switch (st)
	{
	case CLICKPLC_State_Idle:				name = "Idle"; break;
	case CLICKPLC_State_Hardware_Info:		name = "HW Info"; break;
	case CLICKPLC_State_Firmware_Info:		name = "FW Info"; break;
	case CLICKPLC_State_Firmware_Read:		name = "FW Read"; break;
	case CLICKPLC_State_Firmware_Write:		name = "FW Write"; break;
	case CLICKPLC_State_Software_Info:		name = "SW Info"; break;
	case CLICKPLC_State_EXE_Info:			name = "EXE Info"; break;
	case CLICKPLC_State_EXE_Read:			name = "EXE Read"; break;
	case CLICKPLC_State_EXE_Write:			name = "EXE Write"; break;
	case CLICKPLC_State_PRM_Info:			name = "PRM Info"; break;
	case CLICKPLC_State_PRM_Read:			name = "PRM Read"; break;
	case CLICKPLC_State_PRM_Write:			name = "PRM Write"; break;
	case CLICKPLC_State_PRJ_Info:			name = "PRJ Info"; break;
	case CLICKPLC_State_PRJ_Read:			name = "PRJ Read"; break;
	case CLICKPLC_State_PRJ_Write:			name = "PRJ Write"; break;
	case CLICKPLC_Event_DAT_Write:			name = "DAT Write"; break;
	case CLICKPLC_State_Shutdown:			name = "Shutdown"; break;
	case CLICKPLC_State_EXE_GetReady:		name = "EXE GetReady"; break;
	case CLICKPLC_State_PRM_GetReady:		name = "PRM GetReady"; break;
	case CLICKPLC_State_PRJ_GetReady:		name = "PRJ GetReady"; break;
	case CLICKPLC_State_EXE_AreYouReady:	name = "EXE AreYouReady"; break;
	case CLICKPLC_State_PRM_AreYouReady:	name = "PRM AreYouReady"; break;
	case CLICKPLC_State_PRJ_AreYouReady:	name = "PRJ AreYouReady"; break;
	case CLICKPLC_State_EndOfSession:		name = "EndOfSession"; break;
	case CLICKPLC_State_GetValue:			name = "GetValue"; break;
	case CLICKPLC_State_SetValue:			name = "SetValue"; break;
	}

	return(name);
}
char* get_clickplc_event_name(int ev)
{
	char* name = "Unknown";

	switch(ev)
	{
	case CLICKPLC_Event_TimeOut:	name = "TimeOut"; break;
	case CLICKPLC_Event_HardwareInfo:	name = "HW Info"; break;
	case CLICKPLC_Event_FirmwareInfo:	name = "FW Info"; break;
	case CLICKPLC_Event_FirmwareRead:	name = "FW Read"; break;
	case CLICKPLC_Event_FirmwareWrite:	name = "FW Write"; break;
	case CLICKPLC_Event_SoftwareInfo:	name = "SW Info"; break;
	case CLICKPLC_Event_EXE_Info:	name = "EXE Info"; break;
	case CLICKPLC_Event_EXE_Read:	name = "EXE Read"; break;
	case CLICKPLC_Event_EXE_Write:	name = "EXE Write"; break;
	case CLICKPLC_Event_PRM_Info:	name = "PRM Info"; break;
	case CLICKPLC_Event_PRM_Read:	name = "PRM Read"; break;
	case CLICKPLC_Event_PRM_Write:	name = "PRM Write"; break;
	case CLICKPLC_Event_PRJ_Info:	name = "PRJ Info"; break;
	case CLICKPLC_Event_PRJ_Read:	name = "PRJ Read"; break;
	case CLICKPLC_Event_PRJ_Write:	name = "PRJ Write"; break;
	case CLICKPLC_Event_DAT_Write:	name = "DAT Write"; break;
	case CLICKPLC_Event_Shutdown:	name = "Shutdown"; break;
	case CLICKPLC_Event_Error:	name = "Error"; break;
	case CLICKPLC_Event_FromUpper:	name = "FromUpper"; break;
	case CLICKPLC_Event_EXE_GetReady:	name = "EXE GetReady"; break;
	case CLICKPLC_Event_PRM_GetReady:	name = "PRM GetReady"; break;
	case CLICKPLC_Event_PRJ_GetReady:	name = "PRJ GetReady"; break;
	case CLICKPLC_Event_EXE_AreYouReady:	name = "EXE AreYouReady"; break;
	case CLICKPLC_Event_PRM_AreYouReady:	name = "PRM AreYouReady"; break;
	case CLICKPLC_Event_PRJ_AreYouReady:	name = "PRJ AreYouReady"; break;
	case CLICKPLC_Event_EndOfSession:	name = "EndOfSession"; break;
	case CLICKPLC_Event_GetValue:	name = "GetValue"; break;
	case CLICKPLC_Event_SetValue:	name = "SetValue"; break;
	case CLICKPLC_Event_Unknown:	name = "UnknownEv"; break;
	}

	return(name);
}
