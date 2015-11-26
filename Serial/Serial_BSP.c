/**
  **************************************************************
  * @file       Serial_BSP.c
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

/** @addtogroup Serial
  * @brief      串口数据发送模块
  * @{
  */

/*--------------------此部分需要修改--------------------*/
/* 编译选项开关，部分编译器（如51）不支持inline、va_list等，此时不使用内联函数，不实现printf() */
//#define UART_LEGACY

/* 如未定义uint8_t等基本数据类型，需要先定义 */
//#include "TypeDef.h"

/* HAL层提供的UART底层实现宏定义，此处以TMS320F28027为例，需要根据实际情况修改 */

/* HAL头文件 */
#include "System.h"
#include "UART.h"

/**
  * UART发送数据宏定义，一般发送数据都是向某个寄存器中写入一个8位数据。
  * 此处data即为要发送的8位（1 byte）数据。
  */
#define HAL_UART_SEND_UINT8(UARTdata) (SciaRegs.SCITXBUF = UARTdata)

/**
  * UART判断是否可以发送新数据宏定义。
  * 一般都存在一个寄存器可以用于判断此时TX Buffer是否可以接收新数据。
  * 可以发送数据时，宏的值应为非0值；
  * 当宏的值为0时，代表此时不能发送新数据。
  */
#define HAL_UART_TX_READY (SciaRegs.SCICTL2.bit.TXRDY)

/*--------------------此部分需要修改--------------------*/

/* 内联函数直接在头文件中定义 */
#include "UARTSendData.h"

#ifndef UART_LEGACY
#include "stdio.h"
#endif

#ifdef UART_LEGACY
/**
  * @brief  发送一个字符
  *
  * @param  c: 需要发送的字符
  *
  * @retval None
  */
void UARTSendChar(const char c)
{
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8(c);
}
#endif



#ifdef UART_LEGACY
/**
  * @brief  发送一个二进制数据，以字节形式
  *
  * @param  UARTdata: 需要发送的数据
  *
  * @retval None
  */
void UARTSendByte(const uint8_t UARTdata)
{
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8(UARTdata);
}
#endif



#ifdef UART_LEGACY
/**
  * @brief  发送一个二进制数据，以字(WORD, 2 Bytes)形式
  *
  * @param  UARTdata: 需要发送的数据
  *
  * @retval None
  *
  * @note   先发送高字节
  */
void UARTSendWord(const uint16_t UARTdata)
{
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata>>8 & 0x00FF));
  while(!HAL_UART_TX_READY);
  HAL_UART_SEND_UINT8((uint8_t)(UARTdata & 0x00FF));
}
#endif



#ifdef UART_LEGACY
/**
  * @brief  发送一个二进制数据，以双字(DWORD, 4 Bytes)形式
  *
  * @param  UARTdata: 
  *
  * @retval 需要发送的数据
  *
  * @note   先发送高字节
  */
void UARTSendDword(const uint32_t UARTdata)
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

/**
  * @brief  发送一个无符号二进制数据，以ASCII码形式
  *
  * @param  UARTdata: 需要发送的数据
  * @param  base:     数据进制，理论上支持2~36进制，常用值：
  *   @arg 2:   二进制
  *   @arg 10:  十进制
  *   @arg 16:  十六进制
  * @param  align:    是否需要对齐
  *   @arg 0:     无需对齐
  *   @arg other: 指定的位数，若数据所需位数小于此值，将用0在前面补足
  *
  * @retval void
  *
  * @note
  * 若数据所需位数大于align值，将按实际位数输出，不会截短。
  */
void UARTSendUnsignASCII(uint32_t UARTdata, uint8_t base, uint8_t align)
{
	/* 转换后的ASCII码 */
	char str[32];
	/* 计数变量 */
	signed char i;
	/* 临时变量 */
	char c;

	/* 处理输入数据为0的情况 */
	if(UARTdata == 0)
	{
		/* 补0对齐数据 */
		if(align == 0)
			align = 1;
		for(;align > 0;align--)
			UARTSendChar('0');
		return;
	}

	/* 逆序获得ASCII码，不断 % base */
	for(i = 0; UARTdata > 0; i++, UARTdata /= base)
	{
		c = UARTdata % base;
		if(c < 10)
			/* 如果数字小于10，将数字加上0的ASCII值(即'0')就得到该数字的ASCII值 */
			str[i] = c + '0';
		else
			/* 如果数字大于等于10，将数字减去10再加上'A' */
			str[i] = c - 10 + 'A';
	}

	/* 补0对齐数据 */
	if(i < align)
		for(;align > i;align--)
			UARTSendChar('0');

	/* 逆序发送转换得到的ASCII码 */
	for(--i;i >= 0;i--)
		UARTSendChar(str[i]);
}

/**
  * @brief  发送一个带符号二进制数据，以ASCII码形式
  *
  * @param  UARTdata: 需要发送的数据
  * @param  align:    是否需要对齐
  *   @arg 0:     无需对齐
  *   @arg other: 指定的位数，若数据所需位数小于此值，将用0在前面补足
  *
  * @retval void
  *
  * @note 
  * 若数据所需位数大于align值，将按实际位数输出，不会截短。
  * 此处的位数是指数据的位数，不包括前面的符号位。
  */
void UARTSendSignASCII(int32_t UARTdata, uint8_t align)
{
	if(UARTdata < 0)
	{
		UARTSendChar('-');
		UARTSendUnsignASCII((uint32_t)(-UARTdata), 10, align);
	}
	else
	{
		UARTSendChar('+');
		UARTSendUnsignASCII((uint32_t)(UARTdata), 10, align);
	}
}


/**
  * @brief  发送字符串
  *
  * @param  str: 字符串头指针
  *
  * @retval void
  *
  * @note   字符串需以'\0'结尾
  */
void UARTSendString(const char * str)
{
    for(; *str != '\0'; str++)
        UARTSendChar(*str);
}

/**
  * @brief  发送二进制数据数组，以字节形式
  *
  * @param  UARTdata: 数组头指针
  * @param  num:      数组中数据个数
  *
  * @retval void
  */
void UARTSendByteArray(const uint8_t * UARTdata, const uint16_t num)
{
    uint16_t count = 0;
    for(; count < num; count++, UARTdata++)
    {
    	UARTSendByte(*UARTdata);
    }
}

/**
  * @brief  发送二进制数据数组，以字(WORD, 2 Bytes)形式
  *
  * @param  UARTdata: 数组头指针
  * @param  num:      数组中数据个数
  *
  * @retval void
  *
  * @note   先发送高字节
  */
void UARTSendWordArray(const uint16_t * UARTdata, const uint16_t num)
{
	uint16_t count = 0;
	for(; count < num; count++, UARTdata++)
	{
		UARTSendWord(*UARTdata);
	}
}

/**
  * @brief  发送二进制数据数组，以双字(DWORD, 4 Bytes)形式
  *
  * @param  UARTdata: 数组头指针
  * @param  num:      数组中数据个数
  *
  * @retval void
  *
  * @note   先发送高字节
  */
void UARTSendDwordArray(const uint32_t * UARTdata, const uint16_t num)
{
	uint16_t count = 0;
	for(; count < num; count++, UARTdata++)
	{
		UARTSendDword(*UARTdata);
	}
}

#ifndef UART_LEGACY
/**
  * @brief  简易版printf函数
  *
  * @param  format: 格式化字符串，支持的占位符：
  *   @arg %d: 带符号整数
  *   @arg %u: 无符号整数, 十进制形式
  *   @arg %x: 无符号整数, 十六进制形式, 自动补足至2、4、8位
  *   @arg %b: 无符号整数, 二进制形式, 自动补足至8、16、32位
  *   @arg %c: 一个ASCII字符
  *   @arg %s: 字符串
  *
  * @retval void
  *
  * @warning
  * 输入的数字类型必须为uint32_t，否则会发生错误
  */
void UARTprintf(const char *format, ...)
{
	/* 可变参数指针 */
	va_list ap;
	/* 临时变量 */
	uint32_t tmp;

	/* 初始化va_list类型的变量ap，使ap指向第一个可变参数 */
	va_start(ap, format);

	/* 依次处理字符串中的每个字符 */
	for(; *format != '\0'; format++)
	{
		if(*format != '%')
		{
			/* 一般字符，按常规字符发送即可 */
			UARTSendChar(*format);
			continue;
		}
		else
		{
			/* 处理特殊占位符 */
			/* va_arg宏获取下一个可变参数 */
			switch(*(++format)) /* 跳过%号 */
			{
			case 'd':
				UARTSendSignASCII(va_arg(ap, uint32_t), 0);
				break;
			case 'u':
				tmp = va_arg(ap, uint32_t);
				UARTSendUnsignASCII(tmp, 10, 0);
				break;
			case 'x':
				tmp = va_arg(ap, uint32_t);
				if(tmp <= 0xFF)
					UARTSendUnsignASCII(tmp, 16, 2);
				else if(tmp <= 0xFFFF)
					UARTSendUnsignASCII(tmp, 16, 4);
				else
					UARTSendUnsignASCII(tmp, 16, 8);
				break;
			case 'b':
				tmp = va_arg(ap, uint32_t);
				if(tmp <= 0xFF)
					UARTSendUnsignASCII(tmp, 2, 8);
				else if(tmp <= 0xFFFF)
					UARTSendUnsignASCII(tmp, 2, 16);
				else
					UARTSendUnsignASCII(tmp, 2, 32);
				break;
			case 'c':
				UARTSendChar(va_arg(ap, char));
				break;
			case 's':
				UARTSendString(va_arg(ap, char *));
				break;
			default:
				/* 遇到未定义占位符按字符原样发送 */
				UARTSendChar('%');
				UARTSendChar(*format);
				break;
			}
		}
	}

	/* 将ap置空 */
	va_end(ap);
}

/**
* @}
*/

#endif
 
