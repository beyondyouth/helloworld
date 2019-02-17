#ifndef QUEUE_H_
#define QUEUE_H_


typedef struct{
    Mutex               *tableMutex;        // 表锁 
    uint32_t            item_len;          // 元素的固定长度 
    
    uint32_t            count;             // 已使用数 
    uint32_t            capacity;          // 承载容量 
    uint32_t            overflow_cnt;      // 溢出次数统计: 当加入元素而freelist为空时, 表示一次溢出

    uint32_t            front;             // 队首：指向队头元素
    uint32_t            rear;              // 队尾：指向队尾元素的”下一个“位置

    uint8_t             data_array[];      // 可变长的 散列头(指针型数组) 以及 数据存储区
}Queue_t;

class Queue
{
public:
    Queue(uint32_t capacity, uint32_t item_len);
    ~Queue();
    // 获取队列统计情况
    void            Queue_Statistics(uint32_t *capacity, uint32_t *count, uint32_t *overflow_cnt);
    uint32_t        Queue_GetSize   (const uint32_t capacity, const uint32_t item_len);
    Queue_t         *Queue_Make     (const uint32_t capacity, const uint32_t item_len);

    // 插入队尾
    int             Queue_Put       (const void *val, uint32_t vlen);
    // 从队首取
    void            *Queue_Get      (void *val, uint32_t vlen);

    uint32_t        Queue_Capacity  (void);
    uint32_t        Queue_Count     (void);
    void            Queue_Clear     (void);
private:
    Queue_t         *_ht = NULL;
    void            Queue_Lock      (void);
    void            Queue_Unlock    (void);
};

void Queue_TestSelf(void);

#endif /* QUEUE_H_ */