#include "Colour.h"


Colour Colour::rgb1 (float r, float g, float b)
{
  return Colour((int)(r*255.0), (int)(g*255.0), (int)(b*255.0));
}

 
Colour Colour::rgb255 (int r, int g, int b)
{
  return Colour(r,g,b);
}


Colour::Colour (const Colour& c) // Copy constructor
{
  this->red   = c.red;
  this->green = c.green;
  this->blue  = c.blue;
}


float Colour::r1 ()
{
  return ((float)this->red)/255.0;
}


float Colour::g1 ()
{
  return ((float)this->green)/255.0;
}


float Colour::b1 ()
{
  return ((float)this->blue)/255.0;
}


Colour::Colour (int r, int g, int b)
{
  this->red   = r;
  this->green = g;
  this->blue  = b;
}


Colour::~Colour ()
{ 
}
