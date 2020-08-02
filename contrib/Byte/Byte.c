#include<stdio.h>
#include<stdlib.h>
struct lools_foracetone{
	int a:2;
	int b:2;
	int c:2;
	int d:1;
	int e:1;
};// бля a 2 бита, для б2 ))))


struct BitsInByte{
	unsigned char ones;
	unsigned char zeros;
	unsigned char delim;//count of first ones
};
typedef unsigned char Byte;
typedef enum{false,true}bool;

struct BitsInByte countMyBits( Byte byte ){
        bool oneFound=false;
	
        struct BitsInByte retval={0,0};
	char examplebinary[9];
	examplebinary[8]=0;
	for(int i =0; i < 8; i++ ){
		unsigned char bit = (byte << i) & 0x80;
		printf("%d bit\n", bit);
		if(bit > 0){
			 if( !oneFound ) retval.delim++;;
			 retval.ones++;
			 examplebinary[i]='1';
		}
		else{
			 if( !oneFound )oneFound=true;
			 retval.zeros++;
			 examplebinary[i]='0';
		}
	}
		printf("Example binary: %s\n", examplebinary);
	return retval;
}


int main(int argCount, char**arguments){
	if( argCount < 2 ) 
		return fprintf(stderr, "%s [byte]/[number < 255]\n", arguments[0]);
	Byte searchbyte;
        if ( arguments[1][0] >= 48 && arguments[1][0] <= '9' ) searchbyte=atoi(arguments[1]);
	else searchbyte=arguments[1][0];
	printf("Search %d\n", searchbyte);
	struct BitsInByte myCount = countMyBits( searchbyte );
	printf(" count of zeros %d ;count of ones %d;лидирующие(?) %d\n ", myCount.zeros, myCount.ones, myCount.delim);
}
