#include<openssl/sha.h>
#include<openssl/evp.h>
#include<openssl/bn.h>
#include<string.h>
#ifdef __linux__
	#include<arpa/inet.h>
	#include<sys/socket.h>
	#include<sys/types.h> 
#else
	#include <Winsock2.h>
#endif

#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<regex>
#include<getopt.h>
#include<thread>
#include<sstream>
#include<random>

#include<bitset>
#define KEYSIZE 32
#define NAMEPROGRAM "SimpleYggGen"
#define COAUTHORS "lialh4, orignal, i2pd-project."

constexpr auto defaultHighSearchFileName="syg-highsearch.txt";
constexpr auto defaultSearchFileName="syg-search.txt";//(reg)

typedef struct {
    uint8_t PublicKey[KEYSIZE];
    uint8_t PrivateKey[KEYSIZE];
    // unsigned char PublicKey[crypto_box_PUBLICKEYBYTES];
    // unsigned char PrivateKey[crypto_box_SECRETKEYBYTES];
}BoxKeys;
enum class ProgramMode 
{ 	high, search	 }; 
struct dataKey{
    	uint8_t pk[KEYSIZE];
    	uint8_t sk[KEYSIZE];
	std::string ip;
};

BoxKeys getKeyPair(void);
void getSHA512(void* data, unsigned char * hash);
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[128]);
char * convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH], BoxKeys keys);
int miner(void);
