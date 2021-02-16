#include "upgrade.h"
#include "task.h"
#include "my_types.h"
#include "my_memory.h"

void stm32CmdFill(U8 cmd, U8 *pCmd, U8 *pData, U8 dataLen)
{

}

U32 readFwLength()
{
    FILE *pFwFile = NULL;
    U8    readLen = 0;
    
    pFwFile = fopen(STM32_FILE_PATH"//application.bin", "rb");
    if (pFwFile <= 0) {
        goto end;
    }
    fseek(pFwFile, 0L, SEEK_END);
    readLen = ftell(pFwFile);
end:
    fclose(pFwFile);
    return readLen;
}

SchedStatus_t readFileLen(task_t *pTask)
{
    U32          fwLen   = 0;
    FILE        *pFwFile = NULL;
    Stm32Cmd_t  *pCmd    = NULL;

    pCmd = (Stm32Cmd_t *)my_zalloc(sizeof(*pCmd), __FUNCTION__);
    if (NULL == pCmd) {
        MY_BUG();
    }

    pCmd->pData = (U8 *)my_zalloc(25, __FUNCTION__);
    fwLen = readFwLength();
    if (fwLen == 0) {
        MY_BUG();
    }
    
    stm32CmdFill(0x21, pCmd->pData, (U8 *)(&fwLen), pCmd->len);
    
    taskPrevDataSet(pTask, (void *)pCmd);
    return SCHED_STATUS_CONTINUE;
}

void uartSndData(U8 *pData, U8 len)
{

}

SchedStatus_t sndCmdToStm32(task_t *pTask)
{
    Stm32Cmd_t   *pCmd    = (Stm32Cmd_t *)getTaskPrevData(pTask);
    
    uartSndData(pCmd->pData, pCmd->len);
    
    return SCHED_STATUS_FINISH;
}

void sndCmdFinish(task_t *pTask)
{
    Stm32Cmd_t   *pCmd    = (Stm32Cmd_t *)getTaskPrevData(pTask);

    my_free((U64 *)pCmd->pData);
    my_free((U64 *)pCmd);
}

TaskOps_t fileLen = {
    .maxStep = 1,
    .firstStep = 0,
    .pName = "file len",
    .finishFunc = sndCmdFinish,
    .func = {
        readFileLen,
        sndCmdToStm32,
    },
};

TaskOps_t *getFileLenTask()
{
    return &fileLen;
}

U32 readFwData(U32 addr, U8 len, U8 *pReadData)
{
    FILE *pFwFile = NULL;
    U8    readLen = 0;
    
    pFwFile = fopen(STM32_FILE_PATH"//application.bin", "rb");
    if (pFwFile <= 0) {
        goto end;
    }
    fseek(pFwFile, addr, SEEK_SET);
    readLen = fread(pReadData ,1, 128, pFwFile);
end:
    fclose(pFwFile);
    return readLen;
}

SchedStatus_t sndFwPackToStm32(task_t *pTask)
{
    U16  pack = 0;
    U32  len  = 0;
    S32  rc   = 0;
    U8  *pStm32Cmd   = NULL;
    U8  *pData       = NULL;
    Stm32Cmd_t *pCmd = NULL;

    pCmd = (Stm32Cmd_t *)my_zalloc(sizeof(*pCmd), __FUNCTION__);
    if (NULL == pCmd) {
        MY_BUG();
    }
    
    pData = (U8 *)my_zalloc(128, __FUNCTION__);
    if (NULL == pData) {
        MY_BUG();
    }
    
    pack = pStm32Cmd[3];
    pack = (pack<<8)|pStm32Cmd[4];
    len  = pack*128;
    
    pCmd->len = readFwData(len, 128, pData);
    if (rc == 0) {
        MY_BUG();
    }

    pCmd->pData = (U8 *)my_zalloc(pCmd->len+3, __FUNCTION__);
    if (NULL == pData) {
        MY_BUG();
    }
    
    stm32CmdFill(0x22, pCmd->pData, pData, pCmd->len);
    return SCHED_STATUS_CONTINUE;
}

TaskOps_t sndPack = {
    .maxStep = 1,
    .firstStep = 0,
    .pName = "snd pack",
    .finishFunc = sndCmdFinish,
    .func = {
        sndFwPackToStm32,
        sndCmdToStm32,
    },
};

TaskOps_t *getSndPackTask()
{
    return &sndPack;
}

