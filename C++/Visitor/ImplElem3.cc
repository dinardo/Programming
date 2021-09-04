#include "ImplElem3.h"

void ImplElem3::accept(Visitor& v)
{
  v.visit(this);
}

std::string ImplElem3::fooElem3()
{
  return "[ImplElem3::fooElem3]";
}

std::string ImplElem3::barElem3()
{
  return "[ImplElem3::barElem3]";
}
