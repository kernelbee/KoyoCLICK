/*
**  CRC.H - header file for SNIPPETS CRC and checksum functions
*/

#ifndef CRC__H
#define CRC__H

#include <stdlib.h>           /* For size_t                 */
//#include "sniptype.h"         /* For BYTE, WORD, DWORD      */

#if 0
/*
**  File: ARCCRC16.C
*/

void init_crc_table(void);
WORD crc_calc(WORD crc, char *buf, unsigned nbytes);
void do_file(char *fn);

/*
**  File: CRC-16F.C
*/

WORD updcrc(WORD icrc, BYTE *icp, size_t icnt);


/*
**  File: CHECKSUM.C
*/

unsigned checksum(void *buffer, size_t len, unsigned int seed);

/*
**  File: CHECKEXE.C
*/

void checkexe(char *fname);


#endif //0

/*
**  File: CRC-16.C
*/

unsigned short crc16(char *data_p, unsigned short length);

#define CUSTOM_CKP_POLY 0x8000
unsigned short crc16poly(char *data_p, unsigned short length,unsigned short poly);

/*
**  File: CRC_32.C
*/

#define UPDC32(octet,crc) (crc_32_tab[((crc)\
     ^ ((unsigned char)octet)) & 0xff] ^ ((crc) >> 8))

unsigned int updateCRC32(unsigned char ch, unsigned int crc);
unsigned int crc32buf(char *buf, size_t len);

#endif /* CRC__H */
