/**
  **************************************************************
  * @file       DigitronTable.h
  * @author	    高明飞
  * @version    V1.0
  * @date       2015-11-26
  *
  * @brief      通用8段数码管字符定义头文件
  *
  * @details 
  * @verbatim
  * 通用8段数码管字符定义头文件。
  * 同时支持共阴和共阳两种形式，通过条件编译确定
  * 数据最低位对应a段，依次为a->b->c->d->e->f->g->dp, MSB表示dp
  *
  * 修改记录：
  * 2015-09-25 :
  *   - File Created.
  * 2015-11-26 :
  *   - 修改注释格式
  *
  * @endverbatim
  ***************************************************************
  */
 
#ifndef DIGITRONTABLE_H
#define DIGITRONTABLE_H

/************* 共阴、共阳选择 *************/
#define DIGITRON_COMMMON_ANODE
//#define DIGITRON_COMMMON_CATHODE

#ifdef DIGITRON_COMMMON_ANODE
/**
  * @brief  共阳数码管
  */
const char DigitronTable[] = 
{
  0xC0,  /* 0 */
  0xF9,  /* 1 */
  0xA4,  /* 2 */
  0xB0,  /* 3 */
  0x99,  /* 4 */
  0x92,  /* 5 */
  0x82,  /* 6 */
  0xF8,  /* 7 */
  0x80,  /* 8 */
  0x90,  /* 9 */
  0xC6,  /* C */  /* 0x0A */
  0x86,  /* E */  /* 0x0B */
  0x8E,  /* F */  /* 0x0C */
  0x89,  /* H */  /* 0x0D */
  0xC7,  /* L */  /* 0x0E */
  0x8C   /* P */  /* 0x0F */
};

/**
  * @brief  小数点
  * 
  * @detail
  * 如需使用小数点DP，在原来数字的基础上加上“& Digitron_DP”
  */
#define Digitron_DP 0x7F

#endif

#ifdef DIGITRON_COMMMON_CATHODE
/**
* @brief  共阴数码管
*/
const char DigitronTable[] = 
{
  0x3F,  /* 0 */
  0x06,  /* 1 */
  0x5B,  /* 2 */
  0x4F,  /* 3 */
  0x66,  /* 4 */
  0x6D,  /* 5 */
  0x7D,  /* 6 */
  0x07,  /* 7 */
  0x7F,  /* 8 */
  0x6F,  /* 9 */
  0x39,  /* C */  /* 0x0A */
  0x79,  /* E */  /* 0x0B */
  0x71,  /* F */  /* 0x0C */
  0x76,  /* H */  /* 0x0D */
  0x38,  /* L */  /* 0x0E */
  0x73   /* P */  /* 0x0F */
};

/**
* @brief  小数点
*
* @detail
* 如需使用小数点DP，在原来数字的基础上加上“| Digitron_DP”
*/
#define Digitron_DP 0x80

#endif

#endif

 