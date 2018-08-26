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
string fileDir,saveDir;//file locations to save and open nerdNotes files
//bool mouseDragging=false;pretty sure this is useless now
int xStart,yStart,xPtr,yPtr;//where the mouse clicked down last and where it is now
Note* dragN=0;//the note that is being dragged
int dragX=0;//where mouse was relative to note's upper left corner when clicked
int dragY=0;
bool editing=false;
Note* editN=0;

Textbox* editT=0;

vector<Note> notes={};
vector<Source> sources={};
vector<Button> buttons={};
vector<Textbox> texts={};
vector<Note>::iterator nI;
vector<Note>::reverse_iterator rnI;
vector<Source>::iterator sI;
vector<Button>::iterator bI;
vector<Textbox>::iterator tI;

vector<Button> editButtons={};
vector<Textbox> editTexts={};
vector<Button>::iterator ebI;
vector<Textbox>::iterator etI;


void drawWindow();
void reshape(int w,int h);
void keyboard(unsigned char c,int x,int y);
void keyboardSpecial(int key, int x, int y);
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
  glutSpecialFunc(keyboardSpecial);
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
  cout<<c<<endl;
  if(c==27){
    cout<<"exiting..."<<endl;
    glutDestroyWindow(glutGetWindow());
    exit(0);
  }
  
  if(!editing){//just ignore individual buttons while editing
    switch(c){
    /*case 'q':
    case 'Q':
      cout<<"exiting..."<<endl;
      //TODO: is this safe???
      glutDestroyWindow(glutGetWindow());
      exit(0);
      break;*/
    default:
      break;
    }
  }else{
    if(c!='\\')
      editT->addLetter(c);
    else
      editT->backspace();
  }
  glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y){
  if(editing){
    switch(key){
    case GLUT_KEY_LEFT:
      editT->cursorI--;
    case GLUT_KEY_RIGHT:
      editT->cursorI++;
    default:
      break;
    }
  }
}

void mouse(int button, int state, int x, int y){
  if(GLUT_LEFT_BUTTON==button){
    if(GLUT_DOWN==state){
      //left pressed
      xStart=x;
      yStart=y;
      //mouseDragging=true;
      //logic for notes first?
      if(!editing){
	for(rnI=notes.rbegin();rnI!=notes.rend();++rnI){
	  if(
	     x>rnI->x &&
	     x<rnI->x+rnI->W &&
	     y>rnI->y &&
	     y<rnI->y+rnI->H
	     ){
	    dragN=&(*rnI);//wow this looks reduntant, yay for iterators
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
	for(ebI=editButtons.begin();ebI!=editButtons.end();ebI++){
	  if(ebI->inside(x,y)){
	    ebI->func(notes[0]);
	  }
	}
	
	for(etI=editTexts.begin();etI!=editTexts.end();etI++){
	  if(etI->inside(x,y)){
	    etI->selected=true;
	    editT=&(*etI);
	    etI->findCursor(x,y);
	  }
	}
      }
    }else{
      //left released
      //mouseDragging=false;
      dragN=0;
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
  if(dragN!=0){
    dragN->x=x+dragX;
    dragN->y=y+dragY;
  }else{
    //TODO: would be nice to just drag everything around at once kinda, ya know?
  }
  
  glutPostRedisplay();
}

void mouseMotionPassive(int x,int y){
  xPtr=x;
  yPtr=y;
}

void drawWindow(){
  glClear(GL_COLOR_BUFFER_BIT);
  //draw everything...
  if(!editing){
    glColor3f(.5,.5,.5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10,10);
    glVertex2f(10+Note::W,10);
    glVertex2f(10+Note::W,10+Note::H);
    glVertex2f(10,10+Note::H);
    glEnd();
    glColor3f(.01,.01,.01);//somehow this DOES NOT want to be light
    drawText("(new notes will spawn here when created)",10,10,false,Note::W);
    
    for(bI=buttons.begin();bI!=buttons.end();++bI){
      if(bI->inside(xPtr,yPtr)){
	glColor3f(.9,.9,.9);
      }else{
	glColor3f(1,1,1);
      }
      
      bI->draw();
    }

    
    //don't even have any non-editing textboxes now...
    for(tI=texts.begin();tI!=texts.end();tI++){
      if(tI->selected){
	glColor3f(1,0,0);
      }else{
	glColor3f(0,0,0);
      }
      tI->draw();
    }
    
    //always draw the notes last I think...
    for(nI=notes.begin();nI!=notes.end();++nI){
      nI->draw();
    }
    
  }else{
    //TODO: EDITING MENU
    for(etI=editTexts.begin();etI!=editTexts.end();etI++){
      if(etI->selected){
	glColor3f(1,0,0);
      }else{
	glColor3f(0,0,0);
      }
      etI->draw();
    }
    
    for(ebI=editButtons.begin();ebI!=editButtons.end();++ebI){
      if(ebI->inside(xPtr,yPtr)){
	glColor3f(.9,.9,.9);
      }else{
	glColor3f(1,1,1);
      }
      
      ebI->draw();
    }
  }
  
  glutSwapBuffers();
}

void editNote(Note& n){
  editing=true;
  editN=&n;
  cout<<"editing "<<n.title<<"..."<<endl;
}

void newNote(Note& n){
  //terrible practice, but imma just not do
  //anything with Note n, lmao rip
  notes.push_back(Note(sources[0]));//push an empty rig
  editNote(notes[notes.size()-1]);//edit the last one (that we just pushed in)
}

void saveNote(Note& n){
  //TODO: take everything from editTexts and shove it into n
  editN->title=editTexts[0].text;
  editN->quote=editTexts[1].text;
  editN->summary=editTexts[2].text;
  editN->importance=editTexts[3].text;
  editing=false;
}

int main(int argc,char*argv[]){
  for(int i=0;i<argc;i++){
    cout<<argv[i]<<endl;
  }
  
  sources.push_back(Source("(please select a note...)"));//default source (kinda)
  //should probably just have a check if notes.size()>0, if not then just complain
  
  for(int i=0;i<0;i++){
    notes.push_back(Note("titlerig",sources[0],"quoterig","sumrig","imprig",{"tag","rig","please work"}));
  }
  
  buttons.push_back(Button("New Note",50,HEIGHT-100,100,50,newNote));
  //buttons.push_back(Button("toggle edit",50,50,50,30,toggleEdit)); doesn't work anyway, can't click buttons while editing lol dumbo
  
  
  //editTexts.push_back(Textbox("label","text here",50,150,100,30));
  int twidth=700;
  int linesize=20;//height of a line
  int lines=1;
  int xstart=100;//how far over they start
  editTexts.push_back(Textbox("Title","Title of the rigs",xstart,10,twidth,linesize));//takes up one line
  lines+=2;
  editTexts.push_back(Textbox("Quote","legend has it that aaaaaaaa.",xstart,linesize*(lines),twidth,linesize*10));
  lines+=11;
  editTexts.push_back(Textbox("Summary","theres a legend aaaa",xstart,linesize*(lines),twidth,linesize*5));
  lines+=6;
  editTexts.push_back(Textbox("So what?","important cuz aaa and aa good",xstart,linesize*(lines),twidth,linesize*3));
  lines+=4;
  
  editButtons.push_back(Button("Save Note",50,HEIGHT-100,100,50,saveNote));
  
  init_gl_window();
}
