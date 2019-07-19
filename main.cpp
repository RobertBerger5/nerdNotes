#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <string.h>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>

#include "glutFuncs.h"
#include "Note.h"
#include "Source.h"
#include "Button.h"
#include "Textbox.h"

using namespace std;

char progName[]="nerdNotes";
string fileDir="noname.nn";//file locations to save and open nerdNotes files
//bool mouseDragging=false;pretty sure this is useless now
int xStart,yStart,xPtr,yPtr;//where the mouse clicked down last and where it is now
//xStart=yStart=xPtr=yPtr=0;
Note* dragN=0;//the note that is being dragged
int dragX=0;//where mouse was relative to note's upper left corner when clicked
int dragY=0;
bool editing=false;
Note* editN=0;

Textbox* editT=0;

vector<Note> notes;
vector<Source> sources;
vector<Button> buttons;
vector<Textbox> texts;
//vector<string> allTags;

map<string,Tag> tags;

map<string,Tag>::iterator tagI;

vector<Note>::iterator nI;
vector<Note>::reverse_iterator rnI;
vector<Source>::iterator sI;
vector<Button>::iterator bI;

vector<Button> editButtons;
vector<Textbox> editTexts;
vector<Textbox>::iterator etI;


void drawWindow();
void reshape(int w,int h);
void keyboard(unsigned char c,int x,int y);
void keyboardSpecial(int key, int x, int y);
void mouse(int button,int state,int x,int y);
void mouseMotion(int x,int y);
void mouseMotionPassive(int x,int y);

void deleteTag(string t);
void highlightTag(string t);
void editNote(Note& n);
void saveFile(Note& n);

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
  //TODO: what are these even...
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

void reshape(int w,int h){//TODO: NOTHING MOVES RIGHT NOW
  glViewport(0,0,(GLsizei)w,(GLsizei)h);
  WIDTH=w;
  HEIGHT=h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.,WIDTH-1,HEIGHT-1,0.,-1.0,1.0);
}

void keyboard(unsigned char c,int x,int y){
  if(c==27){
    cout<<"exiting..."<<endl;
    //TODO: PROMPT TO SAVE FIRST?
    //saveFile(*(new Note()));
    glutDestroyWindow(glutGetWindow());
    exit(0);
  }
  
  if(!editing){//just ignore individual buttons while editing
    switch(c){
    default:
      break;
    }
  }else{
    switch(c){
    case 127:
      if(editT->cursorI>0)
	editT->backspace();
      break;
    default:
      editT->addLetter(c);
      break;
    }
    /*if(c==127)
      editT->backspace();
    else if(c!='\\')//this shit is backspace cuz the delete button is hiding from me
      editT->addLetter(c);
    else
    editT->backspace();*/
  }
  glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y){
  /*if(editing && editT){//now nothing here works without a Textbox being sleected
    switch(key){
      case GLUT_KEY_LEFT:
      if((editT->cursorI)>0)
	editT->cursorI=editT->cursorI-1;
      else
	cerr<<"no left"<<endl;
    case GLUT_KEY_RIGHT:
      if(editT->cursorI < editT->text.size()-1)
	editT->cursorI++;
      else
	cerr<<"no right"<<endl;
    default:
      break;
    }
    cout<<editT->cursorI<<endl;
    }*/
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
	//check if notes clicked
	for(rnI=notes.rbegin();rnI!=notes.rend();++rnI){
	  if(rnI->inside(x,y)){
	    dragN=&(*rnI);//wow this looks reduntant, yay for iterators
	    dragX=rnI->x-x;//offset, where the mouse was...
	    dragY=rnI->y-y;//relative to the card when clicked
	    break;
	  }
	}

	//check if buttons clicked
	for(bI=buttons.begin();bI!=buttons.end();++bI){
	  if(bI->inside(x,y)){
	    bI->func(notes[0]);
	  }
	}
	
	//check if tags area clicked
	if(pointInside(x,y,TAG_X,TAG_Y,WIDTH-TAG_X,LETTER_H+2)){
	  //in the right general area
	  int i=0;
	  int xT=TAG_X;
	  int yT=TAG_Y;
	  int w;
	  int h=LETTER_H+2;
	  for(tagI=tags.begin();tagI!=tags.end();++tagI,i++){
	    w=((*tagI).second.name.size())*LETTER_W;
	    if(pointInside(x,y,xT+w*2/3,yT,w/3,h)){
	      deleteTag(tagI->second.name);
	      break;
	    }else if(pointInside(x,y,xT,yT,w*2/3,h)){
	      highlightTag(tagI->second.name);
	      break;
	    }else{
	      xT+=w+TAG_ADD_X;
	    }
	  }
	}
	
      }else{
	//tryna click while editing
	for(bI=editButtons.begin();bI!=editButtons.end();++bI){
	  if(bI->inside(x,y)){
	    bI->func(notes[0]);
	  }
	}
	
	for(etI=editTexts.begin();etI!=editTexts.end();++etI){
	  if(etI->inside(x,y)){
	    if(editT)
	      editT->selected=false;
	    etI->selected=true;
	    editT=&(*etI);
	    etI->findCursor(x,y);
	  }
	}
      }
    }else{
      //left released
      //cycle through, if there's a note clicked and the mouse hasn't moved, then yippee
      if(!editing){
	for(nI=notes.begin();nI!=notes.end();++nI){
	  if((xStart==xPtr && yStart==yPtr) && nI->inside(x,y)){
	    editNote(*nI);
	  }
	}
	dragN=0;
	dragX=dragY=0;
      }
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
    drawBox(10,10,Note::W,Note::H,false);
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

    int i=0;
    int x=TAG_X;
    int y=TAG_Y;
    int w;
    int h=LETTER_H+2;
    for(tagI=tags.begin();tagI!=tags.end();++tagI,i++){//BIG HOSS
      w=((tagI->second.name).size())*LETTER_W;
      if(pointInside(xPtr,yPtr,x+w*2/3,y,w/3,h))
	glColor3f(1,0,0);
      else if(pointInside(xPtr,yPtr,x,y,w*2/3,h))
	glColor3f(1,1,0);
      else
	glColor3f(1,1,1);
      drawBox(x,y,w,h,true);
      glColor3f(0,0,0);
      drawBox(x,y,w,h,false);
      drawText(tagI->second.name,x-1,y-1,false);
      x+=w+TAG_ADD_X;
    }
    
    //always draw the notes last I think...
    for(nI=notes.begin();nI!=notes.end();++nI){
      nI->draw();
    }
    
  }else{
    for(etI=editTexts.begin();etI!=editTexts.end();++etI){
      if(etI->selected){
	glColor3f(1,0,0);
      }else{
	glColor3f(0,0,0);
      }
      etI->draw();
    }
    
    for(bI=editButtons.begin();bI!=editButtons.end();++bI){
      if(bI->inside(xPtr,yPtr)){
	glColor3f(.9,.9,.9);
      }else{
	glColor3f(1,1,1);
      }
      
      bI->draw();
    }
  }
  
  glutSwapBuffers();
}

bool addTag(string s){//returns false if tag already exists
  if(tags.count(s)>0){
    cout<<"already exists"<<endl;
    return false;
  }else{
    Tag tag=*(new Tag(s));
    tags.insert(pair<string,Tag>(s,tag));
    cout<<"inserted "<<s<<endl;
    return true;
  }
}

     /*void addGlobalTag(string t){
  //search allTags, if t isn't in there, push_back that rig
  for(tagI=allTags.begin();tagI!=allTags.end();++tagI){
    if(t== *tagI)
      return;
  }
  allTags.push_back(t);
}*/

void deleteTag(string t){//deletes the tag from allTags, then from all Notes
  //maybe it should highlight all notes you just deleted it from?
  cout<<"gotta delete "<<t<<endl;
}

void highlightTag(string t){
  //loop through Notes, change background color if they have this tag
  cout<<"gotta highlight "<<t<<endl;
}

void editNote(Note& n){//TODO: REMOVE GLOBAL TAGS IF THEY ONLY EXISTED FOR THAT NOTE
  editing=true;
  editN=&n;
  editTexts[0].text=editN->title;
  editTexts[1].text=editN->quote;
  editTexts[2].text=editN->summary;
  editTexts[3].text=editN->importance;
  for(tagI=editN->tags.begin(); tagI!=prev(editN->tags.end()); ++tagI){
    editTexts[4].text+= tagI->second.name +", ";
  }
  editTexts[4].text+= (prev(editN->tags.end()))->second.name;
  //I have editT but I'm too lazy to use it...eh
  for(etI=editTexts.begin();etI!=editTexts.end();++etI){
    etI->selected=false;
  }
}

void deleteNote(Note& n){
  for(nI=notes.begin();nI!=notes.end();++nI){
    if(&(*nI)==&n){
      notes.erase(nI);
      editing=false;
      editN=0;
    }
  }
}

void newNote(Note& n){//TODO: THIS FUCKS ITSELF WITH A NEW FILE, PROBABLY CUZ sources[0] AINT A THING!
  //terrible practice, but imma just not do
  //anything with Note n, lmao rip
  notes.push_back(Note(sources[0]));//push an empty rig
  editNote(notes[notes.size()-1]);//edit the last one (that we just pushed in)
}

void saveNote(Note& n){
  editN->title=editTexts[0].text;
  editN->quote=editTexts[1].text;
  editN->summary=editTexts[2].text;
  editN->importance=editTexts[3].text;
  
  
  //internet people are saying to use boost's algorithm for splitting, but imma do it my own slow way
  editN->tags={};//clear em out
  string tagString=editTexts[4].text;
  string newTag="";//individual tag to get pushed a ton
  for(int i=0;i<tagString.size();i++){
    if(tagString[i]!=','){//not a comma, so add it to the current tag
      newTag+=tagString[i];
    }else{
      //editN->tags.push_back(newTag);//found comma, so push what we have into a tag
      editN->addTag(newTag);
      addTag(newTag);
      //addGlobalTag(newTag);
      newTag="";//clear it for the next tag
      if(tagString[i+1]==' ')
	i++;//skip space
    }
  }
  //push whatever's left over in tagString
  //editN->tags.push_back(newTag);
  editN->addTag(newTag);
  addTag(newTag);
  //addGlobalTag(newTag);
  
  
  editing=false;
  for(etI=editTexts.begin();etI!=editTexts.end();++etI){
    etI->text="";//clear em out (just to be sure)
  }
}

void saveFile(Note& n){//kinda annoying that I set it up for all button-able funcs to have this arg
  ofstream outStream;
  outStream.open(fileDir);
  //if(!outStream)
    //TODO:somehow push somethin here
  outStream<<sources.size()<<endl;
  for(sI=sources.begin();sI!=sources.end();sI++){
    outStream<<sI->author<<endl;
  }
  outStream<<notes.size()<<endl;
  for(nI=notes.begin();nI!=notes.end();++nI){
    outStream<<nI->title<<endl;
    outStream<<0<<endl;//TODO: SOURCE NUMBER!
    outStream<<nI->quote<<endl;
    outStream<<nI->summary<<endl;
    outStream<<nI->importance<<endl;
    outStream<<nI->tags.size()<<endl;
    for(tagI=nI->tags.begin();tagI!=nI->tags.end();++tagI)
      outStream<<tagI->second.name<<endl;
    outStream<<nI->x<<endl;
    outStream<<nI->y<<endl;
  }

  outStream.close();

  cout<<"file saved to "<<fileDir<<endl;
}

void openFile(string l){//this one ain't gonna be in a button...I think...
  ifstream inStream;
  inStream.open(fileDir);
  if(!inStream)//opened a new file/project, let em be
    return;
  
  string sourceNum;
  string auth;
  getline(inStream,sourceNum);
  for(int i=0;i<stoi(sourceNum);i++){
    getline(inStream,auth);
    //TODO: when I get more Source things...
    sources.push_back(Source(auth));
  }
  
  string noteNum;
  getline(inStream,noteNum);
  //various strings here...
  string tit;
  string src;
  string quo;
  string sum;
  string imp;
  string tagNum;
  map<string,Tag> newTags;
  string t;
  string x;
  string y;
  for(int i=0;i<stoi(noteNum);i++){
    getline(inStream,tit);
    getline(inStream,src);
    getline(inStream,quo);
    getline(inStream,sum);
    getline(inStream,imp);
    getline(inStream,tagNum);
    for(int j=0;j<stoi(tagNum);j++){
      getline(inStream,t);
      Tag ta=*(new Tag(t));
      newTags.insert(pair<string,Tag>(t,ta));
      addTag(t);
    }
    getline(inStream,x);
    getline(inStream,y);
    notes.push_back(Note(tit,sources[stoi(src)],quo,sum,imp,newTags,stoi(x),stoi(y)));
    newTags.clear();
  }
  inStream.close();

  /*cerr<<"gonna get the tags"<<endl;
  for(nI=notes.begin();nI!=notes.end();++nI){
    /*cerr<<"in "<<nI->title<<endl;//TODO: why on earth does this not work???
    for(tagI=nI->tags.begin(); tagI!=nI->tags.end(); ++tagI){
      cerr<<"adding "<<(*tagI)<<endl;
      addGlobalTag(*tagI);
      cerr<<"added"<<endl;
      }*
    editNote(*nI);//VERY stupid workaround bc it works when I click on the note to edit, then save
    saveNote(*nI);//wow this is bad...
  }*/
  
}

int main(int argc,char*argv[]){
  
  //sources.push_back(Source("(please select a note...)"));//default source (kinda)
  //should probably just have a check if notes.size()>0, if not then just complain
  
  buttons.push_back(Button("New Note",10,HEIGHT-60,100,50,newNote));
  buttons.push_back(Button("Save File",10,HEIGHT-120,100,50,saveFile));
  
  //editTexts.push_back(Textbox("label","text here",50,150,100,30));
  int linesize=20;//height of a line
  int lines=1;
  int xstart=100;//how far over they start
  int twidth=WIDTH-xstart-10;
  editTexts.push_back(Textbox("Title","these should all be overwritten...",xstart,10,twidth,linesize));//takes up one line
  lines+=2;
  editTexts.push_back(Textbox("Quote","?",xstart,linesize*(lines),twidth,linesize*5));
  lines+=6;
  editTexts.push_back(Textbox("Summary","??",xstart,linesize*(lines),twidth,linesize*5));
  lines+=6;
  editTexts.push_back(Textbox("So what?","???",xstart,linesize*(lines),twidth,linesize*3));
  lines+=4;
  editTexts.push_back(Textbox("Tags","",xstart,linesize*(lines),twidth,linesize));
  
  
  editButtons.push_back(Button("Save Note",10,HEIGHT-60,100,50,saveNote));
  editButtons.push_back(Button("Delete Note",10,HEIGHT-120,100,50,deleteNote));
  
  if(argc==2){
    fileDir=argv[1];
    openFile(fileDir);
  }else{
    cout<<"ERROR: Please enter exactly one file in the command line\n";
    cout<<"Enter it directly after \"./nerdNotes,\" thank you."<<endl;
  }
  
  init_gl_window();
}

/*
big ol TODO list:
  
  make/edit Sources
  give Notes a Source (with a dropdown?)

  maybe list the already existing tags when editing so they can see what they've got...
  
  highlight Notes by tag
  
  remove global tags (and remove the tag from all Notes that had it (maybe highlight em red afterwards?)
  
  have a "Delete Note" editButton
  
 */
