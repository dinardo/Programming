/*
c++ -std=c++11 -o CuriouslyRecurringTemplatePattern CuriouslyRecurringTemplatePattern.cpp
*/

#include <iostream>

template <typename T>
class BaseClass
{
public:
  double foo() const
  {
    return static_cast<const T&>(*this).foo();
  }
};

class DerivedA : public BaseClass<DerivedA>
{
public:
  double foo() const
  {
    return myVal;
  }

private:
  const int myVal = 42;
};

class DerivedB : public BaseClass<DerivedB>
{
public:
  DerivedB (int val) : myVal(val) {}

  double foo() const
  {
    return myVal;
  }

private:
  int myVal;
};

int main()
{
  DerivedA A;
  const BaseClass<DerivedA>& base1 = A;
  std::cout << "Calling BaseClass<DerviedA>::foo(): " << base1.foo() << std::endl;

  DerivedB B(123);
  const BaseClass<DerivedB>& base2 = B;
  std::cout << "Calling BaseClass<DerviedB>::foo(): " << base2.foo() << std::endl;
}

// ###############################################
// # Curiously Recurring Template Pattern (CRPT) #
// ###############################################

// The CRTP consists in inheriting from a template base class, and use the
// derived class itself as a template parameter of the base class:

// template <typename T>
// class Base
// {
//   ...
// };

// class Derived : public Base<Derived>
// {
//   ...
// };

// The most important thing to note is that, although the BaseClass class is
// used polymorphically, there isn't any virtual in the code. This means that
// the polymorphic call has been resolved at compile-time, thus avoiding the
// run-time cost of virtual functions

// A interesting point to note is that, although the CRTP uses inheritance,
// its usage of it does not have the same meaning as other cases of inheritance

// In general, a class deriving from another class expresses that the derived
// class somehow conceptually "is a" base class. The purpose is to use the base
// class in generic code, and to redirect calls to the base class over to code
// in the derived class

// With the CRTP the situation is radically different. The derived class does
// not express the fact it "is a" base class. Rather, it expands its interface
// by inherting from the base class, in order to add more functionality. In this
// case it makes sense to use the derived class directly, and to never use the
// base class

// Therefore the base class is not the interface, and the derived class is not
// the implementation. Rather, it is the other way around: the base class uses
// the derived class methods (such as foo). In this regard, the derived class
// offers an interface to the base class. This illustrates again the fact that
// inheritance in the context of the CRTP can express quite a different thing
// from classical inheritance
