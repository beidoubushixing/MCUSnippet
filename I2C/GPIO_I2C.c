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
 
#include "GPIO_I2C.h"

/** ACK state enum */
typedef enum
{
	ACK,   /**< ACK */
	NOACK  /**< NOACK */
} ACK_State;
 
/* 内部使用的函数声明 */
static void _I2CDelay(void);
static void _I2CStart(void);
static void _I2CStop(void);
static ACK_State _I2CSendAddress(uint8_t address, uint8_t wr);
static ACK_State _I2CSendByte(uint8_t databyte);
static uint8_t _I2CGetByte(void);

/**
 * 持续输出时钟信号，方便示波器观察目前的总线速度
 *
 */
void SCLTestOut()
{
  HAL_SCL_W(0);
  _I2CDelay();
	HAL_SCL_W(0);
	_I2CDelay();
  HAL_SCL_W(1);
  _I2CDelay();
	HAL_SCL_W(1);
	_I2CDelay();
}


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
I2CResult I2CWriteByte(uint8_t address, uint8_t I2Cdata)
{
	_I2CStart();
	
	if(_I2CSendAddress(address, 0) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	if(_I2CSendByte(I2Cdata) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	
	_I2CStop();
	return I2COK;
}
	
	
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
I2CResult I2CWriteMultiBytes(uint8_t address, uint8_t count, uint8_t I2Cdata[])
{
	uint8_t i;
	
	_I2CStart();
	
	if(_I2CSendAddress(address, 0) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	for(i = 0; i < count; i++)
	{
		if(_I2CSendByte(I2Cdata[i]) == NOACK)
		{
			_I2CStop();
			return I2CERROR;
		}
	}

	_I2CStop();
	return I2COK;
}


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
I2CResult I2CWriteReg(uint8_t address, uint8_t reg, uint8_t I2Cdata)
{
	_I2CStart();
	
	if(_I2CSendAddress(address, 0) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	if(_I2CSendByte(reg) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	if(_I2CSendByte(I2Cdata) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	
	_I2CStop();
	return I2COK;
}


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
I2CResult I2CReadByte(uint8_t address, uint8_t * I2Cdata)
{
	_I2CStart();
	
	if(_I2CSendAddress(address, 1) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	*I2Cdata = _I2CGetByte();
	
	_I2CStop();
	return I2COK;
}


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
I2CResult I2CReadReg(uint8_t address, uint8_t reg, uint8_t * I2Cdata)
{
	_I2CStart();
	if(_I2CSendAddress(address, 0) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	if(_I2CSendByte(reg) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	
	_I2CStart();
	if(_I2CSendAddress(address, 1) == NOACK)
	{
		_I2CStop();
		return I2CERROR;
	}
	*I2Cdata = _I2CGetByte();
	
	_I2CStop();
	return I2COK;
}


/**
 * 延时函数，延时的时间为一位数据持续时间的一半
 */
static void _I2CDelay(void)
{
  uint16_t k;
  for(k = 0; k < I2C_DELAY_COUNT; k++);
}

/**
 * 发送Start信号
 */
static void _I2CStart(void)
{
	HAL_SCL_W(0);
	_I2CDelay();
	HAL_SDA_W(1);
	_I2CDelay();
	HAL_SCL_W(1);
	_I2CDelay();
	HAL_SDA_W(0);
  _I2CDelay();
}

/**
 * 发送Stop信号
 */
static void _I2CStop(void)
{
	HAL_SCL_W(0);
	_I2CDelay();
	HAL_SDA_W(0);
	_I2CDelay();
	HAL_SCL_W(1);
	_I2CDelay();
	HAL_SDA_W(1);
	_I2CDelay();
}

/**
 * 发送从设备地址
 */
static ACK_State _I2CSendAddress(uint8_t address, uint8_t wr)
{
	return _I2CSendByte((address << 1) + (wr & 0x01));
}

/**
 * 发送一个字节数据
 */
static ACK_State _I2CSendByte(uint8_t databyte)
{
	uint8_t i;
	uint16_t cnt;
	
	/* Write 8 bit data */
	for(i = 8; i > 0; i--)
	{
		HAL_SCL_W(0);
		_I2CDelay();
		HAL_SDA_W(databyte >> (i - 1) & 0x01);
		_I2CDelay();
		HAL_SCL_W(1);
		_I2CDelay();
		_I2CDelay();
	}
	
	/* Read ACK */
	cnt = 0;
	HAL_SCL_W(0);
	HAL_SDA_W(1);
	_I2CDelay();
	_I2CDelay();
	HAL_SCL_W(1);
	while(HAL_SDA_R == 1)
	{
		if(cnt++ > I2C_DELAY_COUNT * 3)
			return NOACK;
	}
	_I2CDelay();
	_I2CDelay();
	return ACK;
}

/**
 * 读取从设备返回的一个字节数据
 */
static uint8_t _I2CGetByte(void)
{
	uint8_t i;
	uint8_t tmp, res = 0;
	uint16_t cnt;
	
	/* Read 8 bit data */
	HAL_SDA_W(1);
	for(i = 8; i > 0; i--)
	{
		HAL_SCL_W(0);
		_I2CDelay();
		_I2CDelay();
		HAL_SCL_W(1);
		_I2CDelay();
		tmp = HAL_SDA_R;
		res += ((tmp & 0x01) << (i - 1));
		_I2CDelay();
	}
	
	/* Send NOACK */
	HAL_SCL_W(0);
	_I2CDelay();
	HAL_SDA_W(1);
	_I2CDelay();
	HAL_SCL_W(1);
	_I2CDelay();
	_I2CDelay();
	
	return res;
}
 