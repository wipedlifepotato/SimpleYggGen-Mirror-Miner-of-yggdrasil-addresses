libraries= -lcrypto -lpthread
compiler = g++
files= SimpleYggGen.cpp
programname=SimpleYggGen
CXFLAG=-std=gnu++11
all:
	$(compiler) $(files) $(CXFLAG) $(libraries) -o $(programname)
