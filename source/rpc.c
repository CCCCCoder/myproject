#include "rpc.h"
#include "task.h"
#include "my_types.h"
#include "schedule.h"
#include "mylist.h"
#include <pthread.h>
#include "my_log.h"
#include <string.h>
#include "my_memory.h"

S32 rpcSubmitToFifo(rpc_t *pRpc)
{
    S32 rc = MY_SUCCESS;
    schedRes_t       *pSchedRes = schedResGet();
    pthread_mutex_t  *pLock     = &pSchedRes->taskLock;
    struct list_head *pTaskList = &pSchedRes->taskList;

    MY_BUG_ON(NULL == pSchedRes);

 	pthread_mutex_lock(pLock);
	list_add_tail(&pRpc->header.node,pTaskList);
	pthread_mutex_unlock(pLock);   
    return rc;
}

rpc_t *rpcCreat(void *pPrivData, RpcOps_t *pOps)
{
    rpc_t *pRpc = 0;

    pRpc = (rpc_t *)my_zalloc(sizeof(*pRpc), __FUNCTION__);
    if (NULL == pRpc) {
        goto end;
    }
    memset((void *)pRpc, 0, sizeof(*pRpc));
    pRpc->pPrevData = pPrivData;
    pRpc->pRops     = pOps;
	pRpc->header.type = RPC;

end:
    return pRpc;
}

void rpcDestroy(rpc_t *pRpc)
{
    if (NULL != pRpc) {
        my_free((U64 *)pRpc);
    }
    
    return;
}

S32 rpcCreatAndSubmit(void *pPrivData, RpcOps_t *pOps)
{
    rpc_t  *pRpc = NULL;
    S32     rc    = MY_SUCCESS;

    pRpc = rpcCreat(pPrivData, pOps);
    if (NULL == pRpc) {
        rc = -1;
        goto end;
    }

    rc = rpcSubmitToFifo(pRpc);
    if (MY_SUCCESS != rc) {
        rpcDestroy(pRpc);
    }
end:
    return rc;
}

