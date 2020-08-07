#include"enum.h"
#include <stdlib.h>
#include"SHA512.h"
#include<stddef.h>
#define BYTESBUF SHA512_DIGEST_LENGTH
typedef unsigned char Byte;
typedef struct{
	Byte ones;
	Byte bytes[BYTESBUF]; // byte = (byte << 1) | (bits[i] > 0 ? 1 : 0);
}BitSet;


BitSet countMyBits( Byte * bytes, size_t size );
