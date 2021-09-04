#include "ImplElem2.h"

void ImplElem2::accept(Visitor& v)
{
  v.visit(this);
}

std::string ImplElem2::fooElem2()
{
  return "[ImplElem2::fooElem2]";
}

std::string ImplElem2::barElem2()
{
  return "[ImplElem2::barElem2]";
}
