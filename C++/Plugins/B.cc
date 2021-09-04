#include <iostream>

#include "B.h"

B::B()
{
  std::cout << "Ctor of B" << std::endl;
}

B::~B()
{
  std::cout << "Dtor of B" << std::endl;
}

void B::f()
{
  std::cout << "B::f()" << std::endl;
}


extern "C" Mother* create()
{
  return new B;
}

extern "C" void destroy (Mother* p)
{
  delete p;
}
