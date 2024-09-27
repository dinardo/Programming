/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o DynArray DynArray.cpp
*/

#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <vector>
#include <optional>
#include <stdexcept>

template<typename T>
struct DynArray: std::vector<T>
{
  using std::vector<T>::vector; // We inherit all constructors
  using base = std::vector<T>;

  [[nodiscard]] // Obliged to use the return value
  decltype(auto) operator[](int i) & // decltype(auto) allows to return a reference if needed
  {
    return base::at(computeIndex(i));
  }

  decltype(auto) operator[](int i) const &
  {
    return base::at(computeIndex(i));
  }

  // C++23: avoids code duplication for const and non const objects
  // because it deduces the type of self. In this case "this" doesn't
  // refer to the internal variable
  decltype(auto) operator[](this auto& self, int i)
  {
    return self.base::at(computeIndex(i));
  }

  decltype(auto) at(int i) &
  {
    return base::at(computeIndex(i));
  }

  decltype(auto) at(int i) const &
  {
    return base::at(computeIndex(i));
  }

  std::optional<T> pop_back()
  {
    if (this->empty()) return {};
    std::optional<T> popped = std::move(base::back()); //std::move to prevent unnecessary copy
    base::pop_back();
    return popped;
  }

private:
  int computeIndex(int i) const
  {
    return (i < 0 ? this->size() + i: i);
  }

  void throw_if_empty() const
  {
    if (base::empty())
      throw std::out_of_range{"DynArray empty"};
  }
};

template<typename T>
DynArray(std::initializer_list<T>) -> DynArray<T>;

template<std::integral N, typename T>
DynArray(N, T) -> DynArray<T>;

template<typename IT>
//DynArray(IT, IT) -> DynArray<std::iter_value_t<IT>>;
DynArray(IT t, IT) -> DynArray<std::remove_reference_t<decltype(*t)>>;

template<typename T>
auto make_dynArray(std::initializer_list<T> list)
{
  return DynArray(list);
}

#endif

int main()
{
  DynArray<int> arr{10};
  return 0;
}
