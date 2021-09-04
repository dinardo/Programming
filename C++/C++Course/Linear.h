#ifndef LINEAR_H
#define LINEAR_H


#include "Motion.h"
#include "Vector2D.h"


class Linear : public Motion
{

 public:
  void Move (Vector2D&, Vector2D&, float, float) const;
  Motion* ChangeMotion () const;


 private:

};

#endif
