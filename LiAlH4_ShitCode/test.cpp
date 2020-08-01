//#include<openssl/evp.h>

#include"generator.h"
#include<sstream>

std::string getFirst4Byte(unsigned char HashValue[SHA512_DIGEST_LENGTH], BoxKeys keys){
		int bit = 0;				// счетчик для подсчета единиц
		int bitcount = 10;
		uint8_t PublicKeyBest[32];
		uint8_t PrivateKeyBest[32];

		std::string s_first4bytes;  // переменная для хранения хэша

		std::bitset<8> bits_header(HashValue[0]);		// получаем биты первого байта хэша
		s_first4bytes = bits_header.to_string(); 		// сохраняем их в стринг

		for(int y = 1; y < 4; ++y)						// добавляем еще 3 байта
		{
		std::bitset<8> bits_header_temp(HashValue[y]);
		s_first4bytes += bits_header_temp.to_string();
		}
	// bits ----------------------------------
		bit = 0;
		while(s_first4bytes[bit] != '0' && s_first4bytes[bit] == '1' ) // цикл по-битового анализа
		{
		++bit;
			if(bit > bitcount) // сохраняем связку лучших ключей
			{
				bitcount = bit;
				for(int z = 0; z < 32; ++z)
				{
					PublicKeyBest[z] = keys.PublicKey[z];
				}
				for(int z = 0; z < 32; ++z)
				{
					PrivateKeyBest[z] = keys.PrivateKey[z];
				}

				// outout -------------------------------
				if(s_first4bytes[bit] == '0')
				{
					auto ret = std::ostringstream();
					ret << 2 << std::setw(2) << std::setfill('0') << std::hex << bitcount;
					return ret.str();
				}
			}
		}
		return "";
}


BoxKeys getKeyPair(void){
     	BoxKeys keys;
     	crypto_box_keypair(keys.PublicKey,keys.PrivateKey);
	return keys;
}

//		unsigned char hash[SHA512_DIGEST_LENGTH];
void getSHA512(void* data, unsigned char * hash){
		SHA512_CTX sha512;
		SHA512_Init(&sha512);
		SHA512_Update(&sha512, data, crypto_box_PUBLICKEYBYTES);
		SHA512_Final(hash, &sha512);
}
/*
  	SigningPublicKey: 1895d418045e5b08f1b1eaa331793b97a4099171ba56396d952d6915498f2c0f
        inet6 201:d98b:62d3:c0a6:b49:49ee:d950:13d  prefixlen 7  scopeid 0x0<global>
        inet6 fe80::e449:d4c0:35fb:4bc5  prefixlen 64  scopeid 0x20<link>
<Mercury> func GetNodeID(pub *BoxPubKey) *NodeID {
<Mercury>  h := sha512.Sum512(pub[:])
<Mercury>  return (*NodeID)(&h)
<Mercury> }
https://github.com/yggdrasil-network/yggdrasil-go/blob/78b5f88e4bb734d0dd6a138ff08d34ca39dcaea3/contrib/ansible/genkeys.go#L94
	https://github.com/yggdrasil-network/yggdrasil-go/blob/master/src/address/address.go#L52
https://github.com/yggdrasil-network/yggdrasil-go/blob/1fbab17b376bb8f4ee7026dded7461276681056f/src/tuntap/tun.go#L159

*/

void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[128]){
    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
}

//правильно ли? I think is not coorect but from -> 
/*
https://github.com/yggdrasil-network/yggdrasil-go/blob/c3f8db699138a08278017634d3ec0057db2b253c/src/crypto/crypto.go#L85
getNodeID
-> https://github.com/yggdrasil-network/yggdrasil-go/blob/1fbab17b376bb8f4ee7026dded7461276681056f/src/address/address.go#L52
<Mercury>   EncryptionPublicKey: 6ae6ed2c9554ea2252df8ec5f510cd1beb60f6ef05d94f90bdbbb5061440bb75 => is our key
<Mercury> is our ipv6
<Mercury> GuruRandomAscension_, зачем
*/


/*
// BoxPubKey is a NaCl-like "box" public key (curve25519+xsalsa20+poly1305).

type BoxPubKey [BoxPubKeyLen]byte

//info: https://github.com/yggdrasil-network/yggdrasil-go/blob/master/src/crypto/crypto.go#L185
//https://github.com/golang/crypto/blob/master/nacl/box/box.go#L59
//LOL REALLY?
//XSALSA20?!
ORIGINAL CODE:
// AddrForNodeID takes a *NodeID as an argument and returns an *Address.
// This address begins with the contents of GetPrefix(), with the last bit set to 0 to indicate an address.
// The following 8 bits are set to the number of leading 1 bits in the NodeID.
// The NodeID, excluding the leading 1 bits and the first leading 0 bit, is truncated to the appropriate length and makes up the remainder of the address.
func AddrForNodeID(nid *crypto.NodeID) *Address {
	// 128 bit address
	// Begins with prefix
	// Next bit is a 0
	// Next 7 bits, interpreted as a uint, are # of leading 1s in the NodeID
	// Leading 1s and first leading 0 of the NodeID are truncated off
	// The rest is appended to the IPv6 address (truncated to 128 bits total)
	var addr Address
	var temp []byte
	done := false
	ones := byte(0)
	bits := byte(0)
	nBits := 0
	for idx := 0; idx < 8*len(nid); idx++ {
		bit := (nid[idx/8] & (0x80 >> byte(idx%8))) >> byte(7-(idx%8))
		if !done && bit != 0 {
			ones++
			continue
		}
		if !done && bit == 0 {
			done = true
			continue // FIXME? this assumes that ones <= 127, probably only worth changing by using a variable length uint64, but that would require changes to the addressing scheme, and I'm not sure ones > 127 is realistic
		}
		bits = (bits << 1) | bit
		nBits++
		if nBits == 8 {
			nBits = 0
			temp = append(temp, bits)
		}
	}
	prefix := GetPrefix()
	copy(addr[:], prefix[:])
	addr[len(prefix)] = ones
	copy(addr[len(prefix)+1:], temp)
	return &addr
}


*/
char * convertSHA512ToIPv6(unsigned char h[SHA512_DIGEST_LENGTH], BoxKeys myKeys){
		char hash[128];
		convertSHA512ToSum(h, hash);

		char byte;
		bool done;
		char ones;
		int nBits=0;
		char temp[16];
		int i=0;
		int z=0;
		for (auto idx = 0; idx < sizeof(hash); idx++) {
			char bit = (hash[idx/8] & (0x80 >> (unsigned char)(idx%8))) >> (unsigned char)(7-(idx%8));
			if (!done && bit != 0) {
				ones++;
				continue;
			}
			if (!done && bit == 0) {
				done = true;
				continue; // FIXME? this assumes that ones <= 127, probably only worth changing by using a variable length uint64, but that would require changes to the addressing scheme, and I'm not sure ones > 127 is realistic
			}
			byte = (byte << 1) | bit;
			nBits++;
			if (nBits == 8) {
				nBits = 0;
				temp[i++]=(byte);
			}
		}

		int bitcount=10;//magic number?
		unsigned short countones=0;
		for(int i = 1; i < 4; i++){
			for(int y=0; y <= 3; y++)
				if ( (temp[i] >> y) & 1 ) countones++;
		}
		auto first4byte = getFirst4Byte(h, myKeys);
		if ( first4byte == "" ) first4byte ="Not Found";
		std::cout << "[" << first4byte << "]" << std::endl;
		struct in6_addr tmpAdr;
		tmpAdr.s6_addr[0]=0x02;
		for(int i =1; i < 16; i++)
			tmpAdr.s6_addr[i]=temp[i];
		char * addr = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));
		inet_ntop(AF_INET6, &tmpAdr, addr, INET6_ADDRSTRLEN);
		//std::cout << "adress: " << addr  << std::endl;
		return addr;
}	
int miner(void)
{
	for(;;)
	{

	// x25519 -----------------------


		auto myKeys = getKeyPair();
		puts("Public: ");
		for(int i = 0; i < crypto_box_PUBLICKEYBYTES; ++i)
		{
        		printf("%02x", myKeys.PublicKey[i]);// two byte 
		}
		puts("\nPrivate: ");
		for(int i = 0; i < crypto_box_SECRETKEYBYTES; ++i)
		{
        		printf("%02x", myKeys.PrivateKey[i]);// two byte 
		}
		puts("");

// sha512 --------------------------------
		puts("SHA512:");
		unsigned char hash[SHA512_DIGEST_LENGTH];
		getSHA512(myKeys.PublicKey, hash);
		for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
		{
        		printf("%02x", hash[i]);// two byte 
		}
		puts("");
		puts("IPv6:");
		

		auto ipv6=convertSHA512ToIPv6(hash, myKeys);
		printf("%s\n", ipv6);
		free(ipv6);
		puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	}
}

int main(void){
	miner();
}
