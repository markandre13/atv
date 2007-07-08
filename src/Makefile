CXX=c++
CXXFLAGS=-Wall -I.
#CXXFLAGS=-Wall -fPIC -I.
LDFLAGS=
AR=ar
RANLIB=ranlib

OBJS=atvparser.o serializable.o

libatv.a: $(OBJS)
	$(AR) rs libatv.a $(OBJS)
	$(RANLIB) libatv.a

all: libatv.a
#all: libatv.so.0.1 libatv.so

#libatv.so.0.1: $(OBJS)
#	$(CXX) -shared -Wl,-soname,libatv.so.0.2 -o libatv.so.0.2 $(OBJS)
#
#libatv.so:
#	ln -s libatv.so.0.2 libatv.so

clean:
	rm -f $(OBJS) libatv.* *~ DEADJOE
