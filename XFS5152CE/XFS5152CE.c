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
 * 2015-09-27 :\n
 *   -为内部函数增加static限定\n
 * 
 */
 
#include "XFS5152CE.h"

/* 帧头 */
#define XFS_FH 0xFD

/*
 * 返回值 
 */
/* 初始化成功 */
#define XFS_RE_Init 0x4A
/* 收到正确命令 */
#define XFS_RE_Success 0x41
/* 不能识别命令 */
#define XFS_RE_Fail 0x45
/* 芯片忙碌 */
#define XFS_RE_Busy 0x4E
/* 芯片空闲 */
#define XFS_RE_Ready 0x4F

/*
 * 命令字 
 */
/* 状态查询 */
#define XFS_CMD_State 0x21
/* 停止合成 */
#define XFS_CMD_Stop 0x02
/* 暂停合成 */
#define XFS_CMD_Pause 0x03
/* 恢复合成 */
#define XFS_CMD_Resume 0x04
/* 开始合成命令 */
#define XFS_CMD_Start 0x01

/*
 * 编码格式
 */
#define XFS_FORMAT_GB2312 0x00
#define XFS_FORMAT_GBK 0x01
#define XFS_FORMAT_BIG5 0x02
#define XFS_FORMAT_UNICODE 0x03


/* Buffer */
static uint8_t XFS_Buffer[XFS_BUFFER_SIZE + 5];
static uint8_t XFS_tmpRe;

/* Internal Functions */
/* 发送一条命令 */
XFSResult _XF_SendCMD(uint8_t CMD);
/* 获取芯片返回值 */
XFSResult _XF_GetResult();
/* 生成提示音基本部分数据 */
void _XF_FillTone();


/**
 * 初始化XFS5152CE模块
 * 判断通信信道是否正常，恢复默认合成参数
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Init()
{
  /* 发送状态查询命令 */
  XFS_Buffer[0] = XFS_FH;
  XFS_Buffer[1] = 0x00;
  XFS_Buffer[2] = 0x01;
  XFS_Buffer[3] = XFS_CMD_State;
  XFS5152CE_SEND_BYTES(XFS_Buffer, 4);
  
  /* 读取返回结果 */
  XFS5152CE_READ_BYTE(&XFS_tmpRe);
  
  if(XFS_tmpRe == XFS_RE_Init)
    XFS5152CE_READ_BYTE(&XFS_tmpRe);
	
  if(XFS_tmpRe == XFS_RE_Ready)
	{
		/* 恢复默认合成参数 */
		/* [d][z1] */
		XFS_Buffer[0] = XFS_FH;
		XFS_Buffer[1] = 0x00;
		XFS_Buffer[2] = 0x10;
		XFS_Buffer[3] = XFS_CMD_Start;
		XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
		XFS_Buffer[5] = 0x5B;
		XFS_Buffer[6] = 0x00;
		XFS_Buffer[7] = 0x64;
		XFS_Buffer[8] = 0x00;
		XFS_Buffer[9] = 0x5D;
		XFS_Buffer[10] = 0x00;
		XFS_Buffer[11] = 0x5B;
		XFS_Buffer[12] = 0x00;
		XFS_Buffer[13] = 0x7A;
		XFS_Buffer[14] = 0x00;
		XFS_Buffer[15] = 0x31;
		XFS_Buffer[16] = 0x00;
		XFS_Buffer[17] = 0x5D;
		XFS_Buffer[18] = 0x00;
		
		XFS5152CE_SEND_BYTES(XFS_Buffer, 19);
		return _XF_GetResult();
	}
  else
	{
    return XFSERROR;
	}
}

/**
 * 暂停语音合成
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Pause()
{
  return _XF_SendCMD(XFS_CMD_Pause);
}

/**
 * 恢复语音合成
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Resume()
{
	return _XF_SendCMD(XFS_CMD_Resume);
}

/**
 * 停止语音合成
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Stop()
{
	return _XF_SendCMD(XFS_CMD_Stop);
}

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
XFSResult XFS5152CE_Start(uint8_t * XFData, uint8_t XFCount)
{
  uint8_t i;
  
  if(2 * XFCount > XFS_BUFFER_SIZE)
    return XFSERROR;
  
  XFS_Buffer[0] = XFS_FH;
  XFS_Buffer[1] = 0x00;
  XFS_Buffer[2] = 2 * XFCount + 2;
  XFS_Buffer[3] = XFS_CMD_Start;
  XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
  for(i = 0;i < 2 * XFCount;i++)
  {
    XFS_Buffer[i+5] = XFData[i];
  }

  XFS5152CE_SEND_BYTES(XFS_Buffer, 2 * XFCount + 5);
	return _XF_GetResult();
}

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
XFSResult XFS5152CE_Volume(uint8_t XFVolume)
{
	uint16_t i; 
	/* [v?] */
	XFS_Buffer[0] = XFS_FH;
	XFS_Buffer[1] = 0x00;
	XFS_Buffer[2] = 0x0A;
	XFS_Buffer[3] = XFS_CMD_Start;
	XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
	XFS_Buffer[5] = 0x5B;
	XFS_Buffer[6] = 0x00;
	XFS_Buffer[7] = 0x76;
	XFS_Buffer[8] = 0x00;
	XFS_Buffer[9] = XFVolume + 0x30;
	XFS_Buffer[10] = 0x00;
	XFS_Buffer[11] = 0x5D;
	XFS_Buffer[12] = 0x00;
	
	XFS5152CE_SEND_BYTES(XFS_Buffer, 13);
	return _XF_GetResult();
}

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
XFSResult XFS5152CE_Speed(uint8_t XFSpeed)
{
	/* [s?] */
	XFS_Buffer[0] = XFS_FH;
	XFS_Buffer[1] = 0x00;
	XFS_Buffer[2] = 0x0A;
	XFS_Buffer[3] = XFS_CMD_Start;
	XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
	XFS_Buffer[5] = 0x5B;
	XFS_Buffer[6] = 0x00;
	XFS_Buffer[7] = 0x73;
	XFS_Buffer[8] = 0x00;
	XFS_Buffer[9] = XFSpeed + 0x30;
	XFS_Buffer[10] = 0x00;
	XFS_Buffer[11] = 0x5D;
	XFS_Buffer[12] = 0x00;
	
	XFS5152CE_SEND_BYTES(XFS_Buffer, 13);	
	return _XF_GetResult();
}

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
XFSResult XFS5152CE_Tune(uint8_t XFTune)
{
	/* [t?] */
	XFS_Buffer[0] = XFS_FH;
	XFS_Buffer[1] = 0x00;
	XFS_Buffer[2] = 0x0A;
	XFS_Buffer[3] = XFS_CMD_Start;
	XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
	XFS_Buffer[5] = 0x5B;
	XFS_Buffer[6] = 0x00;
	XFS_Buffer[7] = 0x74;
	XFS_Buffer[8] = 0x00;
	XFS_Buffer[9] = XFTune + 0x30;
	XFS_Buffer[10] = 0x00;
	XFS_Buffer[11] = 0x5D;
	XFS_Buffer[12] = 0x00;
	
	XFS5152CE_SEND_BYTES(XFS_Buffer, 13);
	return _XF_GetResult();
}

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
XFSResult XFS5152CE_Speaker(XFSSpeakers XFSpeaker)
{
	/* [m?] */
	/* [m3] 单独处理 */
	if(XFSpeaker == XF_XiaoYan)
	{
		XFS_Buffer[0] = XFS_FH;
		XFS_Buffer[1] = 0x00;
		XFS_Buffer[2] = 0x0A;
		XFS_Buffer[3] = XFS_CMD_Start;
		XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
		XFS_Buffer[5] = 0x5B;
		XFS_Buffer[6] = 0x00;
		XFS_Buffer[7] = 0x6D;
		XFS_Buffer[8] = 0x00;
		XFS_Buffer[9] = 0x33;  /* 3 */
		XFS_Buffer[10] = 0x00;
		XFS_Buffer[11] = 0x5D;
		XFS_Buffer[12] = 0x00;
		
		XFS5152CE_SEND_BYTES(XFS_Buffer, 13);	
		return _XF_GetResult();
	}
	
	/* m[5?] */
	XFS_Buffer[0] = XFS_FH;
	XFS_Buffer[1] = 0x00;
	XFS_Buffer[2] = 0x0C;
	XFS_Buffer[3] = XFS_CMD_Start;
	XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
	XFS_Buffer[5] = 0x5B;
	XFS_Buffer[6] = 0x00;
	XFS_Buffer[7] = 0x6D;
	XFS_Buffer[8] = 0x00;
	XFS_Buffer[9] = 0x35;
	XFS_Buffer[10] = 0x00;
	XFS_Buffer[11] = (uint8_t)XFSpeaker - 2;
	XFS_Buffer[12] = 0x00;
	XFS_Buffer[13] = 0x5D;
	XFS_Buffer[14] = 0x00;
	
	XFS5152CE_SEND_BYTES(XFS_Buffer, 15);
	return _XF_GetResult();
}

/**
 * 播放信息提示音
 *
 * \param XFNum: 信息提示音范围1~25
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Msg_Tone(uint8_t XFNum)
{
	_XF_FillTone();
	
	XFS_Buffer[15] = 0x31;
	XFS_Buffer[16] = 0x00;
	
	XFS_Buffer[17] = (XFNum / 10) + 0x30;
	XFS_Buffer[18] = 0x00;
	XFS_Buffer[19] = (XFNum % 10) + 0x30;
	XFS_Buffer[20] = 0x00;
	
	XFS5152CE_SEND_BYTES(XFS_Buffer, 21);
	return _XF_GetResult();
}

/**
 * 播放铃声提示音
 *
 * \param XFNum: 铃声提示音范围1~25
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Ring_Tone(uint8_t XFNum)
{
	_XF_FillTone();
	
	XFS_Buffer[15] = 0x32;
	XFS_Buffer[16] = 0x00;
	
	XFS_Buffer[17] = (XFNum / 10) + 0x30;
	XFS_Buffer[18] = 0x00;
	XFS_Buffer[19] = (XFNum % 10) + 0x30;
	XFS_Buffer[20] = 0x00;
	
	XFS5152CE_SEND_BYTES(XFS_Buffer, 21);
	return _XF_GetResult();
}

/**
 * 播放警报提示音
 *
 * \param XFNum: 警报提示音1~30
 * 
 * \return 函数执行结果
 * \retval XFSOK: 函数执行正常
 * \retval XFSERROR: 函数执行异常
 */
XFSResult XFS5152CE_Warning_Tone(uint8_t XFNum)
{
	_XF_FillTone();
	
	XFS_Buffer[15] = 0x33;
	XFS_Buffer[16] = 0x00;
	
	XFS_Buffer[17] = (XFNum / 10) + 0x30;
	XFS_Buffer[18] = 0x00;
	XFS_Buffer[19] = (XFNum % 10) + 0x30;
	XFS_Buffer[20] = 0x00;
	
	XFS5152CE_SEND_BYTES(XFS_Buffer, 21);
	return _XF_GetResult();
}

/* 发送一条命令 */
XFSResult _XF_SendCMD(uint8_t CMD)
{
	XFS_Buffer[0] = XFS_FH;
  XFS_Buffer[1] = 0x00;
  XFS_Buffer[2] = 0x01;
  XFS_Buffer[3] = CMD;

  XFS5152CE_SEND_BYTES(XFS_Buffer, 4);
  return _XF_GetResult();
}

/* 获取芯片返回值 */
XFSResult _XF_GetResult()
{
	/* 读取返回结果 */
	while(1)
	{
		XFS5152CE_READ_BYTE(&XFS_tmpRe);
		switch(XFS_tmpRe)
		{
			case XFS_RE_Fail:
				return XFSERROR;
			
			case 0xFF:
				return XFSOK;
			
			default:
				continue;
		}
	}
}

/* 生成提示音基本部分数据 */
void _XF_FillTone()
{
	/* sound??? */
	XFS_Buffer[0] = XFS_FH;
	XFS_Buffer[1] = 0x00;
	XFS_Buffer[2] = 0x12;
	XFS_Buffer[3] = XFS_CMD_Start;
	XFS_Buffer[4] = XFS_FORMAT_UNICODE;  /* 使用UNICODE编码 */
	XFS_Buffer[5] = 0x73;
	XFS_Buffer[6] = 0x00;
	XFS_Buffer[7] = 0x6F;
	XFS_Buffer[8] = 0x00;
	XFS_Buffer[9] = 0x75;
	XFS_Buffer[10] = 0x00;
	XFS_Buffer[11] = 0x6E;
	XFS_Buffer[12] = 0x00;
	XFS_Buffer[13] = 0x64;
	XFS_Buffer[14] = 0x00;
}

