
#include <assert.h>
#include <stdlib.h>
#include <list.h>
// 模块化设计很重要
// c语言的内存管理真的很烦!!!
// 在清除节点的同时,应该清除节点内的指针,所以节点的数据结构,最好有个flag表示哪些些指针需要清理
// void* 类型真的是又爱又恨
// 从双向链表中的任意一个结点开始，都可以很方便地访问它的前驱结点和后继结点。一般我们都构造双向循环链表。
// 默认head节点的value为NULL
// Doubly linked list node.


//从一个链表中查找值,1表示找到,0表示没找到
int list_find(const ListNode *const head, const void *const value, ListNode **output, EqualityFunction cmp)
{
    ListNode *current = head->next;
    while (current != head)
    {
        if (cmp(current->value, value))
        {
            *output = current;
            return 1;
        }
        else
        {
            current = current->next;
        }
    }
    return 0;
}

// Initialize a list node.
ListNode *list_initialize(ListNode *const node)
{
    node->value = NULL;
    node->next = node;
    node->prev = node;
    node->refcount = 1;
    return node;
}

// determine whether a list is empty
int list_empty(const ListNode *const head)
{
    // 判断head是否是空指针
    assert_true(head);
    return (head->next == head);
}

// Used by list_free() to deallocate values referenced by list nodes.
// 节点的value有两种,一种就是普通的value,直接free掉就好了,另一种是value里还包含一个list
// Deallocate a value referenced by a list node.
static void free_value(const void *value, int has_child_list)
{
    // 在本程序中节点的value类型只有两种
    assert_true(value);
    if (!has_child_list)
        free((void *)value);
    else
    {
        SymbolMapValue *const map_value = (SymbolMapValue *)value;
        list_free(&map_value->fn_values_list_head, free_value, 0);
        free(map_value);
    }
}
// Remove a node from a list.
// 是否要free掉value,由调用者决定,如何free掉value由调用者决定
ListNode *list_remove(ListNode *const node, CleanupListValue cleanup_value, int has_child_list)
{
    assert_true(node);
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = NULL;
    node->next = NULL;
    // 切断节点和原链表的联系后再返回节点
    // 这就是为什么不在remove之前提取node的原因
    // 这里为什么不立即free掉空间呢?可能是因为节点的value也需要free
    // 必须先free掉节点value才能接着free节点本身
    if (cleanup_value)
    {
        //如果存在清除value的函数
        cleanup_value(node->value, has_child_list);
    }
    return node;
    //
}

/* Remove a list node from a list and free the node. */
void list_remove_free(ListNode *const node, CleanupListValue cleanup_value, int has_child_list)
{
    /* 释放一个节点的内存不仅仅要释放当前节点本身的内存
     * 也要把节点内value的内存释放掉
     * 否则会出现内存泄漏,如果value也是一个list,要迭代释放
     */
    free(list_remove(node, cleanup_value, has_child_list));
}
void *list_free(ListNode *const head, CleanupListValue cleanup_value, int has_child_list)
{
    assert_true(head);
    while (!list_empty(head))
    {
        // 删除一个链表,从头开始依次删除每个节点
        list_remove_free(head->next, cleanup_value, has_child_list);
    }
}

// get the first node of a list,采用值结果参数
int list_first(ListNode *const head, ListNode **output)
{
    assert_true(head);
    if (list_empty(head))
    {
        return 0;
    }
    *output = head->next;
    list_remove(head->next,0,0);
    // 从链表中移除该节点
    return 1;
}

void list_add(ListNode *const head, ListNode *new_node)
{
    // 采用尾插法,插在head之前
    assert_true(head);
    assert_true(new_node);
    head->prev->next = new_node;
    new_node->prev = head->prev;
    head->prev = new_node;
    new_node->next = head;
    //return ;
}

/* Adds a value at the tail of a given list.
 * The node referencing the value is allocated from the heap. */
ListNode *list_add_value(ListNode *const head, const void *value)
{
    // 这里的value类型由外部传入
    ListNode *const new_node = (ListNode *)malloc(sizeof(ListNode));
    // list结构在这里使用malloc函数 分配空间,由list开辟的空间也应该由list销毁
    assert_true(head);
    assert_true(value);
    new_node->value = value;
    new_node->refcount = 1;
    list_add(head, new_node);
    // 返回一个已经加入链表的node
    return new_node;
}
