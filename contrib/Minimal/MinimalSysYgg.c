#include"miner.h"


#ifdef __linux__
#define RST "\x1B[0m"
#endif

static inline void getRandomColor() {
#ifndef __linux__
  return "";
#endif
  // unsigned short Background_color = (std::rand() % 4 + 41);//41 -> red
  unsigned short Foreground_color =
      (rand() % 3 + 32); // 30-> black 37-> white; 31 = red
  // if(Background_color - 10 == Foreground_color) Background_color=4;
  printf("\033[1;4;46;%dm", Foreground_color);
}


__attribute__((constructor)) void intro() {
   /* static const char randomtable[] = {
	    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
	    'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
	    'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	    'Y', 'Z', '!', '@', '(', ')', '/', '-', '#', '+', '$', '%', '^', '&', '*',
	    '`', '~', '>', '<', '?', '{', '}', '[', ']', ';', ':', '_', '=', '|', '\''};*/

  srand(time(NULL));
  int rv = 60;
  getRandomColor();
#define GETRANDOM(entropy, size_of_line){\
char tmp[size_of_line];tmp[size_of_line-1]=0;\
for(int i=size_of_line-1;i--;) sprintf(tmp[i], "%c", (rand() % entropy));\
printf("%s",tmp);\
}
puts(__FILE__" "__DATE__);
#ifdef __linux__
            printf("%s", RST);
#else
	    puts("");
#endif
}

int main(int argc, char*argv[]){//TODO: optarg... ncurses + gtk.
	//minecraft();
	if ( argc < 2 ) return fprintf(stderr, "%s ipv6\n", argv[0]);
	struct addrBox myAddr = miner(argv[1]);
	puts("PublicKey");
	for(int i =0; i < sizeof(myAddr.keys.PublicKey);i++){
		printf( "%02x", myAddr.keys.PublicKey[i]);
	}
	puts("\nPrivateKey");
	for(int i =0; i < sizeof(myAddr.keys.PrivateKey);i++){
		printf( "%02x", myAddr.keys.PrivateKey[i]);
	}
	puts("");

	printf("IPv6: %s\r\nRawPublicKey: %s\r\nRawPrivateKey: %s\r\n", myAddr.ipv6,
		 myAddr.keys.PublicKey, myAddr.keys.PrivateKey);
	printf("IPv6: %s\r\n", myAddr.ipv6);	
}
