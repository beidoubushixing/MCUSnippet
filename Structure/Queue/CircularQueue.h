/**
  **************************************************************
  * @file       CircularQueue.h
  * @author	    ������
  * @version    V0.1
  * @date       2015-11-26
  *
  * @brief      ѭ������
  *
  * @details 
  * @verbatim
  * ��ģ���װ��ѭ�����е�ʵ�֡�
  * ʹ���������˳��洢��
  *
  * �޸ļ�¼��
  * 2015-11-26 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "TypeDef.h"

/** @addtogroup DataStructure
  * @brief Basic data structure
  * @{
  */

/** @addtogroup CircularQueue
  * @brief ѭ������
  * @{
  */

/**
  * @brief  ����Buffer�����С, <255
  */
#define Queue_Buffer_Size 16

/**
  * @brief  ������ÿ��Ԫ�ص�����
  */
#define Queue_EleType uint8_t

/**
  * @brief  ѭ������
  *
  * @detail
  * Ϊ���ֶ����Ͷӿգ���Ҫ����һ���洢��Ԫ��
  * ��Buffer��ʵ��Ԫ�صĸ���ΪQueue_Buffer_Size + 1��
  */
typedef struct
{
  Queue_EleType Buffer[Queue_Buffer_Size + 1];    /*!<Buffer */
  uint8_t front;                            /*!<����ͷ */
  uint8_t rear;                             /*!<����β */
} CircularQueue;

void CircularInit(CircularQueue * Q);
Bool CircularIsEmpty(CircularQueue * Q);
ErrorStatus CircularEnQueue(CircularQueue * Q, Queue_EleType x);
ErrorStatus CircularDeQueue(CircularQueue * Q, Queue_EleType * x);

/**
  * @}
  */

/**
  * @}
  */

#endif


