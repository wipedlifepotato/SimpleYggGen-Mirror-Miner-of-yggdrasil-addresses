/*
 * Спасибо PurpleI2P Project за активное содействие в написании этого кода.
 * acetone, 2020 (c) GPLv3
 * notabug.org/acetone/SimpleYggGen-CPP
 *
 */
#include <openssl/evp.h> // библиотека OpenSSL
#include <iostream>      // вывод на экран
#include <iomanip>       // форматированный вывод строк
#include <ctime>         // системное время

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
  int rv = 60;
  std::cout << std::endl << getrandom(2,44)                                                                                                                                                        << " # -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~ " << std::endl
            << getrandom(rv, 2)  << "          " << getrandom(rv, 5) << "  " << getrandom(rv, 6) << "  " << getrandom(rv, 5)  << "          " << getrandom(rv, 2)                                  << " # [     SimpleYggGen C++ 1.0 2020-08     ] " << std::endl
            << getrandom(rv, 2)  << "  "         << getrandom(rv,13) << "  " << getrandom(rv, 6) << "  " << getrandom(rv, 5)  << "  "         << getrandom(rv, 10)                                 << " # [ notabug.org/acetone/SimpleYggGen-CPP ] " << std::endl
            << getrandom(rv, 2)  << "          " << getrandom(rv, 5) << "          "                     << getrandom(rv, 5)  << "  "         << getrandom(rv, 3)   << "     " << getrandom(rv, 2) << " # [           acetone (c) GPLv3          ] " << std::endl
            << getrandom(rv, 10) <<         "  " << getrandom(rv,13) <<         "  "                     << getrandom(rv, 5)  << "  "         << getrandom(rv, 6)   <<    "  " << getrandom(rv, 2) << " # [                                      ] " << std::endl
            << getrandom(rv, 2)  << "          " << getrandom(rv, 5) << "          "                     << getrandom(rv, 5)  << "          " << getrandom(rv, 2)                                  << " # [   OpenSSL inside: x25519 -> sha512   ] " << std::endl
            << getrandom(2,44)                                                                                                                                                                     << " # ";

  for(int k = 1; k < 41; ++k) {
    if(k%2==0) {
        std::cout << "-";
      } else {
        std::cout << "~";
      }
  }
  std::cout << std::endl;
}

void miner()
{
  EVP_PKEY_CTX * Ctx;
  EVP_PKEY * Pkey = nullptr;

  Ctx = EVP_PKEY_CTX_new_id (NID_X25519, NULL);

  uint8_t PublicKey[32];
  uint8_t PrivateKey[32];

  EVP_PKEY_keygen_init (Ctx);
  EVP_PKEY_keygen (Ctx, &Pkey);

  size_t lenpub = 32;
  EVP_PKEY_get_raw_public_key (Pkey, PublicKey, &lenpub);

  size_t lenpriv = 32;
  EVP_PKEY_get_raw_private_key (Pkey, PrivateKey, &lenpriv);

  std::cout << std::endl << std::left;
  // std::cout << "\nITERATION: [" << std::setw(24) << "500" << "] ADDRESS: [" << std::setw(38) << "200:a3d2:b6c3::a138:7836:edef" << "]\n";
  std::cout << "Public:  ";
  for(int i = 0; i < 32; ++i)
  {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)PublicKey[i];
  }
  std::cout << std::endl;

  std::cout << "Private: ";
  for(int i = 0; i < 32; ++i)
  {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)PrivateKey[i];
  }
  std::cout << std::endl;

}

// ------------------------------------------------------

int main()
{
  srand(time(NULL));
  intro();
  miner();
  return 0;
}
