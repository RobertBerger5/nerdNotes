#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

#include "Textbox.h"
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
  cursorI=text.size();
}

void Textbox::addLetter(char c){
  //text+=c;
  text.insert(cursorI,1,c);
  cursorI++;
}

void Textbox::backspace(){
  if(text.size()>0)
    text.erase(cursorI-1,1);
  cursorI--;
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
  
  //draw cursor
  if(selected){
    int letPerLine=w/LETTER_W;
    int cX=cursorI%letPerLine*LETTER_W +x;//cursor x and y
    int cY=cursorI/letPerLine*LETTER_H +y;
    glColor3f(.5,.5,.5);
    glBegin(GL_POLYGON);
    glVertex2f(cX,cY);
    glVertex2f(cX+LETTER_W,cY);
    glVertex2f(cX+LETTER_W,cY+LETTER_H);
    glVertex2f(cX,cY+LETTER_H);
    glEnd();
  }
  
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

void Textbox::findCursor(int x1,int y1){
  int letPerLine=w/LETTER_W;
  
  //step 1: find the general range of numbers it can be in with the y coord
  //step 2: use mod or somethin to get which one from the x coord
  cursorI=((y1-y)/LETTER_H)*letPerLine;
  cursorI+=(x1-x)/LETTER_W;
  
  if(cursorI>text.size())
    cursorI=text.size();
}
