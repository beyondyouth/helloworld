#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Mutex.h"
#include "List.h"

#if 0
void List_TestSelf(void)
{
    uint32_t item_len, capacity, count, overflow;
    char testkey[32];
    char testvalue[32];
    char value_buf[1024];
    int ii;

    capacity = 1000;
    item_len = 64;
    
    List test = List(capacity, item_len);

    // 增加/删除/修改
    test.List_Put("foo1", 8);
    test.List_Put("foo1", 8);
    test.List_Put("foo2", 8);
    test.List_Put("foo3", 8);

    for (ii=0; ii < 10000; ii++)
    {
        sprintf(testkey, "foo%d", ii);
        sprintf(testvalue, "FOO%d", ii);

        test.List_Put(testkey, strlen(testkey));
        
        if (0 == ii%10)
        {
            test.List_Get(value_buf, sizeof(value_buf));
        }
        
    }

    // 统计
	test.List_Statistics(&capacity, &count, &overflow);
    test.List_Clear();
}
#endif

List::List()
{
    _ht = List_Make();
}

List::~List()
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

List_t * List::List_Make()
{
    List_t *ht = NULL;

    ht = (List_t *)malloc(sizeof(List_t));

    ht->tableMutex = new Mutex();
    ht->length = 0;

    ht->headNode.prior = NULL;
    ht->headNode.next = NULL;

	ht->head = &ht->headNode;
	ht->tail = &ht->headNode;

    return ht;
}


void List::List_Lock(void)
{
    _ht->tableMutex->lock();
}

void List::List_Unlock(void)
{
    _ht->tableMutex->unlock();
}


// 删除p节点，并返回它的数据元素
int List::List_Delete(LNode_t *p, uint32_t *val)
{
    int rtnValue = -1;

    if(NULL == p)
    {
        return rtnValue;
    }
	List_Lock();
	
    if(NULL != val)
    {
        *val = p->data;
    }

	p->prior->next = p->next;
	p->next->prior = p->prior;
	free(p);
    List_Unlock();
    rtnValue = 0;
    
    return rtnValue;
}

// 在p节点之前插入s节点
int List::List_Insert(LNode_t *p, LNode_t *s)
{
    int rtnValue = -1;

    if(NULL == p)
    {
        return rtnValue;
    }
    if(NULL == s)
    {
        return rtnValue;
    }

    List_Lock();

	s->prior = p->prior;
	p->prior->next = s;
	s->next = p;
	p->prior = s;

    List_Unlock();

    return rtnValue;
}


uint32_t List::List_GetLength(void)
{
    return _ht->length;
}

void List::List_Clear(void)
{
	List_Lock();
    while(NULL != _ht->headNode.next)
    {
        List_Delete(_ht->headNode.next, NULL);
        _ht->headNode.next = _ht->headNode.next->next;
    }
	List_Unlock();
}

