#ifndef __RPC_H__
#define __RPC_H__

#include "my_types.h"
#include "schedule.h"
#include "mylist.h"
#include <pthread.h>
#include "my_log.h"
#include <string.h>
#include "my_memory.h"
#include "task.h"

typedef void (*schedRpcFunc)(void *pData);

///< rpc消息结构体
typedef struct RpcOps{
    const char *pName;
    schedRpcFunc pFunCb;
}RpcOps_t;

typedef struct rpc{
	SchedHeader_t header;
	RpcOps_t     *pRops;
	void         *pPrevData;
}rpc_t;

#endif
