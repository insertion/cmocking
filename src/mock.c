
#include <string.h>
#include <list.h>
#include <io.h>
#include <stdlib.h>

#define LargeINT unsigned long long

/* Contains a list of values for a symbol.
 * NOTE: Each structure referenced by symbol_values_list_head must have a
 * SourceLocation as its' first member.
 */
// 定义个全局变量,value保存的是SymbolMapValue结构类型,双层链表的第一层
static ListNode global_function_result_map_head;

/* Determine whether a symbol name referenced by a symbol_map_value
 * matches the specified function name. */
int symbol_names_match(const void *map_value, const void *symbol)
{
    return !strcmp(((SymbolMapValue *)map_value)->symbol_name, (const char *)symbol);
}

/* 
 * The value is returned as an output parameter
 * return 1 if a value is found, 0 otherwise.
 */
int get_symbol_node(ListNode *head, const char *const symbol, void **output)
{
    ListNode *current;
    for (current = head; current != NULL; current = current->next)
    {
        if (symbol_names_match(current->value, symbol))
        {
            *output = current;
            return 1;
            //
        }
    }
    return 0;
}
// 一般是在测试用例中调用mock函数,首先调用will_return
// willreturn 和mock 开辟出来的链表,应该在teardown的时候清除
LargeINT _mock(const char *const function_name, const char *const file, int line)
{
    // mock 返回的是值的地址
    // 已每个函数为链表名,把返回值存在里面
    // 不考虑mock返回值的free问题
    ListNode *function_node;
    ListNode *head;
    ListNode *return_node;
    SymbolMapValue *fn_map_values;
    LargeINT return_value;
    //  值结果参数
    const int rc = get_symbol_node(&global_function_result_map_head, function_name, &function_node);
    if (rc)
    {
        fn_map_values = (SymbolMapValue *)function_node->value;
        //fn_values_list->symbol_name == function_name
        head = &fn_map_values->fn_values_list_head ;
        if (list_first(head, &return_node))
        {
            // 提取链表的第一个值,这个时候不能free啊!!!!!!,那什么时候free
            return_value = return_node->value;
            free(return_node);
            // 应该这个时候free
            return return_value;
        }
        else
        {
            log_err("%s no more values!");
            exit_test(1);
        }
    }
    else
    {
        // 如果在map中找不到以函数命令的节点
        log_err("%s has not been initialized!", function_name);
        exit_test(1);
    }
    return 0;
}


void add_symbol_value(ListNode* const head,const char *function_name,const LargeINT value)
{
    ListNode *target_node;
    SymbolMapValue *target_map_value;
    if(!list_find(head,function_name,&target_node,symbol_names_match))
    {
        //  没找到重新初始化一个
        SymbolMapValue *const new_symbol_map_value = malloc(sizeof(*new_symbol_map_value));
        new_symbol_map_value->symbol_name = function_name;
        list_initialize(&new_symbol_map_value->fn_values_list_head);
        target_node = list_add_value(head,new_symbol_map_value);    
    }
    target_map_value = (SymbolMapValue *)target_node->value;
    // 先找到以函数名命名的节点,然后往这个节点的链表里添加数据
    list_add_value(&target_map_value->fn_values_list_head,value);
}
// Add a return value for the specified mock function name.
void _will_return(const char * function_name, const LargeINT value)
{
   
    add_symbol_value(&global_function_result_map_head, function_name, value);
}