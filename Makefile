# makefile for pdflib binding for Lua

# change these to reflect your Lua installation
LUA= /tmp/lhf/lua-5.1.5
LUAINC= $(LUA)/src
LUALIB= $(LUA)/src
LUABIN= $(LUA)/src

# these will probably work if Lua has been installed globally
#LUA= /usr/local
#LUAINC= $(LUA)/include
#LUALIB= $(LUA)/lib
#LUABIN= $(LUA)/bin

# change these to reflect your PDFlib installation
PDF= /tmp/lhf/PDFlib-Lite-7.0.5p3/libs/pdflib
PDFINC= $(PDF)
PDFLIB= $(PDF)/.libs
#PDFLIB= $(PDF)/.libs/libpdf.a

# probably no need to change anything below here
CC= gcc
CFLAGS= $(INCS) $(WARN) -O2 $G
WARN= -ansi -pedantic -Wall -Wextra
INCS= -I$(LUAINC) -I$(PDFINC)
MAKESO= $(CC) -shared
#MAKESO= $(CC) -bundle -undefined dynamic_lookup

MYNAME= pdf
MYLIB= l$(MYNAME)
T= $(MYNAME).so
OBJS= $(MYLIB).o
TEST= test.lua

all:	test

test:	$T
	env LD_LIBRARY_PATH=$(PDFLIB) $(LUABIN)/lua $(TEST)

o:	$(MYLIB).o

so:	$T

$T:	$(OBJS)
	$(MAKESO) -o $@ $(OBJS) -L$(PDFLIB) -lpdf

clean:
	rm -f $(OBJS) $T core core.* test.pdf

doc:
	@echo "$(MYNAME) library:"
	@fgrep '/**' $(MYLIB).c | cut -f2 -d/ | tr -d '*' | sort | column

# eof
