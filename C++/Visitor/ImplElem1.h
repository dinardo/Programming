#ifndef _IMPLELEM1_
#define _IMPLELEM1_

#include "Element.h"
#include <string>


// ##########################
// # Element implementation #
// ##########################

class ImplElem1: public Element
{
public:
  void        accept   (Visitor& v);
  std::string fooElem1 ();
  std::string barElem1 ();
};

#endif
