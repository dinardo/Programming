#ifndef COLOUR_H
#define COLOUR_H

class Colour
{

 public:
  static Colour rgb1 (float, float, float);
  static Colour rgb255 (int, int , int);
  Colour (const Colour&); // Copy constructor
  float r1 ();
  float g1 ();
  float b1 ();
  ~Colour ();


 private:
  Colour (int, int, int);
  int red;
  int green;
  int blue;

};

#endif
