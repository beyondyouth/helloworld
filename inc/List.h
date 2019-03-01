#ifndef LIST_H_
#define LIST_H_

typedef struct LNode{
    int data;
    struct LNode *prior;
    struct LNode *next;
}LNode_t;

typedef struct{
    Mutex               *tableMutex;       // 表锁 
    
    uint32_t            length;            // 表长

    LNode_t             headNode;          // 头节点
    LNode_t             *head;             // 头指针
    LNode_t             *tail;             // 尾指针
}List_t;

class List
{
public:
    List();
    ~List();
    
    List_t         *List_Make       ();

    // 在p节点之前插入s节点
    int             List_Insert     (LNode_t *p, LNode_t *s);
    // 删除p节点，返回data
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
