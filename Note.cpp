#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include "glutFuncs.h"
#include "Note.h"

using namespace std;

Note::Note(){
  title=quote=summary=importance="whom knows";
  tags={};
  //does tags start off initialized and okay???
  x=y=10;
  src=*(new Source("person"));
}

Note::Note(Source s){
  title=quote=summary=importance="";
  x=y=10;
  src=s;
}

Note::Note(string t,Source s,string quo,string sum,string imp,map<string,Tag> ta,int x1,int y1){
  title=t;//can I do this with strings???
  src=s;//???
  quote=quo;
  summary=sum;
  importance=imp;
  tags=ta;//TODO: IS THIS OKAY WITH mappy boi???
  x=x1;
  y=y1;//spawn in the top left
}

Note::Note(const Note& other){
  title=other.title;
  src=other.src;
  quote=other.quote;
  summary=other.summary;
  importance=other.importance;
  x=other.x;
  y=other.y;
  tags=other.tags;
}

bool Note::inside(int x1,int y1){
  return(
	 x1>x &&
	 x1<x+W &&
	 y1>y &&
	 y1<y+H
  );
}

void Note::printNote(){
  cout<<title<<endl;
  src.printSource();
  cout<<endl<<"Quote: "<<quote<<endl<<"Summary: "<<summary<<endl<<"Importance: "<<importance<<endl<<"Tags:"<<endl;
  /*for(string s : tags){
    cout<<"\t"<<s<<endl;
    }*/
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
  drawText(title,x,y,true,W);
  
  glBegin(GL_LINE_LOOP);
  glVertex2f(x,y);
  glVertex2f(x+W,y);
  glVertex2f(x+W,y+H);
  glVertex2f(x,y+H);
  glEnd();
  glutPostRedisplay();
}

void Note::addTag(string s){
  if(tags.count(s)>0){
    cout<<"already exists"<<endl;
  }else{
    Tag tag=*(new Tag(s));
    tags.insert(pair<string,Tag>(s,tag));
  }
  //check if it's already a thing, if not then add it to the vector
  /*for(string s : tags){
    if(!str.compare(s)){//string.compare() returns 0 if they're equal
      cout<<title<<" already has tag \""<<str<<"\""<<endl;
      return;
    }
  }
  tags.push_back(str);*/
  cout<<"Added tag \""<<s<<"\" to "<<title<<endl;
}

void Note::removeTag(string str){
  tags.erase(str);
  /*int i=0;//I feel like this is a weird way of doing it, but oh well...
  for(string s : tags){
    if(!str.compare(s)){
      tags.erase(tags.begin()+i);
      cout<<"Removed "<<str<<" from "<<title<<endl;
      return;
    }
    i++;
  }
  cout<<title<<" didn't have tag "<<str<<endl;*/
}


Tag::Tag(){
  //dunno
}
Tag::Tag(std::string n){
  name=n;
}
Tag::Tag(const Tag& other){
  name=other.name;
  notes=other.notes;
}
