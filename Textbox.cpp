#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

#include "Textbox.h"
//#include "Note.h"
#include "glutFuncs.h"

using namespace std;

Textbox::Textbox(){
  label="?: ";
  text="";
  x=y=0;
  w=100;
  h=20;
  selected=false;
}

Textbox::Textbox(string l,string t,int x1,int y1,int w1,int h1){
  label=l+": ";
  text=t;
  x=x1;
  y=y1;
  w=w1;
  h=h1;
  selected=false;
}

void Textbox::addLetter(char c){
  text+=c;
}

void Textbox::backspace(){
  if(text.size()>0)
    text.erase(text.end()-1);
}

void Textbox::draw(){
  //before calling, change color
  //red if selected, black if not
  //border color
  glBegin(GL_POLYGON);
  glVertex2f(x,y);
  glVertex2f(x+w,y);
  glVertex2f(x+w,y+h);
  glVertex2f(x,y+h);
  glEnd();
  
  //inner box
  int thicc=1;
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
  glVertex2f(x+thicc,y+thicc);
  glVertex2f(x+w-thicc*2,y+thicc);
  glVertex2f(x+w-thicc*2,y+h-thicc*2);
  glVertex2f(x+thicc,y+h-thicc*2);
  glEnd();

  glColor3f(.5,.5,.5);
  glBegin(GL_POLYGON);
  glVertex2f(0,0);//TODO: DRAW CURSOR SOMEHOW!!!
  glEnd();

  glColor3f(0,0,0);
  drawText(text,x,y,false,w);
  drawText(label,x-8*label.size(),y,false);
  
  glutPostRedisplay();
}

bool Textbox::inside(int x1,int y1){
  return(
     x1>x &&
     x1<x+w &&
     y1>y &&
     y1<y+h
	 );
}
