/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o BoundedBufferHeap BoundedBufferHeap.cpp
*/

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include <memory>
#include <algorithm>
#include <stdexcept>

namespace heap_memory
{
  template<typename T>
  struct BoundedBuffer
  {
    using size_type = std::size_t;

    explicit BoundedBuffer(size_type N)
      : theSize{N == 0 ? throw std::invalid_argument("Zero elements not permitted") : N}
      , theQueue{std::make_unique<T[]>(theSize)}
    {}

    ~BoundedBuffer() noexcept = default;

    BoundedBuffer(const BoundedBuffer& from)
      : theSize{from.theSize}
      , position{from.position}
      , elements{from.elements}
      , theQueue{std::make_unique<T[]>(theSize)}
    {
      std::copy_n(from.theQueue.get() + from.position, from.elements, theQueue.get());
    }

    BoundedBuffer& operator=(const BoundedBuffer& from)
    {
      if(this != &from)
        {
          BoundedBuffer tmp{from};
          this->swap(tmp);
        }
      return *this;
   }

    BoundedBuffer(BoundedBuffer&& from) noexcept
    {
      swap(from);
    }

    BoundedBuffer& operator=(BoundedBuffer&& from) noexcept
    {
      theQueue.reset();
      this->swap(from);
      return *this;
    }

    void swap(BoundedBuffer &from)
    {
      using std::swap;
      swap(theSize,from.theSize);
      swap(position,from.position);
      swap(elements,from.elements);
      swap(theQueue,from.theQueue);
    }

    bool empty() const &
    {
      return (elements == 0 ? true : false);
    }

    bool full() const &
    {
      return (elements == theSize ? true : false);
    }

    size_type size() const &
    {
      return elements;
    }

    decltype(auto) front(this auto& self)
    {
      if (self.empty())
        throw std::logic_error("Buffer empty");
      if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>)
        return const_cast<T const&>(self.theQueue[self.position]);
      else
        return self.theQueue[self.position];
    }

    decltype(auto) back(this auto& self)
    {
      if(self.empty())
        throw std::logic_error("Buffer empty");
      if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>)
        return const_cast<T const&>(self.theQueue[self.computeLast()]);
      else
        return self.theQueue[self.computeLast()];
    }

    void push(const T& t) &
    {
      if(full())
        throw std::logic_error("Buffer full");
      theQueue[computeNext()] = t;
      elements++;
    }

    void push(T&& t) &
    {
      if(full())
        throw std::logic_error("Buffer full");
      theQueue[computeNext()] = std::move(t);
      elements++;
    }

    void pop() &
    {
      if(empty())
        throw std::logic_error("Buffer empty");
      position++;
      position %= theSize;
      elements--;
    }

  private:
    size_type theSize{0};
    size_type position{0};
    size_type elements{0};
    std::unique_ptr<T[]> theQueue;

    size_type computeLast() const
    {
      auto ele = (elements == 0 ? 0 : elements - 1);
      return ((position + ele) % theSize);
    }

    size_type computeNext() const
    {
      auto ele = (elements == 0 ? 0 : elements);
      return ((position + ele) % theSize);
    }
  };

}

#endif

int main()
{
  heap_memory::BoundedBuffer<int> buff{10};
  return 0;
}
