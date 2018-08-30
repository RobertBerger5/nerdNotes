#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

#include "Button.h"
#include "Note.h"
#include "glutFuncs.h"

using namespace std;

Button::Button(){
  label="___";
  x=y=0;
  w=h=10;

  //TODO: func
}

Button::Button(string l,int x1, int y1, int w1,int h1,void (*f)(Note& n)){
  label=l;
  x=x1;
  y=y1;
  w=w1;
  h=h1;
  func=f;
}

void Button::draw(){
  //glColor3f(1,1,1); commented out so buttons can be colored
  glBegin(GL_POLYGON);
  glVertex2f(x,y);
  glVertex2f(x+w,y);
  glVertex2f(x+w,y+h);
  glVertex2f(x,y+h);
  glEnd();

  glColor3f(0,0,0);
  drawText(label,x,y,false,w);//just have the title
  
  glBegin(GL_LINE_LOOP);
  glVertex2f(x,y);
  glVertex2f(x+w,y);
  glVertex2f(x+w,y+h);
  glVertex2f(x,y+h);
  glEnd();
  glutPostRedisplay();
}

bool Button::inside(int x1,int y1){
  if(
     x1>x &&
     x1<x+w &&
     y1>y &&
     y1<y+h
     ){
    return true;
  }
  return false;
}
