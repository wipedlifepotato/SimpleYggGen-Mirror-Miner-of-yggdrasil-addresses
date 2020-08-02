#include"generator.h"
#include<fstream>
#include<vector>
#include<regex>
#include<getopt.h>
#include<thread>

static bool found=false;

static struct{
    	uint8_t pk[KEYSIZE];
    	uint8_t sk[KEYSIZE];
	std::string ip;
}dataKey;

static struct{
        bool reg=false;
        int threads=-1;
        std::string outputpath="";
        std::regex regex;

}options;


static inline bool NotThat(const char * what, const std::regex & reg){
	return std::regex_match(what,reg) == 1 ? false : true;
}

static inline bool NotThat(const char * a, const char *b)
{
	while(*b)
		if(*a++!=*b++)
			return true;
	return false;
}



void usage(void){
	const constexpr char * help="IPv6YggDrasil [text-pattern|regex-pattern] [options]\n"
	"-h --help, help menu\n"
	"-r --reg,  regexp instead just text pattern (e.g. '(one|two).*')\n"
	"--threads -t, (default count of system)\n"
	"-o --output output file (default keys.txt)\n"
	"--usage this menu\n"
	//"--prefix -p\n"
	"";
	puts(help);
}

void parsing(int argc, char ** args){
	int option_index;
	static struct option long_options[]={
		{"help",no_argument,0,'h'},
		{"reg", no_argument,0,'r'},
		{"threads", required_argument, 0, 't'},
		{"output", required_argument,0,'o'},
		{"usage", no_argument,0,0},
		{0,0,0,0}
	};

	int c;
	while( (c=getopt_long(argc,args, "hrt:s:o:", long_options, &option_index))!=-1){
		switch(c){
			case 0:
				if ( std::string(long_options[option_index].name) == std::string("usage") ){
					usage();
					exit(1);
				}
			case 'h':
				usage();
				exit(0);
				break;
			case 'r':
				options.reg=true;
				break;
			case 't':
				options.threads=atoi(optarg);
				break;
			case 'o':
				options.outputpath=optarg;
				break;
			case '?':
				std::cerr << "Undefined argument" << std::endl;
			default:
				std::cerr << args[0] << " --usage / --help" << std::endl;
				exit(1);
				break;
		}
	}
}


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
void getSHA512(void* data, unsigned char * hash){
		SHA512_CTX sha512;
		SHA512_Init(&sha512);
		SHA512_Update(&sha512, data, KEYSIZE);
		SHA512_Final(hash, &sha512);
}
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[128]){
    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
}

/*
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#Public: 
#2e66d506ecf9cbac867dc6e80b59af9806245e779ef4a4da1f47398aeab5a210
#Private: 
#087b03e87d2ccc3f0862df887a2400240d47cd76a2726c7b9956a8bb9c7afc70
#SHA512:
#b0aa25d73b4aa8e55cd45f481d6b75655936431de5d9304154ee9e01b16ead0cd232f9e3a4db6a02c917a9efda884ee4923f920ffddde9d81548978e4e53e95f
#IPv6:
#I:63
#201:975c:ed2a:a395:7351:7d20:75ad:d595
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#tun0: flags=4305<UP,POINTOPOINT,RUNNING,NOARP,MULTICAST>  mtu 65535
#        inet6 201:c2a8:975c:ed2a:a395:7351:7d20:75ad  prefixlen 7  scopeid 0x0<global>
#        inet6 fe80::8646:6a8c:399a:61b8  prefixlen 64  scopeid 0x20<link>
#        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 500  (UNSPEC)
#        RX packets 0  bytes 0 (0.0 B)
#        RX errors 0  dropped 0  overruns 0  frame 0
#        TX packets 4  bytes 304 (304.0 B)
#        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
*/
char * convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH], BoxKeys myKeys){
		//char hash[128];
		//convertSHA512ToSum(h, hash);
		unsigned char byte;
		bool done;
		unsigned char lOnes=0;
		unsigned char nBits=0;
		unsigned char temp[SHA512_DIGEST_LENGTH];
		memset(temp, 0, sizeof(temp));
		int z=0;
		std::vector<std::bitset<8>> bytes;
		for( auto i =0; i < SHA512_DIGEST_LENGTH; i++) bytes.push_back(hash[i]);

		for( auto bits : bytes ){
				for(int i =7; i>=0;i--){
					if(bits[i]==1 && !done) {
						lOnes++;
						continue;
					}if(!done && bits[i] == 0){
						done = true;
						continue;
					}
					byte = (byte << 1) | (bits[i] >0? 1 : 0);
					nBits++;
					if(nBits>=8){
						nBits = 0;
						temp[z++]=byte;
						//std::cout << "val:" << int(temp[i-1]) << std::endl;
					}
				}
		}
		//std::cout << "I:" << z << std::endl;
		struct in6_addr tmpAdr;
/*
tun0: flags=4305<UP,POINTOPOINT,RUNNING,NOARP,MULTICAST>  mtu 65535
        inet6 207:f61:d4d3:f5ae:7dc9:73e0:b02f:ae48  prefixlen 7  scopeid 0x0<global>
        inet6 fe80::63c8:71c2:c9db:f446  prefixlen 64  scopeid 0x20<link>
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 500  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 4  bytes 304 (304.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Public: 
856fe97b3c367ce5db48ae3611f8ed6841850f15f65129d4482fafc4f741e00e
Private: 
a865d8a331f533993b9f7ddf635870c362d9673c4bdeac7cc0c3e5f6fcbc8b7b
SHA512:
fe0f61d4d3f5ae7dc973e0b02fae48b824bf2453f11f6c3bb4e93368be470756ea630bbed78dccaec81dee9603a29df7dd23c19cb5fac2748dba8b96e9d32c16
IPv6:
I:63
207:f61:d4d3:f5ae:7dc9:73e0:b02f:ae48
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
		tmpAdr.s6_addr[0] = 0x02;
		tmpAdr.s6_addr[1] = lOnes;

		for(int i =2; i < 16; i++) 
			tmpAdr.s6_addr[i]=temp[i-2];
	/*	std::cout << "IPv6:";
		for(int i=0;i < 16; i++){
			std::cout << int(tmpAdr.s6_addr[i]) << "," << std::endl;
		}
		std::cout << std::endl;
		std::cout << "temp:";
		for(int i=0;i < 16; i++){
			std::cout << int(temp[i]) << "," << std::endl;
		}*/
		char * addr = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));
		inet_ntop(AF_INET6, &tmpAdr, addr, INET6_ADDRSTRLEN);
		return addr;
}	
static inline void miner(const char * prefix)
{
	while(!found)
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
		if(	( options.reg ? !NotThat(ipv6, options.regex) : !NotThat(ipv6,prefix) ) )
		{
			found=true;
			std::cout <<"Address founded: " << ipv6 << std::endl;
			memcpy(dataKey.sk, myKeys.PrivateKey, sizeof(myKeys.PrivateKey));
			memcpy(dataKey.pk, myKeys.PublicKey, sizeof(myKeys.PublicKey));
			dataKey.ip =std::string(ipv6);
		}
		free(ipv6);
		puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

	}
}

int main(int argc, char**argv){
	if ( argc < 2 )
	{
		usage();
		return 0;
	}
	parsing( argc > 2 ? argc-1 : argc, argc > 2 ? argv+1 : argv);

	if(options.reg) options.regex=std::regex(argv[1]);
	if ( options.threads < 0 ) options.threads=1;
	std::vector<std::thread> threads(options.threads);
	std::cout << "threads " <<  "" << std::endl;
	for ( unsigned int j = options.threads;j--;)
	{
		std::cout << "thread " << j << "start" << std::endl;
		threads[j] = std::thread(static_cast<void(*)(const char*)>(miner), argv[1]);
		std::cout << "thread " << j << "started" << std::endl;
	}//for
	for(unsigned int j = 0; j < (unsigned int)options.threads;j++)
		threads[j].join();
	if(options.outputpath.size() == 0) options.outputpath="keys.txt";
	std::ofstream f (options.outputpath, std::ofstream::binary | std::ofstream::out);
	if (f)
	{
		f << "Your keys: " << std::endl;
		f << "Secret key: ";
		for(int i = 0; i < KEYSIZE; ++i)
		{
		 f << std::setw(2) << std::setfill('0') << std::hex << (int)dataKey.sk[i];
		}
		f << std::endl;

		f << "Public Key: ";
		for(int i = 0; i < 32; ++i)
		{
		 f << std::setw(2) << std::setfill('0') << std::hex << (int)dataKey.pk[i];
		}

		f << std::endl;
		f << "IPv6: " <<dataKey.ip << std::endl;
	}
	else
		std::cout << "Can't create file " << options.outputpath << std::endl;	

}
