libraries= -lcrypto -lpthread
cxx = g++
files= SimpleYggGen.cpp
ncursesfiles=ncurses/field.c ncurses/ncurses-interface.c
ncursesoptions=-lform -lmenu -lncurses -fpermissive
programname=SimpleYggGen
CXFLAG=-std=gnu++11
GTKINTERFACENAME=interface
MINGWNCURSESWDIR=/home/user/ncursesw/
MINGWOPENSSLDIR=/home/user/openssl/
OPTIMIZATIONFLAG=-O3
c = gcc
GTKDIR=GTK
all: SimpleYggGen gtk 
	
	cp $(GTKDIR)/$(GTKINTERFACENAME) $(GTKDIR)/*.glade .
SimpleYggGen: 
	$(cxx) $(files) $(ncursesfiles) $(CXFLAG) $(libraries) $(ncursesoptions) $(OPTIMIZATIONFLAG) -o $(programname) 
gtk:
	cd $(GTKDIR) && make 

clear:
	rm $(programname) $(GTKINTERFACENAME) # GTK.glade
	cd $(GTKDIR) && make clear

mingw32:
	i686-w64-mingw32-g++-posix $(files) $(OPTIMIZATIONFLAG) $(ncursesfiles) -I$(MINGWNCURSESWDIR)/include/ -L$(MINGWNCURSESWDIR)/lib/ -lncursesw -lformw -lmenuw -fpermissive -I$(MINGWOPENSSLDIR)/include -L$(MINGWOPENSSLDIR) -lcrypto -lws2_32 -lpthread -static-libstdc++ -static-libgcc -fopenmp
	


	
	
