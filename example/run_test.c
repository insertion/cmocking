#include<cmocking.h>
void test_case_err(void **state)
{
    int a = 5/0;
}
void test_case_success(void **state)
{
    
    int a = 5/3;
}

int main(int argc,char **argv)
{
    const UnitTest tests[] ={
        fn_to_UnitTest(test_case_err),
        fn_to_UnitTest(test_case_success),
    }; 
    return run_tests(tests);
}