// Redirect malloc, calloc and free to the unit test allocators.
#if UNIT_TESTING
#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#endif // UNIT_TESTING
// 只在测试用例是否有内存问题的时候用到