# SimpleYggGen-CPP

Simple Yggdrasil Address Miner on C++. Простой майнер адресов Yggdrasil на C++. 
# Usage
```
SimpleYggGen [text-pattern|regex-pattern] [options]
-h --help, help menu
-r --reg,  regexp instead just text pattern (e.g. '(one|two).*')
--threads -t, (default count of system)
-o --output output file (default keys.txt)
--usage this menu
--highhead -H mode of high head...
--searchadress -s (default) mode
--limitfound=n -lN limit found
--ncurses -n start ncurses interface
```

# Build instruction 
## Linux - Ubuntu / Debian(apt/apt-get)
 * Install prerequisites:

 
```
sudo apt-get install libssl-dev autoconf libgtk-3-dev libncurses-dev devscripts build-essential lintian dh-make
``` 


 * To complile:
   - `make -j NUMBER_OF_PROCESSOR_CORES` e.g `make -j8`
   - or `make` to compile single-threaded
