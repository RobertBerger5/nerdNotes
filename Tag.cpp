#include "Tag.h"

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
