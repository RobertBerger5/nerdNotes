OPTS = -Wall -g -std=c++11 -Wno-deprecated
LIBS = -lGL -lglut -lm
ARCH := $(shell uname)
ifeq ($(ARCH), Linux)
else
 MACOSX_DEFINE = -DMACOSX -I/sw/include
 LIBS = -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -framework Cocoa #-lGL -lm -lobjc -lstdc++ -lGLU lGL #-lglut
endif

nerdNotes:	Note.o Source.o Button.o Textbox.o main.o
	g++ $(OPTS) -o nerdNotes Note.o Source.o Button.o Textbox.o main.o $(LIBS)

main.o:	main.cpp Note.h Source.h glutFuncs.h Button.h Textbox.h
	g++ $(OPTS) $(MACOSX_DEFINE) -c main.cpp

Note.o:	Note.h Note.cpp Source.h glutFuncs.h
	g++ $(OPTS) -c Note.cpp

Source.o:	Source.h Source.cpp Note.h
	g++ $(OPTS) -c Source.cpp

Button.o:	Button.h Button.cpp Note.h glutFuncs.h
	g++ $(OPTS) -c Button.cpp

Textbox.o:	Textbox.h Textbox.cpp glutFuncs.h
	g++ $(OPTS) -c Textbox.cpp


clean:
	rm -f *.o nerdNotes *~
