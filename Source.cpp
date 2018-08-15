#include <iostream>
#include <string.h>

#include "Source.h"

using namespace std;

Source::Source(){
  author="ass";
}

Source::Source(string auth){
  author=auth;
}

Source& Source::operator=(const Source& other){
  author=other.author;//TODO: when I add more shit, add it here too...
  return *this;
}

void Source::printSource(){
  cout<<"Written by "<<author<<", I should really add more things to the source fuckery";
}
