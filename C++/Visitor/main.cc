/*
c++ -o main main.cc ImplElem1.cc ImplElem2.cc ImplElem3.cc

The Visitor programming pattern is used to implement double dispatch.
It means that the code that gets executed depends on runtime types of
two objects. The method that is being called ultimately depends on the
type of two objects - the type of the object implementing the "Element"
and the type of the object on which you call accept.
*/

#include "Element.h"

#include "ImplElem1.h"
#include "ImplElem2.h"
#include "ImplElem3.h"

#include "ImplVisitor1.h"
#include "ImplVisitor2.h"

#include <iostream>
#include <string>


// ################
// # Main program #
// ################
int main()
{
  Element* list[] =
    {
     new ImplElem1(), new ImplElem2(), new ImplElem3()
    };


  // #######################################
  // # 4. Client creates "vistor" objects" #
  // #######################################
  ImplVisitor1 implV1;
  ImplVisitor2 implV2;

  for (int i = 0; i < 3; i++)
    list[i]->accept(implV1);

  std::cout << std::endl;
  for (int i = 0; i < 3; i++)
    list[i]->accept(implV2);
}
