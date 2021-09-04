#ifndef VECTOR_H
#define VECTOR_H


class Vector2D
{

 public:
  Vector2D(float, float);
  ~Vector2D();
  Vector2D operator * (const float&);
  Vector2D operator / (const float&);
  Vector2D operator + (const Vector2D&);
  Vector2D operator - (const Vector2D&);
  void operator += (const Vector2D&);
  void operator -= (const Vector2D&);
  void operator =  (const Vector2D&);
  float Mag ();

  float x;
  float y;


 private:

};

#endif
