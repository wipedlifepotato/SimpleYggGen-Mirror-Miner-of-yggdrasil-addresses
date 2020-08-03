nameprog=interface
all: gtk-interface.o $(nameprog)	
interface: 
	gcc *.o -o interface `pkg-config --libs gtk+-3.0` -rdynamic
gtk-interface.o: gtk-interface.o
	gcc gtk-interface.c -c `pkg-config --cflags gtk+-3.0` -rdynamic
clear:
	rm $(nameprog) gtk-interface.o 2>/dev/null || echo "already cleared?"
