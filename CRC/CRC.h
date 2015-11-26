/*
* Calculate CRC-32.
* Ref : https://en.wikipedia.org/wiki/Cyclic_redundancy_check
* 
* Author : Mingfei Gao
* 
* Modified:
* 2015-06-22 : File created.
*/

#ifndef CRC_H
#define CRC_H

#ifdef __cplusplus  
extern "C" {
#endif  

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/* CRC generating polynomial */
#define CRC_POLY 0x04C11DB7

/*
 * Initialize value to initiate CRC calculation.
 * At start up, the program sets CRC to the CRC_INITVAL XOR with the Input_Data.
 */
#define CRC_INITVAL 0xFFFFFFFF

/*
* Calculate CRC
*
* Parameter :
* pBuffer : pointer to the input data buffer
* BufferLength : buffer size, in byte.
*
* Return :
* CRC value.
*/
uint32_t CRC_Calculate(const uint8_t *pBuffer, uint16_t BufferLength);

#ifdef __cplusplus  
}
#endif

#endif