#include <iostream>
#include <string>
#include "./stack.hpp"

void EXPECT_EQ(int lhs, int rhs, std::string test_name)
{
    if(lhs==rhs)
    {
        std::cout<<"PASS"<<std::endl;
    }

    else
    {
        std::cout<<"FAILED "<<test_name<<std::endl;
    }

    return;
}

void EXPECT_TRUE(bool is_true, std::string test_name)
{
    if(is_true)
    {
        std::cout<<"PASS"<<std::endl;
    }

    else
    {
        std::cout<<"FAILED "<<test_name<<std::endl;
    }

    return;
}


int main()
{
    IntStack test_stack;
    EXPECT_EQ(test_stack.size(),0, "initialization");


    test_stack.push(99);
    EXPECT_EQ(test_stack.top(),99, "pushed_top");
    EXPECT_EQ(test_stack.size(),1, "pushed_size");

    test_stack.pop();
    EXPECT_EQ(test_stack.size(),0, "pop_size");
    EXPECT_TRUE(test_stack.empty(), "pop_empty");

    int n=300000000;
    for(int i=0; i<n; ++i)
    {
        test_stack.push(i);
    }
    EXPECT_EQ(test_stack.size(),n, "push_many_size");
    
    for(int i=0; i<n/2; ++i)
    {
        test_stack.pop();
    }
    EXPECT_EQ(test_stack.size(),n/2, "pop_many_size");
    EXPECT_EQ(test_stack.top(),n/2-1, "pop_many_top");

    
    for(int i=0; i<n/2; ++i)
    {
        test_stack.pop();
    }
    EXPECT_EQ(test_stack.size(),0, "pop_all_size");
    EXPECT_TRUE(test_stack.empty(), "pop_all_empty");

    return 0;
}
