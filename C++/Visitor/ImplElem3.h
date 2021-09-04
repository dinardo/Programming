#ifndef _IMPLELEM3_
#define _IMPLELEM3_

#include "Element.h"
#include <string>


// ##########################
// # Element implementation #
// ##########################

class ImplElem3: public Element
{
public:
  void        accept   (Visitor& v);
  std::string fooElem3 ();
  std::string barElem3 ();
};

#endif
