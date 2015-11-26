/**
  **************************************************************
  * @file       TypeDef.h
  * @author	    高明飞
  * @version    V1.1
  * @date       2015-11-26
  *
  * @brief      定义一些基本数据类型
  *
  * @details 
  * @verbatim
  * 程序的详细说明。
  *
  * 修改记录：
  * 2015-09-12 :
  *   - File Created.
  * 2015-09-27 :
  *   - 修改注释格式使其可以使用Doxygen
  * 2015-11-23 :
  *   - 进一步修改注释格式
  * 2015-11-26 :
  *   - 增加Bool类型的定义
  *   - 增加ErrorStatus类型的定义
  *
  * @endverbatim
  *
  * @note
  * 如果编译平台提供了自己的定义文件，不要使用此文件
  ***************************************************************
  */

#ifndef TYPEDEF_H
#define TYPEDEF_H

#ifndef TYPE_UINT32_T
#define TYPE_UINT32_T
/** 32位无符号整数 */
typedef unsigned int uint32_t;
#endif

#ifndef TYPE_UINT16_T
#define TYPE_UINT16_T
/** 16位无符号整数 */
typedef unsigned short uint16_t;
#endif

#ifndef TYPE_UINT8_T
#define TYPE_UINT8_T
/** 8位无符号整数 */
typedef unsigned char uint8_t;
#endif

#ifndef TYPE_INT32_T
#define TYPE_INT32_T
/** 32位有符号整数 */
typedef signed int int32_t;
#endif

#ifndef TYPE_INT16_T
#define TYPE_INT16_T
/** 16位有符号整数 */
typedef signed short int16_t;
#endif

#ifndef TYPE_INT8_T
#define TYPE_INT8_T
/** 8位有符号整数 */
typedef signed char int8_t;
#endif

#ifndef TYPE_BOOL
#define TYPE_BOOL
/** Bool类型 */
typedef enum { FALSE = 0, TRUE = !FALSE } Bool;
#endif

#ifndef TYPE_RESULT
#define TYPE_RESULT
/** ErrorStatus类型 */
typedef enum { ERROR = 0, SUCCESS = !ERROR } ErrorStatus;
#endif

#endif

