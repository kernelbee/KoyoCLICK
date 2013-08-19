#ifndef __modbus_rtu_crc16_h__
#define __modbus_rtu_crc16_h__

#ifdef __cplusplus
extern "C" {
#endif
	
	/* return: CRC16 code of stream */
	unsigned int modbus_rtu_crc16(unsigned char* encoded_message,int length);
	
#ifdef __cplusplus
}
#endif


#endif /*__modbus_rtu_crc16_h__*/
