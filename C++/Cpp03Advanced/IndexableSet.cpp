/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o IndexableSet IndexableSet.cpp
*/

#include <iostream>
#include <string>
#include <iterator>
#include <set>

template<typename T, typename COMPARE = std::less<T>>
struct indexableSet: std::set<T, COMPARE>
{
  using std::set<T, COMPARE>::set;
  using setIter = std::set<T, COMPARE>::iterator;

  decltype(auto) operator[](int i) const&
  {
    auto it = this->begin();
    std::advance(it, computeIndex(i));
    return *it;
  }

  decltype(auto) operator[](int i) &
  {
    auto it = this->begin();
    std::advance(it, computeIndex(i));
    return *it;
  }

  decltype(auto) at(int i) const&
  {
    auto it = this->begin();
    std::advance(it, computeIndex(i));
    return *it;
  }

  decltype(auto) at(int i) &
  {
    auto it = this->begin();
    std::advance(it, computeIndex(i));
    return *it;
  }

  decltype(auto) front() const&
  {
    return this->at(0);
  }

  decltype(auto) front() &
  {
    return this->at(0);
  }

  decltype(auto) back() const&
  {
    return this->at(-1);
  }

  decltype(auto) back() &
  {
    return this->at(-1);
  }

private:
  size_t computeIndex(int i)
  {
    if ((i >= static_cast<int>(this->size())) || ((i < 0) && (std::fabs(i) > this->size())))
      throw std::out_of_range{"[indexableSet::computeIndex] out of range"};
    return (i < 0 ? this->size() + i : i);
  }
};

// #############################
// # Explicit Deduction Guides #
// #############################
template<typename T>
indexableSet(std::initializer_list<T>) -> indexableSet<T>;

int main()
{
  indexableSet mySet({3, 3, 1, 5, 4, 4});

  for(auto a: mySet)
    std::cout << "Ele: " << a << '\n';

  for(auto i = 0ul; i < mySet.size(); i++)
    std::cout << "Ele " << i << " : " << mySet[i] << '\n';

  for(auto i = 1ul; i <= mySet.size(); i++)
    std::cout << "Ele " << -(int)i << " : " << mySet[-i] << '\n';

  return 0;
}
