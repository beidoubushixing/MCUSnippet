/*
* Calculate CRC-32.
* Ref : https://en.wikipedia.org/wiki/Cyclic_redundancy_check
*
* Author : Mingfei Gao
*
* Modified:
* 2015-06-22 : File created.
*/

#include "CRC.h"

/* Internal variable
 * Current CRC value, for iteration
 */
static uint32_t Current_CRC;

/*
 * Internal function.
 * Calculate CRC value for each 32-bit.
 */
static void CRC32(uint32_t Input_Data)
{
  uint8_t bindex;

  Current_CRC ^= Input_Data;

  for (bindex = 0; bindex < 32; bindex++)
  {
    if (Current_CRC & 0x80000000)
      Current_CRC = (Current_CRC << 1) ^ CRC_POLY;
    else
      Current_CRC <<= 1;
  }
}

/*
* Internal function.
* Calculate CRC value for 16-bit.
*/
static void CRC16(uint16_t Input_Data)
{
  uint8_t bindex;

  Current_CRC ^= (Input_Data << 16);

  for (bindex = 0; bindex < 16; bindex++)
  {
    if (Current_CRC & 0x80000000)
      Current_CRC = (Current_CRC << 1) ^ CRC_POLY;
    else
      Current_CRC <<= 1;
  }
}

/*
* Internal function.
* Calculate CRC value for 8-bit.
*/
static void CRC8(uint8_t Input_Data)
{
  uint8_t bindex;

  Current_CRC ^= (Input_Data << 24);

  for (bindex = 0; bindex < 8; bindex++)
  {
    if (Current_CRC & 0x80000000)
      Current_CRC = (Current_CRC << 1) ^ CRC_POLY;
    else
      Current_CRC <<= 1;
  }
}

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
uint32_t CRC_Calculate(const uint8_t *pBuffer, uint16_t BufferLength)
{
  uint16_t i;

  /* Initialize current CRC value */
  Current_CRC = CRC_INITVAL;

  for (i = 0; i < (BufferLength / 4); i++)
  {
    CRC32(((uint32_t)pBuffer[4 * i] << 24) | ((uint32_t)pBuffer[4 * i + 1] << 16) | ((uint32_t)pBuffer[4 * i + 2] << 8) | (uint32_t)pBuffer[4 * i + 3]);
  }
  /* last bytes specific handling */
  if ((BufferLength % 4) != 0)
  {
    if (BufferLength % 4 == 1)
    {
      CRC8((uint8_t)pBuffer[4 * i]);
    }
    if (BufferLength % 4 == 2)
    {
      CRC16((uint16_t)pBuffer[4 * i] << 8 | (uint16_t)pBuffer[4 * i + 1]);
    }
    if (BufferLength % 4 == 3)
    {
      CRC16(((uint16_t)pBuffer[4 * i] << 8) | (uint16_t)pBuffer[4 * i + 1]);
      CRC8(pBuffer[4 * i + 2]);
    }
  }

  /* Return the CRC computed value */
  return Current_CRC;
}

