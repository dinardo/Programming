#ifndef LCENTRAL_H
#define LCENTRAL_H


#include "Motion.h"
#include "Vector2D.h"


class LCentral : public Motion
{

 public:
  void Move (Vector2D&, Vector2D&, float, float) const;
  Motion* ChangeMotion () const;


 private:

};

#endif
