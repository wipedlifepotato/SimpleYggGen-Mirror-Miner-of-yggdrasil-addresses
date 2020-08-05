/*
 * Спасибо PurpleI2P Project за активное содействие в написании этого кода.
 * notabug.org/acetone/SimpleYggGen-CPP
 *
 * acetone, lialh4 (c) GPLv3
 *
 */

#include <openssl/evp.h> // библиотека OpenSSL
#include <openssl/sha.h>
#include <iostream>      // вывод на экран
#include <iomanip>       // форматированный вывод строк
#include <ctime>         // системное время
#include <bitset>        // побитовое чтение
#include <cstring>		 // memcmp - побайтовое сравнение
#include <vector>

#define KEYSIZE 32

////////////////////////////////////////////////// Заставка и прочая вода

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

		std::string getrandom(int entropy, unsigned int size_of_line)
		{
			std::string random_value;
			while(random_value.size() < size_of_line)
			{
				random_value += randomtable[(std::rand() % entropy)];
			}
			random_value.shrink_to_fit();
			return random_value;
		}

		void intro()
		{
			srand(time(NULL));
			int rv = 60;
			std::cout << std::endl
			<< "|   SimpleYggGen C++ 1.0-headhunter    |" << getrandom(2,44)   << std::endl
			<< "|   OpenSSL inside: x25519 -> sha512   |" << getrandom(rv, 2)  << "          "  << getrandom(rv, 5) << "  " << getrandom(rv, 6) << "  " << getrandom(rv, 5)  << "          " << getrandom(rv, 2)	<< std::endl
			<< "| notabug.org/acetone/SimpleYggGen-CPP |" << getrandom(rv, 2)  << "  "          << getrandom(rv,13) << "  " << getrandom(rv, 6) << "  " << getrandom(rv, 5)  << "  "         << getrandom(rv, 10)	<< std::endl
			<< "|                                      |" << getrandom(rv, 2)  << "          "  << getrandom(rv, 5) << "          "                     << getrandom(rv, 5)  << "  "         << getrandom(rv, 3)	<< "     " << getrandom(rv, 2) << std::endl
			<< "| developers: lialh4, acetone, orignal |" << getrandom(rv, 10) <<         "  "  << getrandom(rv,13) <<         "  "                     << getrandom(rv, 5)  << "  "         << getrandom(rv, 6)	<<    "  " << getrandom(rv, 2) << std::endl
			<< "|            GPLv3  (c) 2020           |" << getrandom(rv, 2)  << "          "  << getrandom(rv, 5) << "          "                     << getrandom(rv, 5)  << "          " << getrandom(rv, 2)	<< std::endl
			<< "| "  << __DATE__ << "                 "  << __TIME__ <<  " |"	    << getrandom(2,44) << std::endl;
		}

////////////////////////////////////////////////// Суть вопроса

struct BoxKeys
{
    uint8_t PublicKey[KEYSIZE];
    uint8_t PrivateKey[KEYSIZE];
};

BoxKeys getKeyPair(void)
{
	BoxKeys keys;
	size_t len = KEYSIZE;

	EVP_PKEY_CTX * Ctx;
	EVP_PKEY * Pkey = nullptr;
	Ctx = EVP_PKEY_CTX_new_id (NID_X25519, NULL);

	EVP_PKEY_keygen_init (Ctx);
	EVP_PKEY_keygen (Ctx, &Pkey);

	EVP_PKEY_get_raw_public_key (Pkey, keys.PublicKey, &len);
	EVP_PKEY_get_raw_private_key (Pkey, keys.PrivateKey, &len);

	EVP_PKEY_CTX_free(Ctx);
	EVP_PKEY_free(Pkey);

	return keys;
}

int Ones(unsigned char HashValue[SHA512_DIGEST_LENGTH])
{
	unsigned char byte;
	bool done;
	int lOnes=0;
	int nBits=0;
	unsigned char temp[SHA512_DIGEST_LENGTH];
	memset(temp, 0, sizeof(temp));
	int z = 0;
	std::vector<std::bitset<8>> bytes;
	for(auto i = 0; i < SHA512_DIGEST_LENGTH; i++)
	{
		bytes.push_back(HashValue[i]);
	}

	for(auto bits : bytes){
		for(int i = 7; i >= 0; --i)
		{
			if(bits[i] == 1 && !done)
			{
				++lOnes;
				continue;
			}
			if(bits[i] == 0 && !done)
			{
				done = true;
				continue;
			}

			byte = (byte << 1) | (bits[i] > 0 ? 1 : 0);
			++nBits;

			if(nBits >= 8)
			{
				nBits = 0;
				temp[++z] = byte;
			}
		}
	}
	return lOnes;
}

void miner()
{
	unsigned char HashValue[SHA512_DIGEST_LENGTH];

	uint8_t PublicKeyBest[KEYSIZE];
	uint8_t PrivateKeyBest[KEYSIZE];

	// ------------------------ ОСНОВНОЙ ЦИКЛ

	int bitcount = 9; 	// переменная для хранения наибольшего количества единиц (не с нуля начинаем)

	int totalcount= 0;	// счетчик основного цикла
	bool count50 = false; // счетчики для отображения прогресса
	bool count100 = true;
	bool count500 = true;

	while(true)
	{
		BoxKeys myKeys = getKeyPair();
		SHA512(myKeys.PublicKey, KEYSIZE, HashValue);
		int newones = Ones(HashValue);

		if(newones > bitcount)
		{
			bitcount = newones;
			for(int i = 0; i < KEYSIZE; ++i)
			{
				PublicKeyBest[i] = myKeys.PublicKey[i];
				PrivateKeyBest[i] = myKeys.PrivateKey[i];
			}
			std::cout << "\nAddress:    [2" << std::setw(2) << std::setfill('0') << std::hex << bitcount << ":...]" << std::endl;
			std::cout << "PublicKey:  ";
			for(int i = 0; i < 32; ++i)
			{
				std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)PublicKeyBest[i];
			}
			std::cout << std::endl;

			std::cout << "PrivateKey: ";
			for(int i = 0; i < 32; ++i)
			{
				std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)PrivateKeyBest[i];
			}
			std::cout << std::endl;
			count50 = false;
			count100 = true;
			count500 = true;
			totalcount = 0;
		}


		++totalcount;
		if(totalcount % 50000 == 0 && !count50)
		{
			std::cerr << "50k ";
			std::cerr.flush();
			count50 = true;
			count100 = false;
			continue;
		}
		if(totalcount % 100000 == 0  && !count100)
		{
			std::cerr << "100k ";
			std::cerr.flush();
			count100 = true;
			count500 = false;
			continue;
		}
		if(totalcount % 500000 == 0  && !count500)
		{
			std::cerr << "500k ";
			std::cerr.flush();
			count500 = true;
			continue;
		}
		if(totalcount % 500000 == 0)
		{
			std::cerr << "# ";
			std::cerr.flush();
		}
	}
}

// ------------------------------------------------------

int main()
{
	intro();
	miner();
}