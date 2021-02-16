#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#include "my_types.h"

#define STM32_FILE_PATH ""
/*
1. 当收到uart中来的数据之后，创建rpc信息，发送rpc
2. 当收到rpc之后，创建task，然后挂入队列（在不同核上执行）
3. 执行task

1. 收到uart指令后可以创建task
2. 执行task
*/
typedef struct Stm32Cmd {
    U8 *pData;
    U8  len;
}Stm32Cmd_t;
#endif

