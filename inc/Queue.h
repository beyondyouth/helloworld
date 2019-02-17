#ifndef QUEUE_H_
#define QUEUE_H_

#define QUEUE_PADDING_32 32

typedef struct{
    Mutex               *tableMutex;        // 表锁 
    uint32_t            item_len;          // 元素的固定长度 
    
    uint32_t            count;             // 已使用数 
    uint32_t            capacity;          // 承载容量 
    uint32_t            overflow_cnt;      // 溢出次数统计: 当加入元素而freelist为空时, 表示一次溢出

    uint32_t            front;             // 队首
    uint32_t            rear;              // 队尾

    char                padding [QUEUE_PADDING_32];

    uint8_t             data_array[];      // 可变长的 散列头(指针型数组) 以及 数据存储区
}Queue_t;

class Queue
{
public:
    void            Queue_TestSelf  (void);
    // 获取队列统计情况
    void            Queue_Statistics(Queue_t *ht,
                                    uint32_t *capacity,
                                    uint32_t *count, 
                                    uint32_t *overflow_cnt);
    uint32_t        Queue_GetSize   (const uint32_t capacity, const uint32_t item_len);
    Queue_t         *Queue_Make     (void *pool, 
                                    const uint32_t capacity, 
                                    const uint32_t item_len);
    void            Queue_Lock      (Queue_t *ht);
    void            Queue_Unlock    (Queue_t *ht);

    // 插入队尾
    int             Queue_Put       (Queue_t *ht, const void *val, uint32_t vlen);
    // 从队首取
    void            *Queue_Get      (Queue_t *ht, void *val, uint32_t vlen);

    uint32_t        Queue_Capacity  (Queue_t *ht);
    uint32_t        Queue_Count     (Queue_t *ht);
    void            Queue_Clear     (Queue_t *ht);
    void            Queue_Destroy   (Queue_t *ht);
};


#endif /* QUEUE_H_ */