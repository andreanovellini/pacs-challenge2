CXXFLAGS = -fPIC
LDFLAGS = -L. -Wl,-rpath=${PWD}
LIBS = -lclassZeroFun

.PHONY: all clean distclean

all: main

main: main.o libclassZeroFun.so 
	$(CXX) $(LDFLAGS) main.o -o main $(LIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

libclassZeroFun.so: classZeroFun.o
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,libclassZeroFun.so classZeroFun.o -o libclassZeroFun.so

classZeroFun.o: classZeroFun.cpp
	$(CXX) $(CXXFLAGS) -c classZeroFun.cpp

clean:
	$(RM) *.o

distclean: clean
	$(RM) libclassZeroFun.so main
