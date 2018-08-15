#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <string.h>

using namespace std;

static void drawText(string str,int x,int y,int size){//MAKING IT STATIC WORKED FOR SOME REASON
  //TODO: wrap if over a certain width, go to "..." if over a certain height
  glRasterPos2i(x,y);
  //glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,str);
  for(int i=0;i<str.length();i++){
    switch(size){
    case 0:
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
      break;
    case 1:
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
      break;
    case 2:
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
      break;
    default:
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
      break;
    }
  }
}
