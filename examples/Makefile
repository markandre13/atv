CXX=c++
CC=$(CXX)
CXXFLAGS=-I../src -g
LDFLAGS=

all: menu graphic poll

graphic: graphic.o serializable.o atvparser.o

menu: menu.o atvparser.o

poll: poll.o atvparser.o

clean:
	rm -f menu graphic *.o libatv.a *~ DEADJOE core
	rm -f atvparser.cc atvparser.hh serializable.cc serializable.hh

menu.o: atvparser.hh
graphic.o: atvparser.hh serializable.hh

atvparser.o: atvparser.cc atvparser.hh
serialize.o: serializable.cc serializable.hh atvparser.hh

atvparser.cc:
	ln -s ../src/atvparser.cc atvparser.cc
atvparser.hh:
	ln -s ../src/atvparser.hh atvparser.hh
serializable.cc:
	ln -s ../src/serializable.cc serializable.cc
serializable.hh:
	ln -s ../src/serializable.hh serializable.hh
