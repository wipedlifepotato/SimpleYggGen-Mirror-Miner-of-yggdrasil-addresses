#pragma once
#include"bitset.h"
#include"SHA512.h"

#include <openssl/sha.h>
#include <unistd.h>
#include"inet_ntop.h"

char *convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH]) ;


