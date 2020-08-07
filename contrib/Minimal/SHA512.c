#include<stdio.h>
#include"SHA512.h"
#define KEYSIZE 32
void getSHA512(void *data, unsigned char *hash) {
  SHA512_CTX sha512;
  SHA512_Init(&sha512);
  SHA512_Update(&sha512, data, KEYSIZE);
  SHA512_Final(hash, &sha512);
///unsigned char *SHA512(const unsigned char *d, size_t n,
               //        unsigned char *md); //SHA512 (buf, len, digest);
 // SHA512(hash, strlen(data), data);
}
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH],
                        char outputBuffer[128]) {
  for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
    sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
  }
}
