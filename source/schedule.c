#include "stdio.h"
#include "task.h"
#include "rpc.h"
#include "schedule.h"
#include <pthread.h>
#include "mylist.h"
#include "memory.h"
#include "my_log.h"
#include "test.h"
#include <unistd.h>

schedRes_t gschedRes;

schedRes_t *schedResGet(void)
{
    return &gschedRes;
}

static SchedStatus_t runTaskStep(task_t *pTask)
{
    SchedStatus_t status = SCHED_STATUS_PENDING;

    MY_BUG_ON(pTask->step >= pTask->pTOps->maxStep);
    MY_BUG_ON(pTask->pTOps->func[pTask->step] == NULL);
    
    if ((pTask->pTOps->maxStep > pTask->step) && (pTask->pTOps->func[pTask->step] != NULL)) {
        status = pTask->pTOps->func[pTask->step](pTask);
    } else {
        status = SCHED_STATUS_FINISH;
    }
    
    return status;
}

static void taskFinish(task_t *pTask)
{
    MY_BUG_ON(NULL == pTask);
    
    if (pTask->pTOps->finishFunc != NULL) {
        pTask->pTOps->finishFunc(pTask);
    }
    taskDestroy(pTask);
    return;
}

static void runTask(task_t *pTask)
{
    SchedStatus_t     status    = 0;
    
    do {
        status = runTaskStep(pTask);
    }while(SCHED_STATUS_CONTINUE == status);
    
    if (SCHED_STATUS_FINISH == status) {
        taskFinish(pTask);
    }
}

static void runRpc(rpc_t *pRpc)
{
    MY_BUG_ON(pRpc->pRops->pFunCb == NULL);
    
	pRpc->pRops->pFunCb(pRpc->pPrevData);
}

static void run(task_t *pTask)
{
	if (TASK == pTask->header.type) {
		runTask(pTask);
	} else if (RPC == pTask->header.type) {
		runRpc((rpc_t *)pTask);
	} else {
		MY_BUG();
	}
}

task_t *node2TaskAddr(struct list_head *pTaskNode)
{
    return (task_t *)list_entry(pTaskNode, SchedHeader_t, node);
}

static void *thdSchedule(void *arg)
{
    U8                needSched = 0;
    task_t           *pTask     = NULL;
    schedRes_t       *pSchedRes = schedResGet();
    pthread_mutex_t  *pLock     = &pSchedRes->taskLock;
    struct list_head *pTaskList = &pSchedRes->taskList;
    struct list_head *pTmpList  = NULL;
    struct list_head *pTaskNode = NULL;

    for (;;) {
        needSched = 1;
        pthread_mutex_lock(pLock);
        while (!list_empty(pTaskList)) {
            pTaskNode = del_item(pTaskList);
            pthread_mutex_unlock(pLock);
            pTask = node2TaskAddr(pTaskNode);

            MY_BUG_ON(pTask == NULL);
            needSched = 0;
            run(pTask);
            pthread_mutex_lock(pLock);
        }
        pthread_mutex_unlock(pLock);
        if (1 == needSched) {
            sleep(10);
        }
    }
}

S32 schedInit(U32 threadNum)
{
    U32 i  = 0;
    S32 rc = MY_SUCCESS;
    pthread_t tid;
    schedRes_t *pSchedRes = schedResGet();

    INIT_LIST_HEAD(&pSchedRes->taskList);
    rc = pthread_mutex_init(&pSchedRes->taskLock, NULL);
    if (rc != MY_SUCCESS) {
        MY_BUG();
    }
    
    for (i = 0; i < threadNum; i++) {
        rc = pthread_create(&tid, NULL, thdSchedule, NULL);
        if (rc != MY_SUCCESS) {
            MY_BUG(); 
        }  
    }
  
    return rc;
}

