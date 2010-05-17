# customize area
prefix=/usr/local

host=mingw
 #cygwin,msvc,linux,bsd... (currently mingw only)

objsuffix=.o
 #.o or .obj

exesuffix=.exe
 #.exe or (none)

fe=-o
 #-o or /Fe

fo=-o
 #-o or /Fo

CXXFLAGS = -I. -DHAS_CXX0X_MEMORY
LINK = $(CXX)
LINKFLAGS = $(CXXFLAGS)
TARGET = git$(exesuffix)
OBJS = $(addsuffix $(objsuffix), modern-git compat/$(host))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK) $(LINKFLAGS) $(fe) $@ $^

