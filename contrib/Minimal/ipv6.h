#pragma once
#include"bitset.h"
#include"SHA512.h"
#include <openssl/sha.h>
#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#else
	#include <winsock2.h>
	#define _WIN32_WINNT _WIN32_WINNT_WIN8 // Windows 8.0
	#include <ws2tcpip.h> // -lws2_32
#endif
#include <unistd.h>

char *convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH]) ;


