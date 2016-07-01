#pragma once

template<int v>
struct Int2Type
{
    enum { value = v };
};

template<class T, class U>
class Conversion
{
private:
    typedef char Small;
    class Big { char dummy[2]; };

    // ReSharper disable CppFunctionIsNotImplemented
    static Small Test(U);
    static Big Test(...);
    static T MakeT();
    // ReSharper restore CppFunctionIsNotImplemented
public:
    // true if T is assignable to U
    enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
    enum { sameType = false };
};

template<class T>
class Conversion<T, T>
{
public:
    // true if T is assignable to U
    enum { exists = true };
    enum { sameType = true };
};

#define SUPERSUBCLASS(T, U) (Conversion<const U*, const T*>::exists && !Conversion<const T*, const void*>::sameType)

template<bool, class T, class U> struct Select;

template<class T, class U>
struct Select<true, T, U>
{
    typedef T Result;
};

template<class T, class U>
struct Select<false, T, U>
{
    typedef U Result;
};

class NullType {};

template <class T, class U>
class TypeList
{
    typedef T Head;
    typedef U Tail;
};

#define TYPELIST_1(T1) TypeList<T1, NullType>
#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T2) >
#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3) >
#define TYPELIST_4(T1, T2, T3, T4) TypeList<T1, TYPELIST_3(T2, T3, T4) >

namespace TL
{
    //Length
    template<class  TList> struct Length;

    template<> struct Length<NullType>
    {
        enum { value = 0 };
    };

    template<class T, class U> struct Length<TypeList<T, U>>
    {
        enum { value = 1 + Length<U>::value };
    };

    //TypeAt
    template<class TList, int index> struct TypeAt;

    template<class Head, class Tail> struct TypeAt<TypeList<Head, Tail>, 0>
    {
        typedef Head Result;
    };

    template<class Head, class Tail, int index> struct TypeAt<TypeList<Head, Tail>, index>
    {
        typedef typename TypeAt<Tail, index - 1>::Result Result;
    };

    //IndexOf
    template <class TList, class T> struct IndexOf;

    template<class T> struct IndexOf<NullType, T>
    {
        enum { value = -1 };
    };

    template<class Tail, class T> struct IndexOf<TypeList<T, Tail>, T>
    {
        enum { value = 0 };
    };

    template<class Head, class Tail, class T> struct IndexOf<TypeList<Head, Tail>, T>
    {
    private:
        enum { temp = IndexOf<Tail, T>::value };
    public:
        enum { value = temp < 0 ? -1 : temp + 1 };
    };

    //Append
    template <class TList, class T> struct Append;

    template<>
    struct Append<NullType, NullType>
    {
        typedef NullType Result;
    };

    template<class T>
    struct Append<NullType, T>
    {
        typedef TYPELIST_1(T) Result;
    };

    template<class Head, class Tail>
    struct Append<NullType, TypeList<Head, Tail>>
    {
        typedef TypeList<Head, Tail> Result;
    };

    template<class Head, class Tail, class T>
    struct Append<TypeList<Head, Tail>, T>
    {
        typedef TypeList<Head, typename Append<Tail, T>::Result> Result;
    };

    //Erase
    template<class TList, class T> struct Erase;

    template<class T>
    struct Erase<NullType, T>
    {
        typedef NullType Result;
    };

    template<class Tail, class T>
    struct Erase<TypeList<T, Tail>, T>
    {
        typedef Tail Result;
    };

    template<class Head, class Tail, class T>
    struct Erase<TypeList<Head, Tail>, T>
    {
        typedef TypeList<Head, typename Erase<Tail, T>::Result> Result;
    };

    //EraseAll
    template<class  TList, class T> struct  EraseAll;

    template<class T>
    struct EraseAll<NullType, T>
    {
        typedef NullType Result;
    };

    template<class Tail, class T>
    struct EraseAll<TypeList<T, Tail>, T>
    {
        typedef typename EraseAll<Tail, T>::Result Result;
    };

    template<class Head, class Tail, class T>
    struct EraseAll<TypeList<Head, Tail>, T>
    {
        typedef TypeList<Head, typename EraseAll<Tail, T>::Result> Result;
    };

    //NoDuplicates
    template <class TList> struct NoDuplicates;

    template<>
    struct NoDuplicates<NullType>
    {
        typedef NullType Result;
    };

    template <class Head, class Tail>
    struct NoDuplicates<TypeList<Head, Tail>>
    {
    private:
        typedef typename NoDuplicates<Tail>::Result L1;
        typedef typename Erase<L1, Head>::Result L2;
    public:
        typedef TypeList<Head, L2> Result;
    };

    //Replace
    template <class Tlist, class T, class U> struct Replace;

    template<class T, class U>
    struct Replace<NullType, T, U>
    {
        typedef NullType Result;
    };

    template <class Tail, class T, class U>
    struct Replace<TypeList<T, Tail>, T, U>
    {
        typedef TypeList<U, Tail> Result;
    };

    template <class Head, class Tail, class T, class U>
    struct Replace<TypeList<Head, Tail>, T, U>
    {
        typedef TypeList<Head, typename Replace<Tail, T, U>::Result> Result;
    };

    //ReplaceAll
    template <class Tlist, class T, class U> struct ReplaceAll;

    template<class T, class U>
    struct ReplaceAll<NullType, T, U>
    {
        typedef NullType Result;
    };

    template <class Tail, class T, class U>
    struct ReplaceAll<TypeList<T, Tail>, T, U>
    {
        typedef TypeList<U, typename ReplaceAll<Tail, T, U>::Result> Result;
    };

    template <class Head, class Tail, class T, class U>
    struct ReplaceAll<TypeList<Head, Tail>, T, U>
    {
        typedef TypeList<Head, typename ReplaceAll<Tail, T, U>::Result> Result;
    };

    //MostDerived
    template <class TList, class T> struct MostDerived;

    template<class T>
    struct MostDerived<NullType, T>
    {
        typedef T Result;
    };

    template<class Head, class Tail, class T>
    struct MostDerived<TypeList<Head, Tail>, T>
    {
    private:
        typedef typename MostDerived<Tail, T>::Result Candidate;
    public:
        typedef typename Select<SUPERSUBCLASS(Candidate, Head), Head, Candidate>::Result Result;
    };

    //DerivedToFront
    template<class TList> struct  DerivedToFront;

    template<>
    struct DerivedToFront<NullType>
    {
        typedef NullType Result;
    };

    template<class Head, class Tail>
    struct DerivedToFront<TypeList<Head, Tail>>
    {
    private:
        typedef typename MostDerived<Tail, Head>::Result TheMostDerived;
        typedef typename Replace<Tail, TheMostDerived, Head>::Result L;
    public:
        typedef TypeList<TheMostDerived, L> Result;
    };

}
