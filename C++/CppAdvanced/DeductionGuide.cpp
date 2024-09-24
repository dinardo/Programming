/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o DeductionGuide DeductionGuide.cpp
*/

#include <iostream>
#include <vector>
#include <string>

template<typename T>
struct dynArray: std::vector<T>
{
  using std::vector<T>::vector;
  // Makes the symbol vector from the namespace
  // std::vector<T> accessible for unqualified
  // lookup as if declared in the same class scope

  decltype(auto) operator[](int i) &
  {
    return std::vector<T>::at(computeIndex(i));
  }

  decltype(auto) operator[](int i) const &
  {
    return std::vector<T>::at(computeIndex(i));
  }

private:
  int computeIndex(int i)
  {
    return (i < 0 ? this->size() + i : i);
  }
};

// #################################
// # Class Template Specialization #
// #################################
template<>
struct dynArray<const char*> : dynArray<std::string>
{
  using dynArray<std::string>::dynArray;
};

// #############################
// # Explicit Deduction Guides #
// #############################
template<std::integral N, typename T>
dynArray(N, T) -> dynArray<T>;

template<typename IT>
// It is similar to: constructor -> how my class should be called by spelling out the type
// dynArray(IT, IT) -> dynArray<std::iter_value_t<IT>>;
// OR
dynArray(IT t, IT) -> dynArray<std::remove_reference_t<decltype(*t)>>;

template<typename T>
dynArray(std::initializer_list<T>) -> dynArray<T>;

int main()
{
  dynArray vec1(10, "hi");
  for(auto v: vec1)
    std::cout << "Ele: " << v << '\n';

  auto vec2 = dynArray({1,2,3,4});
  for(int i = -static_cast<int>(vec2.size()); i < static_cast<int>(vec2.size()); i++)
    std::cout << "Indx: " << i << " Ele: " << vec2[i] << '\n';

  dynArray vec3(vec2.begin(), vec2.end());
  for(int i = -static_cast<int>(vec3.size()); i < static_cast<int>(vec3.size()); i++)
    std::cout << "Indx: " << i << " Ele: " << vec3[i] << '\n';

  return 0;
}
