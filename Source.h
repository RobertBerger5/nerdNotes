#ifndef _SOURCE_
#define _SOURCE_

#include <string.h>

using namespace std;

//TODO: not gonna bother with MLA/Chicago shittery probably, just (author),"(title)"...
class Source{
 public:
  std::string author;
  //TODO: there's a fuckton of other stuff too...

  Source();
  Source(std::string auth);

  Source& operator=(const Source& other);

  void printSource();
};
#endif
