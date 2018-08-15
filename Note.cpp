#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include <iostream>

#include "glutFuncs.h"
#include "Note.h"
#include "Source.h"

using namespace std;

Note::Note(){
  title=quote=summary=importance="whom knows";
  tags={};
  //does tags start off initialized and okay???
  x=y=0;
  src=*(new Source("person"));
}

Note::Note(string t,Source s,string quo,string sum,string imp,vector<string> ta){
  title=t;//can I do this with strings???
  src=s;//???
  quote=quo;
  summary=sum;
  importance=imp;
  tags=ta;//TODO: IS THIS OKAY WITH VECTORS???
  x=y=10;//spawn in the top left?
}

Note::Note(const Note& other){
  title=other.title;
  src=other.src;//TODO: Source::operator=
  quote=other.quote;
  summary=other.summary;
  importance=other.importance;
  x=other.x;
  y=other.y;
  for(int i=0;i<tags.size();i++){
    tags[i]=other.tags[i];
  }
}

void Note::printNote(){
  cout<<title<<endl;
  src.printSource();
  cout<<endl<<"Quote: "<<quote<<endl<<"Summary: "<<summary<<endl<<"Importance: "<<importance<<endl<<"Tags:"<<endl;
  for(string s : tags){
    cout<<"\t"<<s<<endl;
  }
}

void Note::draw(){
  glColor3f(1,1,1);//white background
  glBegin(GL_POLYGON);
  glVertex2f(x,y);
  glVertex2f(x+W,y);
  glVertex2f(x+W,y+H);
  glVertex2f(x,y+H);
  glEnd();

  glColor3f(0,0,0);
  drawText(title,x,y+18,2);//just have the title
  //drawText(quote,x+5,y+25,0);
  /* size 18 is 14px under the top...
     sizes 12 is 9px under
     size 10 is 8px under
   */
  
  glBegin(GL_LINE_LOOP);
  glVertex2f(x,y);
  glVertex2f(x+W,y);
  glVertex2f(x+W,y+H);
  glVertex2f(x,y+H);
  glEnd();
  glutPostRedisplay();
}

void Note::addTag(string str){
  //check if it's already a thing, if not then add it to the vector
  for(string s : tags){
    if(!str.compare(s)){//string.compare() returns 0 if they're equal
      cout<<title<<" already has tag \""<<str<<"\""<<endl;
      return;
    }
  }
  tags.push_back(str);
  cout<<"Added tag \""<<str<<"\" to "<<title<<endl;
}

void Note::removeTag(string str){
  int i=0;//I feel like this is a weird way of doing it, but oh well...
  for(string s : tags){
    if(!str.compare(s)){
      tags.erase(tags.begin()+i);
      cout<<"Removed "<<str<<" from "<<title<<endl;
      return;
    }
    i++;
  }
  cout<<title<<" didn't have tag "<<str<<endl;
}