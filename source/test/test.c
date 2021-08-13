#include "stdio.h"
#include "task.h"
#include "my_log.h"

SchedStatus_t schedTaskStepTest1(struct task *pTask)
{
    MY_LOG("this is task step 1\n");
    taskStepSet(pTask, 1);
    return SCHED_STATUS_CONTINUE;
}
SchedStatus_t schedTaskStepTest2(struct task *pTask)
{
    MY_LOG("this is task step 2\n");
    return SCHED_STATUS_FINISH;
}

TaskOps_t gTest = {
    .pName   = "gTest",
    .maxStep = 2,
    .firstStep = 0,
    .finishFunc = NULL,
    .func = {
        [0] = schedTaskStepTest1,
        [1] = schedTaskStepTest2,
    },
};

void test()
{
    S32 rc = MY_SUCCESS;
    rc = taskCreatAndSubmit(NULL, &gTest);
	//MY_LOG("this is a test,result %d\n", rc);
	
}
