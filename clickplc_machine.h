#ifndef __clickplc_machine_h__
#define __clickplc_machine_h__

#include "protocol_clickplc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RX_RESULT_OK			0
#define RX_RESULT_PORTERROR		-1
#define RX_RESULT_MSGFROMUPPER	-2
#define RX_RESULT_TIMEOUT		-3
#define RX_RESULT_UNKNOWNERR	-4

typedef int (*method_recv)(	void* stuff,unsigned char* buffer,int* psize,int timeout,void* exit_event);
typedef int (*method_send)(	void* stuff,unsigned char* data,int size,int *pwrite,int timeout);

#define CLICKPLC_MACHINE_ERR_OK			0
#define CLICKPLC_MACHINE_ERR_MEMORY		1
#define CLICKPLC_MACHINE_ERR_CPUTYPE	2

int clickplc_machine(		int				cputype,
							void*			staff,
							method_recv		rx_callback,
							int				rx_timeout_ms,						
							method_send		tx_callback,
							int				tx_timeout_ms,
							int				tx_retries
							);

#ifdef __cplusplus
}
#endif



#endif /* __clickplc_machine_h__ */
