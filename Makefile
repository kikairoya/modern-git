# customize area
prefix=/usr/local

host=mingw
 #cygwin,msvc,linux,bsd... (currently mingw only)

objsuffix=.o
 #.o or .obj

libsuffix=.a
 #.a or .lib

exesuffix=.exe
 #.exe or (none)

fe=-o
 #-o or /Fe

fo=-o
 #-o or /Fo

MAKEDEP=$(CXX) -E -MM $(CXXFLAGS)

CXXFLAGS = -I. -g -Wall
LINK = $(CXX)
LINKFLAGS = $(CXXFLAGS) -Wl,--enable-auto-import
TARGET = git$(exesuffix)
MAINOBJS = modern-git environment
COMPATOBJS = $(host)
TESTOBJS = ustring_test

OBJS = $(addsuffix $(objsuffix), $(MAINOBJS) $(addprefix compat/, $(COMPATOBJS)))
TESTS = $(addsuffix $(objsuffix), $(addprefix testsuite/, $(TESTOBJS)))
OBJLIB = gitobjs$(libsuffix)

.PHONY: all clean test

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJS:$(objsuffix)=.dep)
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK) $(LINKFLAGS) $(fe) $@ $^

$(OBJLIB): $(OBJS)
	$(AR) rc $@ $^

%.dep: %.cc
	echo $@ \\ > $@
	$(MAKEDEP) $< >> $@

%.o: %.cc %.dep
	$(CXX) $(CXXFLAGS) -c $(fo) $@ $<

test: testsuite/unittest$(exesuffix)
	$<

testsuite/unittest$(exesuffix): $(TESTS) $(OBJLIB)
	$(LINK) $(LINKFLAGS) $(fe) $@ $^

clean:
	-$(RM) $(TARGET) $(TESTS) $(OBJLIB) $(OBJS) $(OBJS:$(objsuffix)=.dep)
