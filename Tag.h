#ifndef _TAG_
#define _TAG_

#include <string>
#include <vector>
#include <map>

#include "Note.h"

class Tag{
 public:
  std::string name;
  std::map<std::string,Note> notes;//TODO: make em pointers, so when the tag is deleted, we can call deleteTag on that note and it'll be super easy and nice

  Tag();
  Tag(std::string n);
  Tag(const Tag& other);
};

#endif
