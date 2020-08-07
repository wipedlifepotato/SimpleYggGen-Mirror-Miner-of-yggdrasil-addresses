#pragma once
#include <openssl/sha.h>
#include <string.h>
void getSHA512(void *data, unsigned char *hash) ;
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH],
                        char outputBuffer[128]);

