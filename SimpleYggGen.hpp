#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string.h>
#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#else
#include <winsock2.h>
#endif

#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <thread>
#include <vector>

#include <bitset>
#define KEYSIZE 32
#define NAMEPROGRAM "SimpleYggGen"
#define COAUTHORS "lialh4, orignal, i2pd-project."

//to inline?
#define ADDKEYS(to, from, ipv6){ \
	memcpy(to.sk, from.PrivateKey, sizeof(from.PrivateKey)); \
	memcpy(to.pk, from.PublicKey, sizeof(from.PublicKey));\
	to.ip =std::string(ipv6);}

constexpr auto defaultHighSearchFileName = "syg-highsearch.txt";
constexpr auto defaultSearchFileName = "syg-search.txt"; //(reg)

typedef struct {
  uint8_t PublicKey[KEYSIZE];
  uint8_t PrivateKey[KEYSIZE];
  // unsigned char PublicKey[crypto_box_PUBLICKEYBYTES];
  // unsigned char PrivateKey[crypto_box_SECRETKEYBYTES];
} BoxKeys;
enum class ProgramMode { high, search };
struct dataKey {
  uint8_t pk[KEYSIZE];
  uint8_t sk[KEYSIZE];
  std::string ip;
};

BoxKeys getKeyPair(void);
void getSHA512(void *data, unsigned char *hash);
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH],
                        char outputBuffer[128]);
char *convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH],
                          BoxKeys keys);
int miner(void);
