#ifndef _TEXTBOX_
#define _TEXTBOX_

#include <string.h>

class Textbox{
 public:
  std::string label;
  std::string text;
  int x,y,w,h;
  bool selected;
  int cursorI;//index of where the cursor is in the string
  
  Textbox();
  Textbox(std::string l,std::string t,int x1,int y1,int w1,int h1);
  
  //to make it work with just button presses
  void addLetter(char c);
  void backspace();
  
  void draw();
  
  bool inside(int x,int y);
};

#endif
