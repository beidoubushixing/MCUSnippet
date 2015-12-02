/**
  **************************************************************
  * @file       CircularQueue.h
  * @author     高明飞
  * @version    V0.1
  * @date       2015-11-26
  *
  * @brief      循环队列
  *
  * @details 
  * @verbatim
  * 本模块封装了循环队列的实现。
  * 使用数组进行顺序存储。
  *
  * 修改记录：
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
  * @brief 循环队列
  * @{
  */

/**
  * @brief  队列Buffer数组大小, <255
  */
#define Queue_Buffer_Size 16

/**
  * @brief  队列中每个元素的类型
  */
#define Queue_EleType uint8_t

/**
  * @brief  循环队列
  *
  * @detail
  * 为区分队满和队空，需要牺牲一个存储单元。
  * 故Buffer中实际元素的个数为Queue_Buffer_Size + 1。
  */
typedef struct
{
  Queue_EleType Buffer[Queue_Buffer_Size + 1];    /*!<Buffer */
  uint8_t front;                            /*!<数据头 */
  uint8_t rear;                             /*!<数据尾 */
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


