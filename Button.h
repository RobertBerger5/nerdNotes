#ifndef _BUTTON_
#define _BUTTON_

#include <string.h>

#include "Note.h"


class Button{
 public:
  int x,y,w,h;
  std::string label;
  
  void (*func)(Note& n);//TODO: have a subclass that takes Note arguments?

  Button();
  Button(std::string l,int x1,int y1,int w1,int h1,void (*f)(Note& n));

  void draw();

  bool inside(int x,int y);
};

#endif
