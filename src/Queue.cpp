#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Mutex.h"
#include "Queue.h"


void Queue_TestSelf(void)
{
    uint32_t item_len, capacity, count, overflow;
    char testkey[32];
    char testvalue[32];
    char value_buf[1024];
    int ii;

    capacity = 1000;
    item_len = 64;
    
    Queue test = Queue(capacity, item_len);

    // 增加/删除/修改
    test.Queue_Put("foo1", 8);
    test.Queue_Put("foo1", 8);
    test.Queue_Put("foo2", 8);
    test.Queue_Put("foo3", 8);

    for (ii=0; ii < 10000; ii++)
    {
        sprintf(testkey, "foo%d", ii);
        sprintf(testvalue, "FOO%d", ii);

        test.Queue_Put(testkey, strlen(testkey));
        
        if (0 == ii%10)
        {
            test.Queue_Get(value_buf, sizeof(value_buf));
        }
        
    }

    // 统计
	test.Queue_Statistics(&capacity, &count, &overflow);
    test.Queue_Clear();
}


Queue::Queue(uint32_t capacity, uint32_t item_len)
{
    _ht = Queue_Make(capacity, item_len);
}

Queue::~Queue()
{
    if(NULL != _ht)
    {
	    delete(_ht->tableMutex);
    }
    if(NULL != _ht)
    {
        free(_ht);
    }
}


uint32_t Queue::Queue_GetSize(const uint32_t capacity, 
                           const uint32_t item_len)
{
    return sizeof(Queue_t)    // base info
           + (capacity+1) * item_len;
}


Queue_t * Queue::Queue_Make(const uint32_t capacity, const uint32_t item_len)
{
    Queue_t *ht = NULL;

    ht = (Queue_t *)malloc(Queue_GetSize(capacity,item_len));

    ht->tableMutex = new Mutex();
    ht->item_len = item_len;
    ht->count = 0;
    ht->overflow_cnt = 0;
    ht->capacity = capacity+1;

    ht->front = 0;
    ht->rear = 0;

    return ht;
}


void Queue::Queue_Lock(void)
{
    _ht->tableMutex->lock();
}

void Queue::Queue_Unlock(void)
{
    _ht->tableMutex->unlock();
}


/** 从队列中提取头元素 */
void* Queue::Queue_Get(void *val, uint32_t vlen)
{
    char* rtnValue = NULL;
    
    if(NULL == val)
    {
        return rtnValue;
    }
    if(!vlen)
    {
        return rtnValue;
    }
    
    Queue_Lock();

    if(_ht->front == _ht->rear) // 队列空
    {
        rtnValue = NULL;
    }
    else
    {
        memcpy(val, _ht->data_array+_ht->front*_ht->item_len, 
            vlen <= _ht->item_len ? vlen : _ht->item_len);
        
        _ht->front=(_ht->front+1)%_ht->capacity;

        rtnValue = (char *)val;
    }
    Queue_Unlock();

    return rtnValue;
}

/** 插入到队尾 */
int Queue::Queue_Put(const void *val, uint32_t vlen)
{
    int rtnValue = -1;

    if(NULL == val)
    {
        return rtnValue;
    }
    if(!vlen)
    {
        return rtnValue;
    }

    Queue_Lock();

    if((_ht->rear+1) % _ht->capacity == _ht->front) // 队列满 
    {
        _ht->overflow_cnt++;
        rtnValue = -1;
    }
    else
    {
        memcpy(_ht->data_array + _ht->rear * _ht->item_len, val, 
            vlen <= _ht->item_len ? vlen : _ht->item_len);

        _ht->rear = (_ht->rear+1) % _ht->capacity;
        
        rtnValue = 0;
    }

    Queue_Unlock();

    return rtnValue;
}

void Queue::Queue_Statistics(uint32_t *capacity, uint32_t *count, uint32_t *overflow_cnt)
{
    printf("\n=============hash table statistics===============\n");

    *capacity       = Queue_Capacity();
    *count          = Queue_Count();
    *overflow_cnt   = _ht->overflow_cnt  ;

    printf("capacity=%d count=%d overflow_cnt=%d", 
        *capacity, *count, *overflow_cnt);

    printf("\n=============hash statistics end===============\n");
}

uint32_t Queue::Queue_Capacity(void)
{
    return _ht->capacity-1;
}

uint32_t Queue::Queue_Count(void)
{
    return(_ht->rear-_ht->front+_ht->capacity)%_ht->capacity;
}

void Queue::Queue_Clear(void)
{
	Queue_Lock();
    _ht->front = _ht->rear = 0;
	Queue_Unlock();
}

