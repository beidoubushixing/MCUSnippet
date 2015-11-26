/**
 * \file 
 *
 * \brief GPIO模拟I2C程序
 * 
 * \author 高明飞
 *
 * \date 2015-09-12
 *
 * \note 某些低端MCU（如51）没有硬件I2C模块，
 *       故需要使用GPIO实现I2C协议
 *
 * \todo 目前未实现连续读多个字节数据
 *
 * 修改记录：\n
 *
 * 2015-09-27 :\n
 *   -修改注释格式使其可以使用Doxygen\n
 *   -为内部函数增加static限定\n
 * 
 */
 
#ifndef GPIO_I2C_H
#define GPIO_I2C_H

/*--------------------此部分需要修改--------------------*/
/* 如未定义uint8_t等基本数据类型，需要先定义 */
#include "TypeDef.h"

/* HAL层提供的GPIO底层实现宏定义，此处以STC15W1K16S为例，需要根据实际情况修改 */

/* HAL头文件 */
#include "STC15F2K60S2.h"
 
/**
 * 时钟信号(SCL)宏定义，指定SCL电平
 */
#define HAL_SCL_W(STATE) (P33=STATE)

/**
 * 数据信号(SDA)宏定义，指定SDA电平
 */
#define HAL_SDA_W(STATE) (P34=STATE)

/**
 * 读取时钟信号(SCL)当前电位
 */
#define HAL_SCL_R (P33)

/**
 * 读取数据信号(SDA)当前电位
 */
#define HAL_SDA_R (P34)

/**
 * 延时时间宏定义，值越大则总线速度越慢
 */
#define I2C_DELAY_COUNT 45

/*--------------------此部分需要修改--------------------*/

/** 返回结果枚举定义 */
typedef enum
{
	I2COK,    /**< 成功 */
	I2CERROR  /**< 错误 */
} I2CResult;


/**
 * 持续输出时钟信号，方便示波器观察目前的总线速度
 *
 */
void SCLTestOut();


/*
/**
 * 发送一个字节数据
 * 
 * \param address: 从设备地址
 * \param I2Cdata: 需要发送的数据
 *
 * \return 执行结果
 * \retval I2COK    : 成功
 * \retval I2CERROR : 失败
 *
 */
I2CResult I2CWriteByte(uint8_t address, uint8_t I2Cdata);
	
	
/**
 * 发送多个字节数据
 * 
 * \param address : 从设备地址
 * \param count   : 数据长度
 * \param I2Cdata : 需要发送的数据数组
 *
 * \return 执行结果
 * \retval I2COK    : 成功
 * \retval I2CERROR : 失败
 *
 */
I2CResult I2CWriteMultiBytes(uint8_t address, uint8_t count, uint8_t I2Cdata[]);


/**
 * 写寄存器，相当于先发送寄存器地址，再发送数据
 * 
 * \param address : 从设备地址
 * \param reg     : 寄存器地址
 * \param I2Cdata : 需要发送的数据
 *
 * \return 执行结果
 * \retval I2COK    : 成功
 * \retval I2CERROR : 失败
 *
 */
I2CResult I2CWriteReg(uint8_t address, uint8_t reg, uint8_t I2Cdata);


/**
 * 读取一个字节数据
 * 
 * \param address : 从设备地址
 * \param I2Cdata : 读取到的数据存放指针
 *
 * \return 执行结果
 * \retval I2COK    : 成功
 * \retval I2CERROR : 失败
 *
 */
I2CResult I2CReadByte(uint8_t address, uint8_t * I2Cdata);


/**
 * 读取一个寄存器中的数据，相当于先发送寄存器地址，然后从设备返回数据。
 * 发送寄存器地址为Dummy Write，之后跟随一个Start信号
 * 
 * \param address : 从设备地址
 * \param reg     : 寄存器地址
 * \param I2Cdata : 读取到的数据存放指针
 *
 * \return 执行结果
 * \retval I2COK    : 成功
 * \retval I2CERROR : 失败
 *
 */
I2CResult I2CReadReg(uint8_t address, uint8_t reg, uint8_t * I2Cdata);
 
#endif
