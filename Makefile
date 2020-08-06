libraries= -lcrypto -lpthread
cxx = g++
files= SimpleYggGen.cpp
ncursesfiles=ncurses/field.c ncurses/ncurses-interface.c
ncursesoptions=-lform -lmenu -lncurses -fpermissive
programname=SimpleYggGen
CXFLAG=-std=gnu++11
GTKINTERFACENAME=interface
c = gcc
GTKDIR=GTK
all: SimpleYggGen gtk 
	
	cp $(GTKDIR)/$(GTKINTERFACENAME) $(GTKDIR)/*.glade .
SimpleYggGen: 
	$(cxx) $(files) $(ncursesfiles) $(CXFLAG) $(libraries) $(ncursesoptions) -o $(programname)
gtk:
	cd $(GTKDIR) && make 

clear:
	rm $(programname) $(GTKINTERFACENAME) # GTK.glade
	cd $(GTKDIR) && make clear


	
	
