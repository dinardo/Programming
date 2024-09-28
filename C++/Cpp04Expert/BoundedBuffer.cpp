/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o BoundedBuffer BoundedBuffer.cpp
*/

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include <array>
#include <stdexcept>

namespace fixed_size
{
  template<typename T, std::size_t N>
  struct BoundedBuffer: std::array<T, N>
  {
    using std::array<T, N>::array;
    using Base = std::array<T, N>;
    using size_type = typename Base::size_type;
    using container_type = Base;

    bool empty() const &
    {
      return (elements == 0 ? true : false);
    }

    bool full() const &
    {
      return (elements == N ? true : false);
    }

    size_type size() const &
    {
      return elements;
    }

    decltype(auto) front(this auto& self)
    {
      if (self.empty())
        throw std::logic_error("Buffer empty");
      return self[self.position];
    }

    decltype(auto) back(this auto& self)
    {
      if(self.empty())
        throw std::logic_error("Buffer empty");
      return self[self.computeLast()];
    }

    void push(const T& t) &
    {
      if(full())
        throw std::logic_error("Buffer full");
      Base::at(computeNext()) = t;
      elements++;
    }

    void push(T&& t) &
    {
      if(full())
        throw std::logic_error("Buffer full");
      Base::at(computeNext()) = std::move(t);
      elements++;
    }

    void pop() &
    {
      if(empty())
        throw std::logic_error("Buffer empty");
      position++;
      position %= N;
      elements--;
    }

  private:
    size_type position{0};
    size_type elements{0};

    size_type computeLast() const
    {
      auto ele = (elements == 0 ? 0 : elements - 1);
      return ((position + ele) % N);
    }

    size_type computeNext() const
    {
      auto ele = (elements == 0 ? 0 : elements);
      return ((position + ele) % N);
    }
  };

}

#endif

int main()
{
  fixed_size::BoundedBuffer<int, 10> buff;
  return 0;
}
