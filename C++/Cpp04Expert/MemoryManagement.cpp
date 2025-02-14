/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o MemoryManagement MemoryManagement.cpp
*/

#include <iostream>
#include <memory_resource>
#include <array>
#include <vector>
#include <cstddef>
#include <numeric>
#include <cstdint>
#include <iterator>
#include <algorithm>
#include <new>

int use_vector_with_pmr_resource()
{
  using namespace std::pmr;

  std::array<std::byte,1000> heap{};

  monotonic_buffer_resource memory{
    heap.data()
    ,heap.size()
    ,null_memory_resource()};
  //,new_delete_resource()};

  polymorphic_allocator<std::byte> const myalloc { &memory };

  std::vector<int64_t> v{myalloc};

  try
  {
    //v.reserve(sizeof(heap)/sizeof(decltype(v)::value_type));
    generate_n(std::back_inserter(v),1000,[i=0]()mutable{return i++;});
  }
  catch (std::bad_alloc const &)
  {
    // ignore here intentionally
  }

  std::cout << "Size: " << v.size() << std::endl;
  return std::accumulate(begin(v),end(v),0);
}

int main()
{
    std::cout << use_vector_with_pmr_resource() << '\n';
}
