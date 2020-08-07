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

/*
Example for debug:
	puts("PublicKey");
	for(int i =0; i < sizeof(keys.PublicKey);i++){
		printf( "%02x", keys.PublicKey[i]);
	}
	puts("\nPrivateKey");
	for(int i =0; i < sizeof(keys.PrivateKey);i++){
		printf( "%02x", keys.PrivateKey[i]);
	}
	puts("");

	//printf("IPv6: %s\r\nRawPublicKey: %s\r\nRawPrivateKey: %s\r\n", ipv6, keys.PublicKey, keys.PrivateKey);
	//printf("IPv6: %s\r\n", ipv6);
	//sleep(5);
*/
