/**
  **************************************************************
  * @file       Serial_BSP.h
  * @author	    高明飞
  * @version    V1.0
  * @date       2015-11-23
  *
  * @brief      串口数据发送抽象接口
  *
  * @details 
  * @verbatim
  * 本程序主要封装了一些常用的串口数据发送函数，便于不同项目中重复使用。
  * 需要配合底层HAL程序使用。
  *
  * 修改记录：
  * 2015-07-19 :
  *   - File Created.
  * 2015-09-12 :
  *   - Bug Fixed;
  *   - 添加UART_LEGACY宏定义.
  * 2015-09-26 :
  *   - 修改注释格式使其可以使用Doxygen.
  * 2015-11-23 :
  *   - 进一步修改注释格式。
  * @endverbatim
  *
  * @note
  * 程序中未包含具体的底层实现方法，在使用时需要根据具体平台改写相关宏定义。
  ***************************************************************
  */
 
#ifndef UARTSENDDATA_H
#define UARTSENDDATA_H
 
/* C++ */
#ifdef __cplusplus  
extern "C" {
#endif

/** @addtogroup Serial
  * @brief      串口数据发送模块
  * @{
  */

#ifdef UART_LEGACY
void UARTSendChar(const char c);
#else
/**
  * @brief  发送一个字符
  *
  * @param  c: 需要发送的字符
  *
  * @retval None
  */
inline void UARTSendChar(const char c)
{
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8(c);
}
#endif



#ifdef UART_LEGACY
void UARTSendByte(const uint8_t UARTdata);
#else
/**
  * @brief  发送一个二进制数据，以字节形式
  *
  * @param  UARTdata: 需要发送的数据
  *
  * @retval None
  */
inline void UARTSendByte(const uint8_t UARTdata)
{
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8(UARTdata);
}
#endif



#ifdef UART_LEGACY
void UARTSendWord(const uint16_t UARTdata);
#else
/**
  * @brief  发送一个二进制数据，以字(WORD, 2 Bytes)形式
  *
  * @param  UARTdata: 需要发送的数据
  *
  * @retval None
  *
  * @note   先发送高字节
  */
inline void UARTSendWord(const uint16_t UARTdata)
{
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata>>8 & 0x00FF));
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata & 0x00FF));
}
#endif



#ifdef UART_LEGACY
void UARTSendDword(const uint32_t UARTdata);
#else
/**
  * @brief  发送一个二进制数据，以双字(DWORD, 4 Bytes)形式
  *
  * @param  UARTdata:
  *
  * @retval 需要发送的数据
  *
  * @note   先发送高字节
  */
inline void UARTSendDword(const uint32_t UARTdata)
{
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata>>24 & 0x00FF));
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata>>16 & 0x00FF));
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata>>8 & 0x00FF));
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata & 0x00FF));
}
#endif


void UARTSendUnsignASCII(uint32_t UARTdata, uint8_t base, uint8_t align);

void UARTSendSignASCII(int32_t UARTdata, uint8_t align);

void UARTSendString(const char * str);

void UARTSendByteArray(const uint8_t * UARTdata, const uint16_t num);

void UARTSendWordArray(const uint16_t * UARTdata, const uint16_t num);

void UARTSendDwordArray(const uint32_t * UARTdata, const uint16_t num);

#ifndef UART_LEGACY
void UARTprintf(const char *format, ...);
#endif

/**
  * @}
  */


/* C++ */
#ifdef __cplusplus  
}
#endif
 
#endif
 
