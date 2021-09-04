CXXFLAGS	:= -ansi -pedantic -Wall -Wextra -m64 -O3

all:	libA.so libB.so main

libA.so:	Mother.h Mother.cc A.cc A.h
	clang++ $(CXXFLAGS) -dynamiclib -o $@ A.cc Mother.cc
libB.so:	Mother.h Mother.cc B.cc B.h
	clang++ $(CXXFLAGS) -dynamiclib -o $@ B.cc Mother.cc
main:		PluginLoader.h Mother.h Mother.cc main.cc
	clang++ $(CXXFLAGS) $@.cc -o $@

LOCALCC := $(wildcard *.cc)
clean:
	$(RM) $(LOCALCC:%.cc=%) *.so
