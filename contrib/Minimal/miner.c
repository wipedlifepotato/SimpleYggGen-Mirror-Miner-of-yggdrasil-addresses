#include"miner.h"
struct addrBox miner(const char* needle){
	BoxKeys keys;
	char * ipv6=0;
	do{
		if(ipv6!=0)
			free(ipv6);
		keys = getKeyPair();
		unsigned char hash[SHA512_DIGEST_LENGTH]; 
		getSHA512(keys.PublicKey, hash);
		ipv6 = convertSHA512ToIPv6(hash);
	}while(strstr(ipv6,needle) == 0);
	struct addrBox ret={keys,ipv6};
	return ret;		
		
}
