/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o Template Template.cpp
*/

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std::literals;

template <typename T>
T median(T a, T b, T c)
// ############################
// # Using just one operator> #
// ############################
{
  if(a > b)
    {
      if(c > a)
        return a;
      else if(b > c)
        return b;
      else
        return c;
    }
  else
    {
      if(c > b)
        return b;
      else if(a > c)
        return a;
      else
        return c;
    }
}

template <typename T>
void rotate3arguments(T& a, T& b, T& c)
{
  std::swap(a,b);
  std::swap(b,c);
}

template<typename ...Ts>
void readln(std::istream& in, Ts& ...args)
{
  if constexpr (sizeof...(args) > 0)
    (in >> ... >> args);
}

int main ()
{
  std::cout << "Testing mdian" << '\n';
  assert(median(1, 2, 3) == 2 && "Not good");
  assert(median(3, 2, 1) == 2 && "Not good");
  assert(median(3, 1, 2) == 2 && "Not good");
  assert(median(1, 3, 2) == 2 && "Not good");
  assert(median(2, 3, 1) == 2 && "Not good");
  assert(median(2, 1, 3) == 2 && "Not good");
  assert(median(2, 1, 2) == 2 && "Not good");
  assert(median("aa"s, "bb"s, "cc"s) == "bb"s && "Not good");

  std::cout << "\nTesting rotate3arguments" << '\n';
  int a, b, c;
  a = 1;
  b = 2;
  c = 3;
  for(int i = 0; i < 3; i++)
    {
      rotate3arguments(a, b, c);
      std::cout << "Fist: " << a << " Second: " << b << " Third: " << c << '\n';
    }

  std::string s1, s2, s3;
  s1 = "Hello";
  s2 = "Mauro";
  s3 = "Nice";
  for(int i = 0; i < 3; i++)
    {
      rotate3arguments(s1, s2, s3);
      std::cout << "Fist: " << s1 << " Second: " << s2 << " Third: " << s3 << '\n';
    }

  std::vector<int> v1, v2, v3;
  v1 = {1,2,3};
  v2 = {4,5,6};
  v3 = {7,8,9};
  for(int i = 0; i < 3; i++)
    {
      rotate3arguments(v1, v2, v3);
      std::cout << "Fist: ";
      for(auto it: v1)
        std::cout << it;
      std::cout << " Second: ";
      for(auto it: v2)
        std::cout << it;
      std::cout << " Third: ";
      for(auto it: v3)
        std::cout << it;
      std::cout << '\n';
    }

  int one_a{1}, one;
  std::string two_a{"ciao"}, two;
  double three_a{2.3} ,three;
  std::stringstream test;
  test << one_a << " " << two_a << " " << three_a;

  std::cout << "\nTesting readln" << '\n';
  readln(test, one, two, three);
  auto in = std::make_tuple(one, two, three);
  std::cout << "readln 3 params: " << one << " - " << two << " - " << three << '\n';
  assert(in == std::tie(one_a, two_a, three_a));

  readln(std::cin);
  std::cout << "readln 0 params: " << '\n';
}
