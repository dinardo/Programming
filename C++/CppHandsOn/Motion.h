#ifndef MOTION_H
#define MOTION_H


#include "Vector2D.h"


class Motion
{

 public:
  virtual void Move (Vector2D&, Vector2D&, float, float) const = 0;
  // This means that the member method does not change "this"
  virtual Motion* ChangeMotion () const = 0;
  bool CheckBoundaries (Vector2D&, Vector2D&, float, float, float, float, float);
  virtual ~Motion() {};


 private:

};

#endif
