#include<cmocking.h>
#include<stdio.h>
int mock_query()
{
    return (int)mock();
} 

void test_case()
{
    will_return(mock_query,1234);
    log_info("the value is %d",mock_query());
}

int main(int argc,char**argv)
{
    const UnitTest tests[] = {
        fn_to_UnitTest(test_case),
    };
    return run_tests(tests);

}