#include"ipv6.h"
#include"boxKeys.h"
#include"SHA512.h"
struct addrBox{
	BoxKeys keys;
	char * ipv6 ;
};

struct addrBox miner(const char*);
