libraries= -lcrypto -lpthread
cxx = g++
files= SimpleYggGen.cpp
programname=SimpleYggGen
CXFLAG=-std=gnu++11
GTKINTERFACENAME=interface
c = gcc
GTKDIR=GTK
all: gtk 
	$(cxx) $(files) $(CXFLAG) $(libraries) -o $(programname)
	cp $(GTKDIR)/$(GTKINTERFACENAME) $(GTKDIR)/*.glade .
gtk:
	cd $(GTKDIR) && make 

clear:
	rm $(programname) $(GTKINTERFACENAME) # GTK.glade
