#ifndef _VISITOR_
#define _VISITOR_

class ImplElem1;
class ImplElem2;
class ImplElem3;


// ###########################################################################################
// # 2. Create a "visitor" base class w/ a visit() method for every "element" implementation #
// ###########################################################################################
class Visitor
{
public:
  virtual void visit(ImplElem1* e) = 0;
  virtual void visit(ImplElem2* e) = 0;
  virtual void visit(ImplElem3* e) = 0;
};

#endif
