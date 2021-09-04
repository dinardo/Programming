#include <iostream>

#include "A.h"

A::A()
{
  std::cout << "Ctor of A" << std::endl;
}

A::~A()
{
  std::cout << "Dtor of A" << std::endl;
}

void A::f()
{
  std::cout << "A::f()" << std::endl;
}


extern "C" Mother* create()
{
  return new A;
}

extern "C" void destroy (Mother* p)
{
  delete p;
}
