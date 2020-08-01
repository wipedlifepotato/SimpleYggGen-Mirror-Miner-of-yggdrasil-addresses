#include"generator.h"
//#include<sstream>
BoxKeys getKeyPair(void){
     	BoxKeys keys;
	size_t lenpub = KEYSIZE;
	size_t lenpriv = KEYSIZE;

	EVP_PKEY_CTX * Ctx;
	EVP_PKEY * Pkey = nullptr;
	Ctx = EVP_PKEY_CTX_new_id (NID_X25519, NULL);

	EVP_PKEY_keygen_init (Ctx);
	EVP_PKEY_keygen (Ctx, &Pkey);

	EVP_PKEY_get_raw_public_key (Pkey, keys.PublicKey, &lenpub);
	EVP_PKEY_get_raw_private_key (Pkey, keys.PrivateKey, &lenpriv);

	EVP_PKEY_CTX_free(Ctx);
	EVP_PKEY_free(Pkey);    
 	//crypto_box_keypair(keys.PublicKey,keys.PrivateKey);
	return keys;
}
/*
// GetNodeID returns the NodeID associated with a BoxPubKey.
func GetNodeID(pub *BoxPubKey) *NodeID {
	h := sha512.Sum512(pub[:])
	return (*NodeID)(&h)
}
*/

int getOnes(unsigned char HashValue[SHA512_DIGEST_LENGTH]){
		int bitcount = 0, bit =0; 
		std::bitset<8> bits_header(HashValue[0]);		// получаем биты первого байта хэша
		std::string s_first4bytes = bits_header.to_string(); 		// сохраняем их в стринг
		for(int y = 1; y < 4; ++y)						// добавляем еще 3 байта
		{
			std::bitset<8> bits_header_temp(HashValue[y]);
			s_first4bytes += bits_header_temp.to_string();
		}

		bit = 0;

		while(s_first4bytes[bit] != '0' && s_first4bytes[bit] == '1' ) // цикл по-битового анализа
		{
		++bit;
			if(bit > bitcount) // сохраняем связку лучших ключей
			{
				bitcount = bit;
				// outout -------------------------------
				if(s_first4bytes[bit] == '0')
				{
					return bitcount;
				}//if
			}
		}//while
		return bitcount;
}//endfunc
void getSHA512(void* data, unsigned char * hash){
		SHA512_CTX sha512;
		SHA512_Init(&sha512);
		SHA512_Update(&sha512, data, KEYSIZE);
		SHA512_Final(hash, &sha512);
}

/*
func newBoxKey() keySet {
	pub, priv := crypto.NewBoxKeys()
	id := crypto.GetNodeID(pub)
	ip := net.IP(address.AddrForNodeID(id)[:]).String()
	return keySet{priv[:], pub[:], id[:], ip}
}
*/
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[128]){
    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
}
char * convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH], BoxKeys myKeys){
		//char hash[128];
		//convertSHA512ToSum(h, hash);

		char byte;
		bool done;

		unsigned char nBits=0;
		char temp[8*SHA512_DIGEST_LENGTH];
		int i=0;

		// 2 / 4 / 8 -> приближенные значения
		for (auto idx = 0; idx < 8*SHA512_DIGEST_LENGTH; idx++) {
			char bit = (hash[idx/8] & (0x80 >> (unsigned char)(idx%8))) >> (unsigned char)(7-(idx%8));
			if (!done && bit != 0) 
				continue;
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

		
		//std::cout << "I:" << i << std::endl;
		struct in6_addr tmpAdr;
/*
	prefix := GetPrefix()
	copy(addr[:], prefix[:])
	addr[len(prefix)] = ones
	copy(addr[len(prefix)+1:], temp)
*/
		tmpAdr.s6_addr[0] = 0x02;
		tmpAdr.s6_addr[1] = getOnes(hash);
		//sizeof(temp) можно поменять просто на 16
		for(int i =2; i < sizeof(temp); i++) // Тут на самом деле 16 байтов, и это очень не хорошо
			tmpAdr.s6_addr[i]=temp[i];
		char * addr = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));
		inet_ntop(AF_INET6, &tmpAdr, addr, INET6_ADDRSTRLEN);
		for (int i = 7; i >= 4; i--)
			addr[i]='?';
		for(int i = strlen(addr); i>=(strlen(addr)-4);i--) //38 
			addr[i]='?';
		return addr;
}	
int miner(void)
{
	for(;;)
	{

	// x25519 -----------------------


		auto myKeys = getKeyPair();
		puts("Public: ");
		for(int i = 0; i < KEYSIZE; ++i)
		{
        		printf("%02x", myKeys.PublicKey[i]);// two byte 
		}
		puts("\nPrivate: ");
		for(int i = 0; i < KEYSIZE; ++i)
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
