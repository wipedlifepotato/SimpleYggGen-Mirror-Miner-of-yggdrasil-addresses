#include"bitset.h"
BitSet countMyBits( Byte * bytes, size_t size ){
	//init BitSet
	BitSet ret;
	memset(ret.bytes, 0, sizeof(ret.bytes));
	ret.ones=0;
//
  	unsigned char nBits = 0;
  	int z=0;
	Byte byte=0;
        bool oneFound=false;
        for(size_t zise = 0; zise < size; zise++){
		for(int i =0; i <= 7; i++ ){
			unsigned char bit = (bytes[zise] << i) & 0x80;
			if(bit > 0 && !oneFound){
				 ret.ones++;
				 continue;
			}else if(!oneFound && bit == 0){ 
					 oneFound=true;
					 continue;
			}
			byte = (byte << 1) | (bit > 0 ? 1 : 0);
		        nBits++;
		        if (nBits>=8) {
			 nBits = 0;
			 ret.bytes[z++] = byte;
		       }
		}
	}
	return ret;
}
