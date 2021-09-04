#ifndef _IMPLELEM2_
#define _IMPLELEM2_

#include "Element.h"
#include <string>


// ##########################
// # Element implementation #
// ##########################

class ImplElem2: public Element
{
public:
  void        accept   (Visitor& v);
  std::string fooElem2 ();
  std::string barElem2 ();
};

#endif
