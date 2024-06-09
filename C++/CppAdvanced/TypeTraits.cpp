/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o TypeTraits TypeTraits.cpp
*/

#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <cxxabi.h> // __cxa_demangle

struct freeDeleter
{
  template<typename T>
  void operator()(T* p) const
  {
    std::free(const_cast<std::remove_const_t<T> *>(p));
  }
};

// #################################################
// # Safe way to free memory allocated with malloc #
// #################################################
std::string demangle(const char* name)
{
  auto safeConstC = std::unique_ptr<const char, freeDeleter>{__cxxabiv1::__cxa_demangle(name,0,0,0)};
  std::string result(safeConstC.get());
  return result;
}

template<typename T, template<typename> typename TRAIT, typename... REST>
void outputTrait(std::ostream& out, TRAIT<T> trait, REST ...args)
{
  out << demangle(typeid(T).name()) << "'s trait "
      << demangle(typeid(TRAIT<T>).name()) << " is "
      << std::boolalpha
      << trait
      << '\n';

  if constexpr(sizeof...(REST))
    outputTrait(out, args...);
}

template<typename T, template<typename> typename ...TRAITS>
void outputAllTraits(std::ostream &out)
{
  outputTrait(out,TRAITS<T>{}...);
}

template<typename T>
void outputTraits(std::ostream &out)
{
  using namespace std;
  outputAllTraits<T,
    is_null_pointer, is_floating_point, is_array, is_enum, is_union, is_class, is_function, is_pointer,
    is_lvalue_reference, is_rvalue_reference, is_member_object_pointer, is_member_function_pointer,
    is_fundamental, is_arithmetic, is_scalar, is_object, is_compound, is_reference, is_member_pointer,
    is_const, is_volatile, is_trivial, is_trivially_copyable, is_standard_layout, is_empty,
    is_polymorphic, is_abstract, is_final, is_aggregate, is_signed, is_unsigned
    >(out);
}

template<typename T, typename ...REST>
void outputTraitsForTypes(std::ostream& out)
{
  outputTraits<T>(out);
  out << "---------------------------------------------------\n";
  if constexpr (sizeof...(REST))
    outputTraitsForTypes<REST...>(out);
}

int main()
{
  struct Base
  {
    virtual ~Base() = 0;
  };

  outputTraitsForTypes<bool, int, int&, int const &, int *, void, void *, std::string, Base>(std::cout);

  return 0;
}
