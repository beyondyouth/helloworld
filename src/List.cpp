#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Common.h"
#include "Mutex.h"
#include "List.h"

#if 0
void List_TestSelf(void)
{
    int ii;
    
    List test = List();

    // 增加/删除/修改
    

    for (ii=0; ii < 100; ii++)
    {
        LNode_t *s = (LNode_t *)malloc(sizeof(LNode_t));

        s->data = ii;
        
        test.List_Insert_After(test.List_GetHead(), s);
        
    }
    for(LNode_t *p = test.List_GetHead(); p->next != NULL; p = p->next)
    {
        printf("---%d---\n", p->next->data);
    }
    // 统计
    test.List_Clear();
    printf("-----len:%d-----\n", test.List_GetLength());
}
#endif

List::List()
{
    _ht = List_Make();
}

List::~List()
{
    List_Clear();
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

LNode_t* List::List_GetHead(void)
{
    return _ht->head;
}

LNode_t* List::List_GetTail(void)
{
    return _ht->tail;
}

// 删除p节点，并返回它的数据元素
int List::List_Delete(LNode_t *p, LData_t *val)
{
    int rtnValue = -1;

    if(NULL == p)
    {
        return rtnValue;
    }

    if(p == _ht->head)
    {
        return rtnValue;
    }

	List_Lock();
	
    if(NULL != val)
    {
        *val = p->data;
    }
    
    if(p == _ht->tail)
    {
        p->prior->next = p->next;
        _ht->tail = p->prior;
    }
    else
    {
        p->prior->next = p->next;
	    p->next->prior = p->prior;
    }
    
	free(p);
    _ht->length--;
    List_Unlock();
    rtnValue = 0;
    
    return rtnValue;
}

// 在p节点之前插入s节点
int List::List_Insert_Before(LNode_t *p, LNode_t *s)
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
    if(p == _ht->head)
    {
        return rtnValue;
    }

    List_Lock();

	s->prior = p->prior;
	p->prior->next = s;
	s->next = p;
	p->prior = s;
    _ht->length++;
    List_Unlock();
    rtnValue = 0;

    return rtnValue;
}


int List::List_Insert_After(LNode_t *p, LNode_t *s)
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

    s->next = p->next;
    if(p != _ht->tail)
    {
        p->next->prior = s;
    }
    s->prior = p;
    p->next = s;

    if(p == _ht->tail)
    {
        _ht->tail = s;
    }

    _ht->length++;
    List_Unlock();

    rtnValue = 0;
    return rtnValue;
}

uint32_t List::List_GetLength(void)
{
    return _ht->length;
}

void List::List_Clear(void)
{
    while(NULL != _ht->headNode.next)
    {
        List_Delete(_ht->headNode.next, NULL);
    }
}

