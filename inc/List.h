#ifndef LIST_H_
#define LIST_H_

typedef struct LData{
    int x;
    int y;
    dir d;
}LData_t;

typedef struct LNode{
    LData_t data;
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
    
    LNode_t          *List_GetHead   (void);
    LNode_t          *List_GetTail   (void);

    // 在p节点之前插入s节点
    int             List_Insert_Before  (LNode_t *p, LNode_t *s);
    // 在p节点之后插入s节点
    int             List_Insert_After   (LNode_t *p, LNode_t *s);
    // 删除p节点，返回data
    int             List_Delete     (LNode_t *p, LData_t *val);

    uint32_t        List_GetLength  (void);
    void            List_Clear      (void);
private:
    List_t          *_ht = NULL;
    void            List_Lock       (void);
    void            List_Unlock     (void);

    List_t         *List_Make       (void);
};
#if 0
void List_TestSelf(void);
#endif
#endif /* LIST_H_ */
