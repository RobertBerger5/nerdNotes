#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <string.h>
#include <fstream>
#include <iterator>

#include "glutFuncs.h"
#include "Note.h"
#include "Source.h"


using namespace std;


int WIDTH=1000;
int HEIGHT=600;
char progName[]="nerdNotes";
string fileDir,saveDir;
bool mouseDragging=false;
int xStart,yStart,xPtr,yPtr;
Note* drag=0;
int dragX=0;//for offset of draggage...
int dragY=0;

vector<Note> notes={};
vector<Source> sources={};
vector<Note>::iterator nI;
vector<Note>::reverse_iterator rnI;

void drawWindow();
void reshape(int w,int h);
void keyboard(unsigned char c,int x,int y);
void mouse(int button,int state,int x,int y);
void mouse_motion(int x,int y);

void init(void){
  glClearColor(50,50,50,1);
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,WIDTH-1,HEIGHT-1,0,-1,1);

  cout<<"what up dude, welcome to "<<progName<<endl;
}

void init_gl_window(){
  //TODO: wtf are these even...
  char*argv[]={progName};
  int argc=sizeof(argv)/sizeof(argv[0]);

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH,HEIGHT);
  glutInitWindowPosition(0,0);
  glutCreateWindow(progName);
  
  init();
  
  glutDisplayFunc(drawWindow);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);

  glutMainLoop();
}

void reshape(int w,int h){
  glViewport(0,0,(GLsizei)w,(GLsizei)h);
  WIDTH=w;
  HEIGHT=h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.,WIDTH-1,HEIGHT-1,0.,-1.0,1.0);
}


void DrawBox(int x,int y,int w,int h,bool fill){
  if(fill)
    glBegin(GL_POLYGON);
  else
    glBegin(GL_LINE_LOOP);
  glLineWidth(20);
  glVertex2f(x,y);
  glVertex2f(x+w,y);
  glVertex2f(x+w,y+h);
  glVertex2f(x,y+h);
  glEnd();
}

void keyboard(unsigned char c,int x,int y){
  switch(c){
  case 'e':
    cout<<"You have pressed e, congratulations"<<endl;
    break;
  case 'q':
  case 'Q':
    cout<<"exiting..."<<endl;
    //TODO: is this safe???
    glutDestroyWindow(glutGetWindow());
    exit(0);
    break;
  default:
    cout<<"something other than e pressed, :("<<endl;
    break;
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
  if(GLUT_LEFT_BUTTON==button){
    if(GLUT_DOWN==state){
      //left pressed
      xStart=x;
      yStart=y;
      xPtr=x;
      yPtr=y;
      mouseDragging=true;
      //TODO
      for(nI=notes.begin();nI!=notes.end();++nI){
	//Note n=notes[i];
	if(
	   x>nI->x &&
	   x<nI->x+nI->W &&
	   y>nI->y &&
	   y<nI->y+nI->H
	){
	  drag=&(*nI);//wow this looks reduntant, yay for iterators...
	  dragX=nI->x-x;//offset, where the mouse was...
	  dragY=nI->y-y;//relative to the card when clicked
	  break;
	}
      }
    }else{
      //left released
      mouseDragging=false;
      drag=0;
      dragX=dragY=0;
    }
  }else{
    if(GLUT_DOWN==state){
      //right pressed
    }else{
      //right released
    }
  }
  
  glutPostRedisplay();
}

void mouse_motion(int x,int y){
  if(mouseDragging){
    if(drag!=0){
      drag->x=x+dragX;
      drag->y=y+dragY;
    }
  }
  
  glutPostRedisplay();
}

void drawWindow(){
  glClear(GL_COLOR_BUFFER_BIT);
  //draw everything...
  glColor3f(0,0,0);
  
  for(rnI=notes.rbegin();rnI!=notes.rend();++rnI){
    rnI->draw();
  }
  
  glutSwapBuffers();
}

int main(int argc,char*argv[]){
  for(int i=0;i<argc;i++){
    cout<<argv[i]<<endl;
  }

  sources.push_back(Source("authordudehere"));//just one source so far...
  
  for(int i=0;i<5;i++){
    notes.push_back(Note("title",sources[0],"your boobs are good/10.","how good","the author is rating tits, very neat",{"boob","rating","Nice."}));
  }
  
  init_gl_window();
}
