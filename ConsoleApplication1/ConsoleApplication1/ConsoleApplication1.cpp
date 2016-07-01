// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include<vector>
#include<iostream>
#include<functional>
#include "Header.h"

typedef TYPELIST_3(char, signed char, unsigned char) CharList;
typedef TL::TypeAt<CharList, 0>::Result T0;
typedef TL::TypeAt<CharList, 1>::Result T1;
typedef TL::TypeAt<CharList, 2>::Result T2;
typedef TL::Append<CharList, int>::Result CharList2;
typedef TL::TypeAt<CharList2, 3>::Result T3;

typedef TL::Erase<CharList2, signed char>::Result CharList3;

typedef TL::EraseAll<TYPELIST_4(int, int, float, int), int>::Result NoDup;

typedef TL::Replace<TYPELIST_4(int, int, float, int), int, double>::Result Repl;
typedef TL::ReplaceAll<TYPELIST_4(int, int, float, int), int, double>::Result ReplAll;

class A
{

};

class B :public A
{

};

const bool e = SUPERSUBCLASS(A, B);

typedef TL::MostDerived<TYPELIST_3(A, B, int), A>::Result MostDer;
typedef TL::DerivedToFront<TYPELIST_4(A, B, B, int)>::Result DerToFront;

template<class F>
void test(F add, int i)
{
    add(i);
}

int main()
{
    std::vector<int> arr = { 1,2,3,4,5,6 };
    int sum = 0;

    auto add = [&sum](int v)
    {
        sum += v;
    };

    for (int a : arr)
    {
        test(add, a);
    }

    std::cout << sum;

    return 0;
}
