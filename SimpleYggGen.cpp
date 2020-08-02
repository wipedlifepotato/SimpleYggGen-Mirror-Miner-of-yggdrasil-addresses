/*
 * Спасибо PurpleI2P Project за активное содействие в написании этого кода.
 * notabug.org/acetone/SimpleYggGen-CPP
 *
 * acetone (c) GPLv3
 *
 */

#include"SimpleYggGen.hpp"

//////////////////////////////////////////////////begin Заставка и прочая вода

const char randomtable[90] =
{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
  'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', '!', '@', '(', ')', '/', '-', '#', '+',
  '$', '%', '^', '&', '*', '`', '~', '>', '<', '?',
  '{', '}', '[', ']', ';', ':', '_', '=', '|', '\''
};

static inline std::string getrandom(int entropy, unsigned int size_of_line)
{
	std::string random_value;
	while(random_value.size() < size_of_line)
	{
		random_value += randomtable[(std::rand() % entropy)];
	}
	random_value.shrink_to_fit();
	return random_value;
}
#ifdef __linux__ 
constexpr const char* RST = "\x1B[0m";
static inline std::string getRandomColor(){
	auto str = std::ostringstream();
	str << "\x1B[";
	str<<(30+(std::rand() % 8));
	str << "m";
	return str.str();
}
#endif

void intro()
{
	srand(time(NULL));
	int rv = 60;
	std::cout
	/*<< __FILE__*/
#ifdef __linux__
<< getRandomColor() << std::endl 
#else
 << std::endl 
#endif
	<<"|                                      |" << getrandom(2,44)   << std::endl
	<< "| "<<NAMEPROGRAM<<" C++ 1.0-headhunter 2020 |" << getrandom(rv, 2)  << "          "  << getrandom(rv, 5) << "  " << getrandom(rv, 6) << "  " << getrandom(rv, 5)  << "          " << getrandom(rv, 2)	<< std::endl
	<< "|   OpenSSL inside: x25519 -> sha512   |" << getrandom(rv, 2)  << "  "          << getrandom(rv,13) << "  " << getrandom(rv, 6) << "  " << getrandom(rv, 5)  << "  "         << getrandom(rv, 10)	<< std::endl
	<< "| notabug.org/acetone/SimpleYggGen-CPP |" << getrandom(rv, 2)  << "          "  << getrandom(rv, 5) << "          "                     << getrandom(rv, 5)  << "  "         << getrandom(rv, 3)	<< "     " << getrandom(rv, 2) << std::endl
	<< "|           acetone (c) GPLv3          |" << getrandom(rv, 10) <<         "  "  << getrandom(rv,13) <<         "  "                     << getrandom(rv, 5)  << "  "         << getrandom(rv, 6)	<<    "  " << getrandom(rv, 2) << std::endl
	<< "|                                      |" << getrandom(rv, 2)  << "          "  << getrandom(rv, 5) << "          "                     << getrandom(rv, 5)  << "          " << getrandom(rv, 2)	<< std::endl
	<< "|     "  << __DATE__ << "         "  << __TIME__ << "     |"	    << getrandom(2,44) <<
std::endl<<"Co-authors: "<< COAUTHORS<< std::endl
#ifdef __linux__
<< RST << std::endl ;
#else
 << std::endl ;
#endif
}
//end

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
		struct in6_addr tmpAdr;
		tmpAdr.s6_addr[0] = 0x02;
		tmpAdr.s6_addr[1] = lOnes;
		for(int i =2; i < 16; i++) 
			tmpAdr.s6_addr[i]=temp[i-2];
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
			std::cout <<"Address found: " << ipv6 << std::endl;
			memcpy(dataKey.sk, myKeys.PrivateKey, sizeof(myKeys.PrivateKey));
			memcpy(dataKey.pk, myKeys.PublicKey, sizeof(myKeys.PublicKey));
			dataKey.ip =std::string(ipv6);
		}
		free(ipv6);
		puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

	}
}
int main(int argc, char**argv){
	intro();
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
