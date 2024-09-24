#include <cmath>


#include "Vector2D.h"


Vector2D::Vector2D(float xVal, float yVal)
{
  x = xVal;
  y = yVal;
}


Vector2D Vector2D::operator * (const float& scalar)
{
  Vector2D result = *this; // Make a copy of myself (same as MyClass result(*this))
  result.x *= scalar;
  result.y *= scalar;
  return result;
}


Vector2D Vector2D::operator / (const float& scalar)
{
  Vector2D result = *this; // Make a copy of myself (same as MyClass result(*this))
  result.x /= scalar;
  result.y /= scalar;
  return result;
}


Vector2D Vector2D::operator + (const Vector2D& other)
{
 Vector2D result = *this; // Make a copy of myself (same as MyClass result(*this))
 result += other;         // Use += to add other to the copy
 return result;
}


Vector2D Vector2D::operator - (const Vector2D& other)
{
 Vector2D result = *this; // Make a copy of myself (same as MyClass result(*this))
 result -= other;         // Use += to add other to the copy
 return result;
}


void Vector2D::operator += (const Vector2D& other)
{
  this->x += other.x;
  this->y += other.y;
}


void Vector2D::operator -= (const Vector2D& other)
{
  this->x -= other.x;
  this->y -= other.y;
}


void Vector2D::operator = (const Vector2D& other)
{
 if (this != &other)
   {
     this->x = other.x;
     this->y = other.y;
   }
}


float Vector2D::Mag ()
{
  return sqrt(x*x+y*y);
}


Vector2D::~Vector2D()
{
}
