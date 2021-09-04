#ifndef _MOTHER_
#define _MOTHER_

#include <iostream>

class Mother
{
 public:
  Mother() { std::cout << "Ctor of Mother" << std::endl; }
  virtual ~Mother() = 0;
  virtual void f()  = 0;
};

#endif
