#include "my_types.h"
#include "timer.h"
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include "my_time.h"
#include "my_log.h"

static mytimer_cfg_t gTimer;

mytimer_cfg_t *get_Timer(void)
{
    return &gTimer;
}

static void *timer_thread(void *arg)
{
    mytimer_cfg_t  *pTimer     = get_Timer();
    mytimer_item_t *pTimerItem = NULL;
    S64             current_time = 0;
    struct timespec time_wait;
    U32             time_delat   = 300;
    struct rb_node *pNode      = NULL;
    
    while(1) {
        MY_LOG("timer\n");
        current_time = get_current_time_us() + time_delat;
        time_wait.tv_sec  = US2SEC(current_time) + 1;
        time_wait.tv_nsec = US2NS(current_time%US2NS(1));
        pthread_mutex_lock(&pTimer->timer_mutex);
        pthread_cond_timedwait(&pTimer->timer_Cond, &pTimer->timer_mutex, &time_wait);
        pthread_mutex_unlock(&pTimer->timer_mutex);
        
        current_time = get_current_time_us();
        pthread_mutex_lock(&pTimer->timer_lock);
        while(MY_TIMER_STOP == pTimer->state){
            
        }
        while(NULL != pTimer->root.rb_node){
            pNode = rb_first(&pTimer->root);
            pTimerItem = container_of(pNode, mytimer_item_t, node);
            if (pTimerItem->time_stamp > current_time) {
                time_delat = pTimerItem->time_stamp - current_time;
                break;
            }
            MY_LOG("rbtree first node time : %llu\n", pTimerItem->time_stamp);
            rb_erase(pNode, &pTimer->root);
            pthread_mutex_unlock(&pTimer->timer_lock);
            if (pTimerItem->pfunc != NULL) {
                pTimerItem->pfunc(pTimerItem->pData);
            }
            current_time = get_current_time_us();
            pthread_mutex_lock(&pTimer->timer_lock);
        }
        pthread_mutex_unlock(&pTimer->timer_lock);
    }
}

S32 myTimer_item_add(mytimer_item_t *pTimer_item)
{
    S32 rc = 0;
    mytimer_cfg_t  *pTimer     = get_Timer();
    struct rb_node* parent;
    mytimer_item_t* tmp_stu;
    struct rb_node* tmp_rb;

    pthread_mutex_lock(&pTimer->timer_lock);
    rb_init_node(&pTimer_item->node);
    if (pTimer->root.rb_node == NULL) {
        MY_LOG("timer111111\n");
        pTimer->root.rb_node = &pTimer_item->node;
        rb_set_parent(pTimer->root.rb_node, NULL);
        rb_set_color(pTimer->root.rb_node, RB_BLACK);
        pthread_mutex_unlock(&pTimer->timer_lock);
        return 0;
    }
    tmp_rb = pTimer->root.rb_node;
    while(tmp_rb)
    {
        parent  = tmp_rb;
        tmp_stu = rb_entry(tmp_rb, mytimer_item_t, node);
        tmp_stu->time_stamp = 0;

        if (tmp_stu->time_stamp > pTimer_item->time_stamp) 
            tmp_rb = parent->rb_left;
        else if (tmp_stu->time_stamp < pTimer_item->time_stamp)
            tmp_rb = parent->rb_right;
        else
            break;
    }
    if (tmp_stu->time_stamp > pTimer_item->time_stamp)
        parent->rb_left = &(tmp_stu->node);
    else
        parent->rb_right = &(tmp_stu->node);

    rb_set_parent(&(tmp_stu->node), parent);
    rb_insert_color(&(tmp_stu->node), &pTimer->root);
    
    pthread_mutex_unlock(&pTimer->timer_lock);
    return 0;
}
S32 mytimer_Init(void)
{
    S32 rc                = MY_TRUE;
    mytimer_cfg_t *pTimer = get_Timer();
    pthread_t tid;

    rc = pthread_mutex_init(&pTimer->timer_mutex, NULL);
    if (rc != MY_SUCCESS) {
        
    }

    rc = pthread_cond_init(&pTimer->timer_Cond, NULL); 
    if (rc != MY_SUCCESS) {
        
    }

    rc = pthread_mutex_init(&pTimer->timer_lock, NULL);
    if (rc != MY_SUCCESS) {
        
    }
    pTimer->root.rb_node = NULL;
    pTimer->state = MY_TIMER_RUNNING;
    pTimer->pItem = NULL;
    
    pthread_create(&tid, NULL, timer_thread, NULL);

    return rc;
}

