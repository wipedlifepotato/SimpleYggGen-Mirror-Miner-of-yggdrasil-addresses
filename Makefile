libraries= -lcrypto -lpthread
cxx = g++
files= SimpleYggGen.cpp
programname=SimpleYggGen
CXFLAG=-std=gnu++11
c = gcc
GTKDIR=GTK
all: gtk 
	$(cxx) $(files) $(CXFLAG) $(libraries) -o $(programname)
	cp $(GTKDIR)/interface $(GTKDIR)/*.glade .
gtk:
	cd $(GTKDIR) && make 
