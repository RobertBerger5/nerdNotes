#ifndef _NOTE_
#define _NOTE_

#include <string.h>
#include <vector>

#include "Source.h"


class Note{
 public:
  static const int H=60;
  static const int W=100;
  std::string title;
  Source src;
  std::string quote;
  std::string summary;
  std::string importance;
  std::vector<std::string> tags;
  int x,y;

  Note();
  Note(Source s);
  Note(std::string t,Source s,std::string quo,std::string sum,std::string imp,std::vector<std::string> tags);
  Note(const Note& other);

  bool inside(int x1,int y1);
  void printNote();
  void draw();

  void addTag(std::string str);
  void removeTag(std::string str);

  void coutTags(){
    std::cout<<"HERE ARE THE TAGS: ";
    for(std::vector<std::string>::iterator i=tags.begin();i!=tags.end();++i){
      std::cout<<*i<<", ";
    }
    std::cout<<std::endl;
  }
};

#endif
