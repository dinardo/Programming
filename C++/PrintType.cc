// c++ -std=c++17 -o PrintType PrintType.cc

#include <iostream>

template <typename T>
constexpr std::string_view type_name()
{
    std::string_view p = __PRETTY_FUNCTION__;
    return std::string_view(p.data() + 34, p.size() - 34 - 1);
}

int main()
{
    const double a = 0;
    std::cout << type_name<decltype(a)>() << '\n';
    return 1;
}
