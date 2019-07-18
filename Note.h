#ifndef _NOTE_
#define _NOTE_

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "Source.h"
class Tag;

class Note{
 public:
  static const int H=60;
  static const int W=100;
  std::string title;
  Source src;
  std::string quote;
  std::string summary;
  std::string importance;
  std::map<std::string,Tag> tags;
  int x,y;

  Note();
  Note(Source s);
  Note(std::string t,Source s,std::string quo,std::string sum,std::string imp,std::map<std::string,Tag> tags,int x1,int y1);
  Note(const Note& other);

  bool inside(int x1,int y1);
  void printNote();
  void draw();

  void addTag(std::string str);
  void removeTag(std::string str);

  void coutTags(){
    std::cout<<"HERE ARE THE TAGS: ";
    /*for(std::vector<std::string>::iterator i=tags.begin();i!=tags.end();++i){
      std::cout<<*i<<", ";
      }*/
    std::cout<<"jk"<<std::endl;
  }
};


class Tag{
 public:
  std::string name;
  std::map<std::string,Note> notes;//TODO: make em pointers, so when the tag is deleted, we can call deleteTag on that note and it'll be super easy and nice

  Tag();
  Tag(std::string n);
  Tag(const Tag& other);
};


#endif
