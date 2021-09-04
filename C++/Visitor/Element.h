#ifndef _ELEMENT_
#define _ELEMENT_

#include "Visitor.h"


// ###############################################################
// # 1. Add an accept(Visitor) method to the "element" hierarchy #
// ###############################################################
class Element
{
public:
  virtual void accept(class Visitor &v) = 0;
};

#endif
