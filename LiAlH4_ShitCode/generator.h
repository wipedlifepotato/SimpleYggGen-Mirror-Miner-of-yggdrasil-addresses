#include<openssl/sha.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/types.h> 
#include<iostream>
#include<iomanip>
#include<nacl/crypto_box.h>
#include<bitset>
typedef struct {
     unsigned char PublicKey[crypto_box_PUBLICKEYBYTES];
     unsigned char PrivateKey[crypto_box_SECRETKEYBYTES];
}BoxKeys;
BoxKeys getKeyPair(void);
std::string getFirst4Byte(unsigned char hash[SHA512_DIGEST_LENGTH], BoxKeys keys);
void getSHA512(void* data, unsigned char * hash);
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[128]);
char * convertSHA512ToIPv6(unsigned char h[SHA512_DIGEST_LENGTH], BoxKeys keys);
int miner(void);
