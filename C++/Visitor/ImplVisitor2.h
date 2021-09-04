#ifndef _IMPLVISITOR2_
#define _IMPLVISITOR2_

#include "Visitor.h"
#include <iostream>


// ###############################################################################
// # 3. Create a "visitor" derived class for each "operation" to do on "element" #
// ###############################################################################
class ImplVisitor2: public Visitor
{
  void visit(ImplElem1* e)
  {
    std::cout << "Calling: " << e->barElem1() << std::endl;
  }

  void visit(ImplElem2* e)
  {
    std::cout << "Calling: " << e->barElem2() << std::endl;
  }

  void visit(ImplElem3* e)
  {
    std::cout << "Calling: " << e->barElem3() << std::endl;
  }
};

#endif
