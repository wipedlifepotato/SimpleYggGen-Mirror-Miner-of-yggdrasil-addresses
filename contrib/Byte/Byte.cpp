#include<iostream>
#include<bitset>
#include<iomanip>
#include<vector>

using Byte = unsigned char  ;

const int err(const int code, const std::string err){
	std::cerr <<  err << std::endl;
	return code;
}
struct BitsInByte{
	Byte ones;
	Byte zeros;
	Byte delim;//count of first ones
};

BitsInByte countMyBits( Byte byte ){
	std::bitset<8> bits_header(byte);
	BitsInByte retval{0,0,0};
	//std::vector<bool> bits = dynamic_cast<std::vector<bool>>(bits_header);
	auto & bits = bits_header;
	bool oneFound=false;
	std::cout << bits_header << " имеет - "<< std::endl;
	std::cout << bits.count() << " Единиц ";
  	std::cout << (bits.size()-bits.count()) << " нулей.\n";

	for(int i =7; i>=0;i--){
		std::cout << i << ":" << bits[i] << std::endl;
		if(bits[i]==1) {
			retval.ones++;
			if( !oneFound ) retval.delim++;;
		}else{
			 if( !oneFound )oneFound=true;
			 retval.zeros++;
		}
	}

	return retval;
}

int main(int argc, char * argv[]){
	if ( argc < 2 ) return err(1, (argv[0] + std::string(" byte") ));

	Byte searchbyte;
        if ( argv[1][0] >= '0' && argv[1][0] <= '9' ) searchbyte=atoi(argv[1]);
	else searchbyte=argv[1][0];

	std::cout << "Search: " << short(searchbyte) << std::endl;
	auto myCount = countMyBits(searchbyte);
	printf(" count of zeros %d ;count of ones %d;лидирующие(?) %d\n ", myCount.zeros, myCount.ones, myCount.delim);
}
