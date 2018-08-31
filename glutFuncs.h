#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <string.h>


static int WIDTH=1000;
static int HEIGHT=600;

static const int TAG_X=150;
static const int TAG_Y=HEIGHT-60;
static const int TAG_ADD_X=10;
static const int LETTER_W=8;
static const int LETTER_H=13;


inline void drawText(std::string str,int x,int y,bool big,int w=0){
  void** size;
  int maxchars=999999;//max amount of chars that can fit before overflowing
  int letterH;
  if(big){
    size=GLUT_BITMAP_9_BY_15;
    if(w!=0)
      maxchars=w/9-1;
    letterH=15;
  }else{
    size=GLUT_BITMAP_8_BY_13;
    if(w!=0)
      maxchars=w/8-1;
    letterH=13;
  }
  y+=letterH;
  
  glRasterPos2i(x+2,y);

  //it'd be nice if it broke up lines between words
  //nevermind, I have NO idea how that'd work with the clicking shit now...
  int over=0;
  int lines=0;
  for(int i=0;i<str.length();i++){
    over++;
    if(over>maxchars){
      over=0;
      lines++;
      glRasterPos2i(x+2,y+lines*letterH);
      if(str[i]==' ')//don't put a space at line starts
	i++;
    }

    glutBitmapCharacter(size,str[i]);
  }
}

inline void drawBox(int x,int y,int w,int h,bool fill){
  //color beforehand...
  if(fill)
    glBegin(GL_POLYGON);
  else
    glBegin(GL_LINE_LOOP);
  glVertex2f(x,y);
  glVertex2f(x+w,y);
  glVertex2f(x+w,y+h);
  glVertex2f(x,y+h);
  glEnd();
}

inline bool pointInside(int xPt,int yPt,int x,int y,int w,int h){
  return(
	 xPt>x &&
	 yPt>y &&
	 xPt<x+w &&
	 yPt<y+h
	 );
}
