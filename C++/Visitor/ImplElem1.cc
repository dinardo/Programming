#include "ImplElem1.h"

void ImplElem1::accept(Visitor& v)
{
  v.visit(this);
}

std::string ImplElem1::fooElem1()
{
  return "[ImplElem1::fooElem1]";
}

std::string ImplElem1::barElem1()
{
  return "[ImplElem1::barElem1]";
}
