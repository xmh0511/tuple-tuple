
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
#include <iostream>
#include <tuple>
#include <typeinfo>
using namespace std;
template<int N,int N2,typename ElementA,typename TupleB>
struct explainB
{
    template<typename Res>
    static auto package(ElementA elA,TupleB tupb, Res ret)
    {
        auto n_res = std::tuple_cat(ret,std::tuple<std::pair<ElementA, typename std::tuple_element<N, TupleB>::type>>(std::pair<ElementA, typename std::tuple_element<N, TupleB>::type>(elA, std::get<N>(tupb))));
        return explainB<N + 1, N2, ElementA, TupleB>::template package(elA, tupb, n_res);
    }
};
template<int N, typename ElementA, typename TupleB>
struct explainB<N,N, ElementA, TupleB>
{
    template<typename Res>
    static auto package(ElementA elA, TupleB tupb, Res ret)
    {
        auto n_res = std::tuple_cat(ret, std::tuple<std::pair<ElementA, typename std::tuple_element<N, TupleB>::type>>(std::pair<ElementA, typename std::tuple_element<N, TupleB>::type>(elA, std::get<N>(tupb))));
        return n_res;
    }
};
template<int N,int N2,typename TupleA,typename...Args>
struct explainA
{
    template<typename Res>
    static auto package(TupleA tupa, std::tuple<Args...> tupb, Res ret)
    {
        auto b_res = explainB<0, sizeof...(Args) - 1,typename std::tuple_element<N, TupleA>::type, std::tuple<Args...>>::template package(std::get<N>(tupa), tupb, ret);
        return explainA<N + 1, N2, TupleA, Args...>::template package(tupa, tupb, b_res);
    }
};
template<int N,typename TupleA, typename...Args>
struct explainA<N,N, TupleA, Args...>
{
    template<typename Res>
    static auto package(TupleA tupa, std::tuple<Args...> tupb, Res ret)
    {
        auto b_res = explainB<0, sizeof...(Args) - 1, typename std::tuple_element<N, TupleA>::type, std::tuple<Args...>>::template package(std::get<N>(tupa), tupb, ret);
        return b_res;
    }
};
 
template<typename...TAParams, typename...TBParams>
auto multiply(std::tuple<TAParams...> ta, std::tuple<TBParams...> tb)
{
    return explainA<0, sizeof...(TAParams) - 1, std::tuple<TAParams...>, TBParams...>::template package(ta, tb, std::tuple<>());
};
template<typename TupleA,typename TupleB>
struct Multiply_Tuple
{
    using type = decltype(multiply(std::declval<TupleA>(),std::declval<TupleB>()));
};
int main()
{
    cout << typeid(typename Multiply_Tuple<std::tuple<int, float>,std::tuple<float, char>>::type).name() << endl;
    return 0;
}
