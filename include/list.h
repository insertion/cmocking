
typedef struct ListNode 
{
    const void *value;
    int refcount;
    // 表示该节点是否被引用
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

typedef struct SymbolMapValue
{
    const char *symbol_name;
    ListNode fn_values_list_head;
    // 不是指针,而是结构体对象,不完全的类型只能定义指针,而不能定义变量
} SymbolMapValue;


// Determines whether two values are the same. 1表示相等,0表示不想等
typedef int (*EqualityFunction)(const void *left, const void *right);
// Used by list_free() to deallocate values referenced by list nodes.
typedef void (*CleanupListValue)(const void *value, int has_child_list);
/*
    我觉得在写一个程序时,数据结构的耦合读不要太高,否则容易引起混乱,代码的可读性也不够好
*/

int list_find(const ListNode *const head, const void *const value, ListNode **output, EqualityFunction cmp);
ListNode *list_initialize(ListNode *const node);
int list_empty(const ListNode *const head);
ListNode *list_remove(ListNode *const node, CleanupListValue cleanup_value, int has_child_list);
void list_remove_free(ListNode *const node, CleanupListValue cleanup_value, int has_child_list);
void *list_free(ListNode *const head, CleanupListValue cleanup_value, int has_child_list);
int list_first(ListNode *const head, ListNode **output);
void list_add(ListNode *const head, ListNode *new_node);
ListNode *list_add_value(ListNode *const head, const void *value);