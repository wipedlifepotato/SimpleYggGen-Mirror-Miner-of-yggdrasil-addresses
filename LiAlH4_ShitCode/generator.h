#include<openssl/sha.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/types.h> 
#include<iostream>
#include<iomanip>
#include<openssl/evp.h>
#include<bitset>
#define KEYSIZE 32
typedef struct {
    uint8_t PublicKey[KEYSIZE];
    uint8_t PrivateKey[KEYSIZE];
    // unsigned char PublicKey[crypto_box_PUBLICKEYBYTES];
    // unsigned char PrivateKey[crypto_box_SECRETKEYBYTES];
}BoxKeys;
BoxKeys getKeyPair(void);
void getSHA512(void* data, unsigned char * hash);
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[128]);
char * convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH], BoxKeys keys);
int miner(void);
