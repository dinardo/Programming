#ifndef DRAWER_H
#define DRAWER_H


#include "Colour.h"
#include "Vector2D.h"


class Drawer
{

 public:
  virtual void draw_circle (Colour c, Vector2D position, float radius) = 0;
  virtual void draw_square (Colour c, Vector2D position, float length) = 0;


 private:

};

#endif
