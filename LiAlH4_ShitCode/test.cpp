#include"generator.h"
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
		char ones;
		int nBits=0;
		char temp[16];
		int i=0;
		int z=0;
		// 2 / 4 / 8 -> приближенные значения
		for (auto idx = 0; idx < 2*SHA512_DIGEST_LENGTH; idx++) {
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

		
		//std::cout << "I:" << i << std::endl;
		struct in6_addr tmpAdr;
/*
	prefix := GetPrefix()
	copy(addr[:], prefix[:])
	addr[len(prefix)] = ones
	copy(addr[len(prefix)+1:], temp)
*/
		tmpAdr.s6_addr[0] = 0x02;
		tmpAdr.s6_addr[1] = ones;
		for(int i =2; i < 16; i++)
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
