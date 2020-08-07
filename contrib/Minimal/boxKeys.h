#pragma once
#define KEYSIZE 32
#include <openssl/evp.h>

typedef struct {
  uint8_t PublicKey[KEYSIZE];
  uint8_t PrivateKey[KEYSIZE];
  // unsigned char PublicKey[crypto_box_PUBLICKEYBYTES];
  // unsigned char PrivateKey[crypto_box_SECRETKEYBYTES];
} BoxKeys;
BoxKeys getKeyPair(void) ;
