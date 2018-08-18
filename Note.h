#ifndef _NOTE_
#define _NOTE_

#include <string.h>
#include <vector>

#include "Source.h"


class Source;

using namespace std;

class Note{
 public:
  static const int H=60;
  static const int W=100;
  string title;
  Source src;
  string quote;
  string summary;
  string importance;
  vector<string> tags;
  int x,y;

  Note();
  Note(Source s);
  Note(string t,Source s,string quo,string sum,string imp,vector<string> tags);
  Note(const Note& other);

  void printNote();
  void draw();

  void addTag(string str);
  void removeTag(string str);
};

#endif
