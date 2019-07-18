#ifndef _NOTE_
#define _NOTE_

#include <string.h>
#include <vector>
#include <map>

#include "Note.h"

class Tag{
 public:
  std::string name;
  map<string,Note> notes;

  Tag(){
    //dunno what to do here...
  }
  Tag(std::string n){
    name=n;
  }
  Tag(const Tag& other){
    name=other.name;
    notes=other.notes;
  }
};

#endif
