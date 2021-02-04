#ifndef __UPGRADE_H__
#define __UPGRADE_H__

/*
1. 当收到uart中来的数据之后，创建rpc信息，发送rpc
2. 当收到rpc之后，创建task，然后挂入队列（在不同核上执行）
3. 执行task

1. 收到uart指令后可以创建task
2. 执行task
*/

#endif

