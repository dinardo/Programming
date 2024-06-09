/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o Exception Exception.cpp
*/

#include <iostream>
#include <optional>
#include <variant>

std::optional<int> divide_2a(int divident, int divisor)
{
    if(divisor == 0) return {};
    return divident / divisor;
}

enum class error_code
{
    byZero,
    good
};

std::ostream& operator<<(std::ostream& out, error_code e)
{
    return out << static_cast<int>(e);
}

std::variant<int, error_code> divide_2b(int divident, int divisor)
{
    if(divisor == 0) return error_code::byZero;
    return divident / divisor;
}

std::optional<int> divide_3(int divident, int divisor)
{
    if(divisor == 0) return std::nullopt;
    return divident / divisor;
}

int divide_3(int divident, int divisor, error_code& status)
{
    if(divisor == 0)
    {
        status = error_code::byZero;
        return 0;
    }
    status = error_code::good;
    return divident / divisor;
}

int divide_4(int divident, int divisor)
{
    if(divisor == 0)
    {
        throw error_code::byZero;
    }
    return divident / divisor;
}

int main()
{
    std::optional val_2a = divide_2a(1,1);
    if (val_2a)
    {
        std::cout << "Value: " << *val_2a << '\n';
    }
    else
    std::cout << "Division by zero" << '\n';

    auto val_2b = divide_2b(1,0);
    if(std::holds_alternative<error_code>(val_2b) && (std::get<error_code>(val_2b) == error_code::byZero))
    {
        std::cout << "Division by zero" << '\n';
    }
    else
        std::cout << "Value: " << std::get<int>(val_2b) << '\n';

    error_code status;
    int val_3 = divide_3(1,0,status);
    if(status == error_code::byZero)
    {
        std::cout << "Division by zero" << std::endl;
    }
    else
        std::cout << "Value: " << val_3 << std::endl;

    std::optional val_3b = divide_3(1,1);
    if(val_3b)
    {
        std::cout << "Value: " << *val_3b << std::endl;
    }
    else
        std::cout << "Division by zero" << std::endl;

    try
    {
        auto val_4 = divide_4(1,0);
        std::cout << "Value: " << val_4 << std::endl;
    }
    catch(const error_code& status)
    {
        std::cout << "Division by zero: " << static_cast<int>(status) << '\n';
        std::cout << "Division by zero: " << status << '\n';
    }
}
