#include "stdio.h"
#include "task.h"
#include "my_log.h"

SchedStatus_t schedTaskStepTest(struct task *pTask)
{
    MY_LOG("this is task step\n");
    return SCHED_STATUS_FINISH;
}

TaskOps_t gTest = {
    .maxStep = 1,
    .firstStep = 0,
    .finishFunc = NULL,
    .func = {
        [0] = schedTaskStepTest,
    },
};

void test()
{
    S32 rc = MY_SUCCESS;
    rc = taskCreatAndSubmit(NULL, &gTest);
	MY_LOG("this is a test,result %d\n", rc);
	
}
