OS       := $(shell uname)

ifeq ($(OS), Darwin)
ONMAC    := -m64 -O3
LDFLAGS  := -framework GLUT -framework OpenGL
endif

ifeq ($(OS), Linux)
LDFLAGS  := -L /usr/lib64 -lglut
endif

CXX      := clang++
INCLUDES := -I .
CXXFLAGS := -ansi -pedantic -Wall $(ONMAC) -g -c $(INCLUDES)
CPPFLAGS += -MD -MP
SOURCES  := $(wildcard *.cc)

OBJS     := $(SOURCES:%.cc=%.o)
DEPS     := $(OBJS:%.o=%.d)


Bounce: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(DEPS) *~ Bounce Bounce.d -rf Bounce.dSYM

-include $(DEPS)
