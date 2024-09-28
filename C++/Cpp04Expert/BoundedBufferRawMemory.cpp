/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o BoundedBufferRawMemory BoundedBufferRawMemory.cpp
*/

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include <memory>
#include <algorithm>
#include <stdexcept>

namespace raw_memory
{
  template<typename T>
  struct BoundedBuffer
  {
    using size_type = std::size_t;

    explicit BoundedBuffer(size_type N)
      : theSize{N == 0 ? throw std::invalid_argument("Zero elements not permitted") : N}
      , theQueue{std::make_unique<std::byte[]>(sizeof(T) * theSize)}
    {}

    ~BoundedBuffer()
    {
      destroyAllElements();
    }

    BoundedBuffer(const BoundedBuffer& from)
      : theSize{from.theSize}
      , theQueue{std::make_unique<std::byte[]>(sizeof(T) * theSize)}
    {
      appendElements(from);
    }

    BoundedBuffer& operator=(const BoundedBuffer& from)
    {
      destroyAllElements();
      if (theSize != from.theSize)
        {
          theSize = from.theSize;
          theQueue = std::make_unique<std::byte[]>(sizeof(T) * theSize);
        }
      appendElements(from);
      return *this;
    }

    BoundedBuffer(BoundedBuffer&& from) noexcept
    {
      swap(from);
    }

    BoundedBuffer& operator=(BoundedBuffer&& from) noexcept
    {
      destroyAllElements();
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
        return const_cast<T const&>(self.elementAt(self.position));
      else
        return self.elementAt(self.position);
    }

    decltype(auto) back(this auto& self)
    {
      if (self.empty())
        throw std::logic_error("Buffer empty");
      if constexpr (std::is_const_v<std::remove_reference_t<decltype(self)>>)
        return const_cast<T const&>(self.elementAt(self.computeLast()));
      else
        return self.elementAt(self.computeLast());
    }

    void push(const T& t) &
    {
      if (full())
        throw std::logic_error("Buffer full");
      std::construct_at<T>(elementAddress(computeNext()), t);
      elements++;
    }

    void push(T&& t) &
    {
      if (full())
        throw std::logic_error("Buffer full");
      std::construct_at<T>(elementAddress(computeNext()), std::move(t));
      elements++;
    }

    void pop() &
    {
      if (empty())
        throw std::logic_error("Buffer empty");
      std::destroy_at<T>(&elementAt(position));
      position++;
      position %= theSize;
      elements--;
    }

    template<typename ...Ts>
    void push_emplace(Ts&& ...Args) &
    {
      if (full())
        throw std::logic_error{"Buffer full"};
      std::construct_at<T>(elementAddress(computeNext()), std::forward<Ts>(Args)... );
      elements++;
    }

  private:
    size_type theSize{0};
    size_type position{0};
    size_type elements{0};
    std::unique_ptr<std::byte[]> theQueue;

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

    T* elementAddress(size_type it) const
    {
      if (it >= theSize) throw std::out_of_range{"Error in BoundedBuffer"};
      size_type const byte_index = sizeof(T) * it;
      return std::launder(reinterpret_cast<T*>(&theQueue[byte_index]));
    }

    T& elementAt(size_type it)
    {
      return *(elementAddress(it));
    }

    T const& elementAt(size_type it) const
    {
      return *const_cast<T const*>(elementAddress(it));
    }

    void destroyAllElements() noexcept
    {
      while(!empty()) pop();
    }

    void appendElements(BoundedBuffer const& from)
    {
      for(size_t it = 0; it < from.size(); it++)
        push(from.elementAt((from.position + it) % from.theSize));
    }
  };

}

#endif

int main()
{
  raw_memory::BoundedBuffer<int> buff{10};
  return 0;
}
