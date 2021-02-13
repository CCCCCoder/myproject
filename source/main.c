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
    U8 i;
    sleep(1);
	while(1){
		MY_LOG("hello thread!!\n");
        for (i = 0; i<255; i++) {
            test();
        }

		sleep(1);
	}
}
// 串口 spi接flash 
int main()
{
	pthread_t tid;
	U8 i;
	libtest();
	schedInit();
	//mytimer_Init();
	pthread_create(&tid, NULL, test_thread, NULL);

	for(;;){
		sleep(10);
	}
	//pthread_cancel(tid);
	//pthread_join(tid,NULL);
}
