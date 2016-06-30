// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "Header.h"

typedef TYPELIST_3(char, signed char, unsigned char) CharList;
typedef TL::TypeAt<CharList, 0>::Result T0;
typedef TL::TypeAt<CharList, 1>::Result T1;
typedef TL::TypeAt<CharList, 2>::Result T2;
typedef TL::Append<CharList, int>::Result CharList2;
typedef TL::TypeAt<CharList2, 3>::Result T3;

typedef TL::Erase<CharList2, signed char>::Result CharList3;

typedef TL::EraseAll<TYPELIST_4(int, int, float, int), int>::Result NoDup;

int main()
{
    typedef TL::Length<CharList> Len;
    int n = Len::value;
    return 0;
}
