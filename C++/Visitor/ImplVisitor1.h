#ifndef _IMPLVISITOR1_
#define _IMPLVISITOR1_

#include "Visitor.h"
#include <iostream>


// ###############################################################################
// # 3. Create a "visitor" derived class for each "operation" to do on "element" #
// ###############################################################################
class ImplVisitor1: public Visitor
{
  void visit(ImplElem1* e)
  {
    std::cout << "Calling: " << e->fooElem1() << std::endl;
  }

  void visit(ImplElem2* e)
  {
    std::cout << "Calling: " << e->fooElem2() << std::endl;
  }

  void visit(ImplElem3* e)
  {
    std::cout << "Calling: " << e->fooElem3() << std::endl;
  }
};

#endif
