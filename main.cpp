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
#include "Button.h"
#include "Textbox.h"


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
bool editing=false;

vector<Note> notes={};
vector<Source> sources={};
vector<Button> buttons={};
vector<Textbox> texts={};
vector<Note>::iterator nI;
vector<Note>::reverse_iterator rnI;
vector<Source>::iterator sI;
vector<Button>::iterator bI;
vector<Textbox>::iterator tI;

void drawWindow();
void reshape(int w,int h);
void keyboard(unsigned char c,int x,int y);
void mouse(int button,int state,int x,int y);
void mouseMotion(int x,int y);
void mouseMotionPassive(int x,int y);

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
  glutMotionFunc(mouseMotion);
  glutPassiveMotionFunc(mouseMotionPassive);
  
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

void keyboard(unsigned char c,int x,int y){
  if(!editing){//just ignore individual buttons while editing
    switch(c){
    case 'q':
    case 'Q':
      cout<<"exiting..."<<endl;
      //TODO: is this safe???
      glutDestroyWindow(glutGetWindow());
      exit(0);
      break;
    default:
      break;
    }
  }else{
    //TODO: register that they're tryna type something while editing
    for(tI=texts.begin();tI!=texts.end();tI++){
      if(tI->selected){
	tI->addLetter(c);
      }
    }
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
  if(GLUT_LEFT_BUTTON==button){
    if(GLUT_DOWN==state){
      //left pressed
      xStart=x;
      yStart=y;
      mouseDragging=true;
      //logic for notes first?
      if(!editing){
	for(rnI=notes.rbegin();rnI!=notes.rend();++rnI){//TODO: should have it go in reverse
	  if(
	     x>rnI->x &&
	     x<rnI->x+rnI->W &&
	     y>rnI->y &&
	     y<rnI->y+rnI->H
	     ){
	    drag=&(*rnI);//wow this looks reduntant, yay for iterators...
	    dragX=rnI->x-x;//offset, where the mouse was...
	    dragY=rnI->y-y;//relative to the card when clicked
	    break;
	  }
	}
	
	for(bI=buttons.begin();bI!=buttons.end();bI++){
	  if(bI->inside(x,y)){
	    bI->func(notes[0]);
	  }
	}
      }else{
	//tryna click while editing
	for(tI=texts.begin();tI!=texts.end();tI++){
	  if(tI->inside(x,y)){
	    tI->selected=true;
	  }
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

void mouseMotion(int x,int y){
  xPtr=x;
  yPtr=y;
  //if(mouseDragging){
  if(drag!=0){
    drag->x=x+dragX;
    drag->y=y+dragY;
  }else{
    //TODO: would be nice to just drag everything around at once kinda, ya know?
  }
  //}
  
  glutPostRedisplay();
}

void mouseMotionPassive(int x,int y){
  xPtr=x;
  yPtr=y;
}

void drawWindow(){
  glClear(GL_COLOR_BUFFER_BIT);
  //draw everything...
  glColor3f(.5,.5,.5);
  glBegin(GL_LINE_LOOP);
  glVertex2f(10,10);
  glVertex2f(10+Note::W,10);
  glVertex2f(10+Note::W,10+Note::H);
  glVertex2f(10,10+Note::H);
  glEnd();
  glColor3f(.01,.01,.01);//somehow this DOES NOT want to be light
  drawText("(notes spawn here)",15,30,0);
  
  for(bI=buttons.begin();bI!=buttons.end();++bI){
    if(bI->inside(xPtr,yPtr)){
      glColor3f(.9,.9,.9);
    }else{
      glColor3f(1,1,1);
    }
    
    bI->draw();
  }

  //always draw the notes last I think...
  for(nI=notes.begin();nI!=notes.end();++nI){
    nI->draw();
  }

  //TODO: should be under if(editing){...}
  for(tI=texts.begin();tI!=texts.end();tI++){
    if(tI->selected){
      glColor3f(1,0,0);
    }else{
      glColor3f(0,0,0);
    }
    tI->draw();
  }
  
  //unless editing...
  if(editing){
    //TODO: DRAW FUCKIN EVERYTHING
  }
  
  glutSwapBuffers();
}

void editNote(Note n){
  editing=true;
  
}

void newNote(Note n){
  //terrible practice, but imma just not do
  //anything with Note n, lmao rip
  notes.push_back(Note(sources[0]));//push an empty rig
  editNote(notes[notes.size()-1]);//edit the last one (that we just pushed in)
}

/*void toggleEdit(Note n){
  cout<<"toggling"<<endl;
  editing=(!editing);
  }*/

int main(int argc,char*argv[]){
  for(int i=0;i<argc;i++){
    cout<<argv[i]<<endl;
  }
  
  sources.push_back(Source("(none)"));//default source (kinda)
  //should probably just have a check if notes.size()>0, if not then just complain
  
  for(int i=0;i<5;i++){
    notes.push_back(Note("titlerig",sources[0],"quoterig","sumrig","imprig",{"tag","rig","please work"}));
  }
  
  buttons.push_back(Button("New Note",50,100,50,30,newNote));
  //buttons.push_back(Button("toggle edit",50,50,50,30,toggleEdit)); doesn't work anyway, can't click buttons while editing lol dumbo

  texts.push_back(Textbox("text here",50,150,100,30));
  
  init_gl_window();
}
