#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Mutex.h"
#include "Queue.h"


//#define MAC_TEST_MAX            100000000
#define MAC_TEST_MAX            10000
#if 1
static char static_special_padding_sureOnce[] = "__kkae__iobler__bmkiwelq__";

//se:       #define CAVIUM_SE
//linux-64: #define CAVIUM_LINUX
//linux-32:

#ifdef CAVIUM_LINUX
#   define PTR_TO_SE(type, ptr)    ((NULL != ptr)?((type)((uint64)((char*)(ptr)) & 0X7FFFFFFFFFFFFFFF)):NULL)
#   define PTR_FROM_SE(type, ptr)  ((NULL != ptr)?((type)((uint64)((char*)(ptr)) | 0X8000000000000000)):NULL)
#else
#   define PTR_TO_SE(type, ptr)    ptr
#   define PTR_FROM_SE(type, ptr)  ptr
#endif
#endif

void Queue::Queue_TestSelf(void)
{
    Queue_t *ht;
    unsigned int item_len, capacity, count, overflow;
    void* memory;
    char testkey[32];
    char testvalue[32];
    char value_buf[1024];
    int ii;

    capacity = 1000;
    item_len = 64;
    memory = malloc(Queue_GetSize(capacity, item_len));

    // 同SE共享内存时, 则请预先分配, 然后将第一个参数(指针)指向已分配地址即可
    ht = Queue_Make(memory, capacity, item_len);

    // 增加/删除/修改
    Queue_Put(ht, "foo1", 8);
    Queue_Put(ht, "foo1", 8);
    Queue_Put(ht, "foo2", 8);
    Queue_Put(ht, "foo3", 8);

    //{
    //    const char *val = Queue_Get(ht, value_buf, sizeof(value_buf));
    //    printf("val is %s\n", val);
    //}

    for (ii=0; ii < MAC_TEST_MAX; ii++)
    {
#ifdef WIN32
#pragma warning(disable:4996)
#endif 
        sprintf(testkey, "foo%d", ii);
        sprintf(testvalue, "FOO%d", ii);
#ifdef WIN32
#pragma warning(default:4996)
#endif 

        Queue_Put(ht, testkey, strlen(testkey));
        
        if (0 == ii%10)
        {
            Queue_Get(ht, value_buf, sizeof(value_buf));
        }

        //if (0 == ii%100000)
        //{
        //    printf("-");
        //}
    }

    //while(NULL != Queue_Get(ht, value_buf, sizeof(value_buf)))
    //{
    //    printf("%s \n", value_buf);
    //}

    //Queue_Unlock(ht);

    // 统计
	Queue_Statistics(ht, &capacity, &count, &overflow);
    Queue_Clear(ht);
	Queue_Destroy(ht);
    free(memory);
}


uint32_t Queue::Queue_GetSize(const uint32_t capacity, 
                           const uint32_t item_len)
{
    return sizeof(Queue_t)    // base info
           + (capacity+1) * item_len;
}


Queue_t * Queue::Queue_Make(void *pool,
                     const uint32_t capacity,
                     const uint32_t item_len)
{
    Queue_t *ht = NULL;
    if(NULL == pool)
    {
        return NULL;
    }
    if(sizeof(static_special_padding_sureOnce) >= QUEUE_PADDING_32)
    {
        return NULL;
    }

    ht = (Queue_t *)pool;
#ifdef CAVIUM_LINUX
    if(0 != memcmp(ht->padding, static_special_padding_sureOnce, sizeof(static_special_padding_sureOnce)))
    {
        printf("MAGIC STRING of Queue_Make in linux ERROR. addr: 0x%p \n", ht->padding);
        return NULL;
    }
#else    
    memcpy(ht->padding, static_special_padding_sureOnce, sizeof(static_special_padding_sureOnce));

    Queue_Lock(ht);

    // 如果外部已分配好内存, 则直接使用即可
    if(pool) ht = (Queue_t *)pool;
    else
    {
        ht = (Queue_t *)malloc(Queue_GetSize(capacity,item_len));
    }

    ht->tableMutex = new Mutex();
    ht->item_len = item_len;
    ht->count = 0;
    ht->overflow_cnt = 0;
    ht->capacity = capacity+1;
#endif

    return ht;
}


void Queue::Queue_Lock(Queue_t *ht)
{
    ht->tableMutex->lock();
}

void Queue::Queue_Unlock(Queue_t *ht)
{
    ht->tableMutex->unlock();
}


/** 从队列中提取头元素 */
void* Queue::Queue_Get(Queue_t *ht, void *val, uint32_t vlen)
{
    char* rtnValue = NULL;
    
    if(NULL == ht)
    {
        return rtnValue;
    }
    if(NULL == val)
    {
        return rtnValue;
    }
    if(!vlen)
    {
        return rtnValue;
    }

    
    Queue_Lock(ht);

    if(ht->front == ht->rear) // 队列空
    {
        rtnValue = NULL;
    }
    else
    {
        memcpy(val, ht->data_array+ht->front*ht->item_len, 
            vlen <= ht->item_len ? vlen : ht->item_len);
        
        ht->front=(ht->front+1)%ht->capacity;

        rtnValue = (char *)val;
    }
    Queue_Unlock(ht);

    return rtnValue;
}

/** 插入到队尾 */
int Queue::Queue_Put(Queue_t *ht, const void *val, uint32_t vlen)
{
    int rtnValue = -1;

    if(NULL == ht)
    {
        return rtnValue;
    }
    if(NULL == val)
    {
        return rtnValue;
    }
    if(!vlen)
    {
        return rtnValue;
    }

    Queue_Lock(ht);

    if((ht->rear+1) % ht->capacity == ht->front) // 队列满 
    {
        ht->overflow_cnt++;
        rtnValue = -1;
    }
    else
    {
        memcpy(ht->data_array + ht->rear * ht->item_len, val, 
            vlen <= ht->item_len ? vlen : ht->item_len);

        ht->rear = (ht->rear+1) % ht->capacity;
        
        rtnValue = 0;
    }

    Queue_Unlock(ht);

    return rtnValue;
}

void Queue::Queue_Statistics(Queue_t *ht,
                           uint32_t *capacity,
                           uint32_t *count, 
                           uint32_t *overflow_cnt)
{
    printf("\n=============hash table statistics===============\n");

    *capacity       = Queue_Capacity(ht);
    *count          = Queue_Count(ht);
    *overflow_cnt   = ht->overflow_cnt  ;

    printf("capacity=%d count=%d overflow_cnt=%d", 
        *capacity, *count, *overflow_cnt);

    printf("\n=============hash statistics end===============\n");
}

uint32_t Queue::Queue_Capacity(Queue_t *ht)
{
    return ht->capacity-1;
}

uint32_t Queue::Queue_Count(Queue_t *ht)
{
    return(ht->rear-ht->front+ht->capacity)%ht->capacity;
}

void Queue::Queue_Clear(Queue_t *ht)
{
	Queue_Lock(ht);
    ht->front = ht->rear = 0;
	Queue_Unlock(ht);
}

void Queue::Queue_Destroy(Queue_t *ht)
{
    if(NULL != ht)
    {
	    delete(ht->tableMutex);
    }
}
