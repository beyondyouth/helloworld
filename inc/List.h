#ifndef LIST_H_
#define LIST_H_

typedef struct LNode{
    int data;
    struct LNode *prior;
    struct LNode *next;
}LNode_t;

typedef struct{
    Mutex               *tableMutex;       // 表锁 
    
    uint32_t            length;            // 已使用数

    LNode_t             headNode;          // 队首：指向队头元素
    LNode_t             *head;
    LNode_t             *tail;              // 队尾：指向队尾元素的”下一个“位置
}List_t;

class List
{
public:
    List();
    ~List();
    
    List_t         *List_Make       ();

    // 插入队尾
    int             List_Insert     (LNode_t *p, LNode_t *s);
    // 从队首取
    int             List_Delete     (LNode_t *p, uint32_t *val);

    uint32_t        List_GetLength  (void);
    void            List_Clear      (void);
private:
    List_t          *_ht = NULL;
    void            List_Lock      (void);
    void            List_Unlock    (void);
};
#if 0
void List_TestSelf(void);
#endif
#endif /* LIST_H_ */
