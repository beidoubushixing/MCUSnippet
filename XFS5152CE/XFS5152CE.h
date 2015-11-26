/**
 * \file
 * 
 * \brief 科大讯飞TTS芯片 XFS5152CE BSP程序
 *
 * \author 高明飞
 *
 * \date 2015-09-24
 *
 * \details 
 * 本模块封装好了XFS5152CE的接口，使用时需要先配置好底层通信驱动，
 * 之后直接调用相关函数即可。\n
 * 其中需要先调用初始化函数。
 * 
 * \note
 * 通信方式可配置为UART、SPI、I2C，此BSP程序中不涉及具体实现方式。
 *
 * \todo
 * 目前未实现以下功能：\n
 * 1.语音编解码功能\n
 * 2.语音识别功能\n
 * 3.文本缓存功能\n
 * 4.省电模式（此部分功能数据手册给出的信息不全，进入省电模式后唤醒有问题）\n
 *
 * 修改记录：\n
 *
 * 2015-09-26 :\n
 *   -修改注释格式使其可以使用Doxygen\n
 * 
 */
 
#ifndef XFS5152CE_H
#define XFS5152CE_H

/*--------------------此部分需要修改--------------------*/
/* 如未定义uint8_t等基本数据类型，需要先定义 */
#include "TypeDef.h"

/* 底层HAL头文件 */
#include "GPIO_I2C.h"
#include "STC15F2K60S2.h"

/**
 * 数据缓存区大小，一次能发送的中文个数最多为 XFS_BUFFER_SIZE / 2 个
 *
 * \warning
 * -XFS_BUFFER_SIZE要小于100，否则程序中某些地方会发生溢出;\n
 * -为支持文本控制标记，XFS_BUFFER_SIZE需要大于16.
 */
#define XFS_BUFFER_SIZE 50
 
/* 底层HAL文件中需要实现3个基本操作：
 * 1.发送一段数据（以字节数组的形式）
 * 2.读取一个字节数据
 * 3.返回RDY引脚电平
 */
 
/**
 * 发送一段数据宏定义
 * \param XFSData: 待发送数据指针
 * \param XFSCount: 数据个数
 */
#define XFS5152CE_SEND_BYTES(XFSData,XFSCount) I2CWriteMultiBytes(0x40,XFSCount,XFSData)

/**
 * 读取一个字节数据宏定义
 * \param XFSData: 读取到的数据结果储存位置指针
 */
#define XFS5152CE_READ_BYTE(XFSData) I2CReadByte(0x40,XFSData)

/**
 * 返回RDY引脚电平
 *
 * \retval H: 芯片正忙
 * \retval L: 芯片空闲
 *
 * \note 程序中可直接调用此宏来判断是否发送新数据
 */
#define XFS5152CE_BUSY P42

/*--------------------此部分需要修改--------------------*/

/** 返回结果枚举定义 */
typedef enum
{
  XFSOK,    /**< 函数执行正常 */
	XFSERROR  /**< 函数执行发生错误 */
} XFSResult;

/** 发音人枚举 */
typedef enum 
{
  XF_XiaoYan=03,     /**< 小燕（女，推荐） */
  XF_Xujiu=51,       /**< 许久（男，推荐） */
  XF_XuDuo=52,       /**< 许多（男） */
	XF_XiaoPing=53,    /**< 小萍 （女）*/
	XF_Donald=54,      /**< 唐老鸭（合成器效果） */
	XF_XuXiaobao=55    /**< 许小宝（女童声） */
} XFSSpeakers;


/**
 * 初始化XFS5152CE模块
 * 判断通信信道是否正常，恢复默认合成参数
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Init();

/**
 * 暂停语音合成
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Pause();

/**
 * 恢复语音合成
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Resume();

/**
 * 停止语音合成
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Stop();

/**
 * 开始语音合成
 * 
 * \param XFData: 待合成数据头指针
 * \param XFCount: 待合成数据个数，需小于XFS_BUFFER_SIZE / 2
 *
 * \note XFCount为实际中文字符字数，不是字节数，一般XFCount为字节数的一半
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Start(uint8_t * XFData, uint8_t XFCount);

/**
 * 设置音量
 * 
 * \param XFVolume: 音量值，取值范围：[0, 9]，为整数
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 *
 * \note 复位后初始值为5
 */
XFSResult XFS5152CE_Volume(uint8_t XFVolume);

/**
 * 设置语速
 * 
 * \param XFSpeed: 语速，取值范围：[0, 9]，为整数
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 *
 * \note 复位后初始值为5
 */
XFSResult XFS5152CE_Speed(uint8_t XFSpeed);

/**
 * 设置语调
 * 
 * \param XFTune: 语调，取值范围：[0, 9]，为整数
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 *
 * \note 复位后初始值为5
 */
XFSResult XFS5152CE_Tune(uint8_t XFTune);

/**
 * 设置发音人
 *
 * \param XFSpeaker: 发音人，可能取值：
 * \arg XF_XiaoYan: 小燕（女，推荐）
 * \arg XF_Xujiu: 许久（男，推荐）
 * \arg XF_XuDuo: 许多（男）
 * \arg XF_XiaoPing: 小萍（女）
 * \arg XF_Donald: 唐老鸭（合成器效果）
 * \arg XF_XuXiaobao: 许小宝（女童声）
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Speaker(XFSSpeakers XFSpeaker);

/**
 * 播放信息提示音
 *
 * \param XFNum: 信息提示音范围1~25
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Msg_Tone(uint8_t XFNum);

/**
 * 播放铃声提示音
 *
 * \param XFNum: 铃声提示音范围1~25
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Ring_Tone(uint8_t XFNum);

/**
 * 播放警报提示音
 *
 * \param XFNum: 警报提示音1~30
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Warning_Tone(uint8_t XFNum);

#endif

