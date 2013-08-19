#include "crc8.h"

unsigned char crc8block( unsigned char crc, unsigned char *data, int len )
{
	while ( len-- > 0 )
	{
		crc += *data++;		
	};
	
    return crc;
}
