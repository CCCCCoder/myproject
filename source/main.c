#include "stdio.h"
#include "schedule.h"
#include "test.h"
#include "libtest.h"
#include <pthread.h>
#include "my_memory.h"
#include "timer.h"
#include "my_time.h"
#include "my_log.h"

static void *test_thread(void *arg)
{
    mytimer_cfg_t *pTimer = get_Timer();
    mytimer_item_t *ptimerItem;
    
    ptimerItem = my_alloc(sizeof(mytimer_item_t));
    ptimerItem->time_stamp = get_current_time_us()+SEC2US(10);
    ptimerItem->pfunc = NULL;
    MY_LOG("timer.time_stamp: %llu\n",ptimerItem->time_stamp);
    myTimer_item_add(ptimerItem);

	while(1){
		MY_LOG("hello thread!!\n");
		pthread_testcancel();
		sleep(1);
	}
}
// 串口 spi接flash 
int main()
{
	//pthread_t tid;
	U8 i;
	libtest();
	//mytimer_Init();
	//pthread_create(&tid, NULL, test_thread, NULL);
	schedInit();
	for (i = 0; i<255; i++) {
	    test();
	}
	while(1);
	sleep(10);
	//pthread_cancel(tid);
	//pthread_join(tid,NULL);
}
