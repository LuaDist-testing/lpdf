# makefile for pdflib binding for Lua

# change these to reflect your Lua installation
LUA= /tmp/lhf/lua-5.1.2
LUAINC= $(LUA)/src
LUALIB= $(LUA)/src
LUABIN= $(LUA)/src

# these will probably work if Lua has been installed globally
#LUA= /usr/local
#LUAINC= $(LUA)/include
#LUALIB= $(LUA)/lib
#LUABIN= $(LUA)/bin

# change these to reflect your PDFlib installation
PDF= /tmp/lhf/PDFlib-Lite-7.0.1/libs/pdflib
#PDF= /tmp/lhf/PDFlib-Lite-6.0.3/libs/pdflib
PDFINC= $(PDF)
PDFLIB= $(PDF)/.libs
#PDFLIB= $(PDF)/.libs/libpdf.a

# probably no need to change anything below here
CC= gcc
CFLAGS= $(INCS) $(WARN) -O2 $G
WARN= -ansi -pedantic -Wall
INCS= -I$(LUAINC) -I$(PDFINC)

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
	$(CC) -o $@ -shared $(OBJS) -L$(PDFLIB) -lpdf

clean:
	rm -f $(OBJS) $T core core.* test.pdf

doc:
	@echo "$(MYNAME) library:"
	@fgrep '/**' $(MYLIB).c | cut -f2 -d/ | tr -d '*' | sort | column

# distribution

FTP= $(HOME)/public/ftp/lua/5.1
D= $(MYNAME)
A= $(MYLIB).tar.gz
TOTAR= Makefile,README,$(MYLIB).c,test.lua,clock.lua,image.lua,image.png,logo.lua,lua-logo.lua

tar:	clean
	tar zcvf $A -C .. $D/{$(TOTAR)}

distr:	tar
	touch -r $A .stamp
	mv $A $(FTP)

diff:	clean
	tar zxf $(FTP)/$A
	diff $D .

# eof
