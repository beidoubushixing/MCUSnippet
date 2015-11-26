/**
  **************************************************************
  * @file       CircularQueue.c
  * @author	    高明飞
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

#include "CircularQueue.h"

/** @addtogroup DataStructure
  * @brief Basic data structure
  * @{
  */

/** @addtogroup CircularQueue
  * @brief 循环队列
  * @{
  */

/**
  * @brief  初始化循环队列
  *
  * @param  Q: 目标队列指针
  *
  * @retval None
  */
void CircularInit(CircularQueue * Q)
{
  Q->front = 0;
  Q->rear = 0;
}

/**
  * @brief  检查队列是否为空
  *
  * @param  Q: 目标队列指针
  *
  * @retval TRUE  队列空
  * @retval FALSE 队列非空
  */
Bool CircularIsEmpty(CircularQueue * Q)
{
  if (Q->front == Q->rear)
    return TRUE;
  else
    return FALSE;
}

/**
  * @brief  入队
  *
  * @param  Q: 目标队列指针
  * @param  x: 需要入队的数据
  *
  * @retval SUCCESS 执行成功
  * @retval ERROR   执行错误（队列已满）
  */
ErrorStatus CircularEnQueue(CircularQueue * Q, Queue_EleType x)
{
  if ((Q->rear + 1) % (Queue_Buffer_Size + 1) == Q->front)
    return ERROR;
  Q->Buffer[Q->rear] = x;
  Q->rear = (Q->rear + 1) % (Queue_Buffer_Size + 1);
  return SUCCESS;
}


/**
  * @brief  出队
  *
  * @param  Q: 目标队列指针
  * @param  x: 出队的数据存储位置指针
  *
  * @retval SUCCESS 执行成功
  * @retval ERROR   执行错误（队列已空）
  */
ErrorStatus CircularDeQueue(CircularQueue * Q, Queue_EleType * x)
{
  if (Q->front == Q->rear)
    return ERROR;
  *x = Q->Buffer[Q->front];
  Q->front = (Q->front + 1) % (Queue_Buffer_Size + 1);
  return SUCCESS;
}

/**
  * @}
  */

/**
  * @}
  */
