#include"miner.h"
void miner(void){
	BoxKeys keys = getKeyPair();


	puts("PublicKey");
	for(int i =0; i < sizeof(keys.PublicKey);i++){
		printf( "%02x", keys.PublicKey[i]);
	}
	puts("\nPrivateKey");
	for(int i =0; i < sizeof(keys.PrivateKey);i++){
		printf( "%02x", keys.PrivateKey[i]);
	}
	puts("");
	unsigned char hash[SHA512_DIGEST_LENGTH]; 
	getSHA512(keys.PublicKey, hash);
	char * ipv6 = convertSHA512ToIPv6(hash);
	//printf("IPv6: %s\r\nRawPublicKey: %s\r\nRawPrivateKey: %s\r\n", ipv6, keys.PublicKey, keys.PrivateKey);
	printf("IPv6: %s\r\n", ipv6);
	sleep(5);
	free(ipv6);
}
