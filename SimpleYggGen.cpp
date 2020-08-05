/*
 * Спасибо PurpleI2P Project за активное содействие в написании этого кода.
 * notabug.org/acetone/SimpleYggGen-CPP
 *
 * acetone (c) GPLv3
 *
 */

#include "SimpleYggGen.hpp"
#include "ncurses/ncurses-interface.h"
#include <mutex>
#include <unistd.h>

// extern OptionBox getOption(void);
static unsigned long long foundAddreses = 0;

//////////////////////////////////////////////////begin Заставка и прочая вода

const char randomtable[90] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
    'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
    'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '!', '@', '(', ')', '/', '-', '#', '+', '$', '%', '^', '&', '*',
    '`', '~', '>', '<', '?', '{', '}', '[', ']', ';', ':', '_', '=', '|', '\''};

static inline std::string getrandom(int entropy, unsigned int size_of_line) {
  std::string random_value;
  while (random_value.size() < size_of_line) {
    random_value += randomtable[(std::rand() % entropy)];
  }
  random_value.shrink_to_fit();
  return random_value;
}
#ifdef __linux__
constexpr const char *RST = "\x1B[0m";
static inline std::string getRandomColor() {
#ifndef __linux__
  return "";
#endif
  // unsigned short Background_color = (std::rand() % 4 + 41);//41 -> red
  unsigned short Foreground_color =
      (std::rand() % 3 + 32); // 30-> black 37-> white; 31 = red
  // if(Background_color - 10 == Foreground_color) Background_color=4;
  auto str = std::ostringstream();

  str << "\033[1;4;46;"; // bold, underline, cyan background
  str << Foreground_color << "m";

  return str.str();
}
#endif

void intro() {
  srand(time(NULL));
  int rv = 60;
  std::cout << getRandomColor() << std::endl
            << "|                                      |" << getrandom(2, 44)
            << std::endl
            << "| " << NAMEPROGRAM << " C++ 1.0-headhunter 2020 |"
            << getrandom(rv, 2) << "          " << getrandom(rv, 5) << "  "
            << getrandom(rv, 6) << "  " << getrandom(rv, 5) << "          "
            << getrandom(rv, 2) << std::endl
            << "|   OpenSSL inside: x25519 -> sha512   |" << getrandom(rv, 2)
            << "  " << getrandom(rv, 13) << "  " << getrandom(rv, 6) << "  "
            << getrandom(rv, 5) << "  " << getrandom(rv, 10) << std::endl
            << "| notabug.org/acetone/SimpleYggGen-CPP |" << getrandom(rv, 2)
            << "          " << getrandom(rv, 5) << "          "
            << getrandom(rv, 5) << "  " << getrandom(rv, 3) << "     "
            << getrandom(rv, 2) << std::endl
            << "|           acetone (c) GPLv3          |" << getrandom(rv, 10)
            << "  " << getrandom(rv, 13) << "  " << getrandom(rv, 5) << "  "
            << getrandom(rv, 6) << "  " << getrandom(rv, 2) << std::endl
            << "|                                      |" << getrandom(rv, 2)
            << "          " << getrandom(rv, 5) << "          "
            << getrandom(rv, 5) << "          " << getrandom(rv, 2) << std::endl
            << "|     " << __DATE__ << "         " << __TIME__ << "     |"
            << getrandom(2, 44) << std::endl
            << "Co-authors: " << COAUTHORS << std::endl
#ifdef __linux__
            << RST << std::endl;
#else
            << std::endl;
#endif
}
// end

static unsigned int maxlones = 0;

static dataKey pDatakey;

static struct {
  bool reg = false;
  int threads = -1;
  std::string outputpath = "";
  std::regex regex;
  ProgramMode mode = ProgramMode::search;
  unsigned long long limit = -1;
  char *searchtextby = nullptr; // from ncurses if set
} options;

static inline bool NotThat(const char *what, const std::regex &reg) {
  return std::regex_match(what, reg) == 1 ? false : true;
}

static inline bool NotThat(const char *a, const char *b) {
  while (*b)
    if (*a++ != *b++)
      return true;
  return false;
}

void usage(void) {
  const constexpr char *help = NAMEPROGRAM
      " [text-pattern(for just search by text, like to 200:ffff)] [options]\n"
      "-h --help, help menu\n"
      "-r --reg, (^2.*ffff.*$)  regexp instead just text pattern (e.g. '(one|two).*')\n"
      "--threads -t, (default count of system)\n"
      "-o --output output file (default keys.txt)\n"
      "--usage this menu\n"
      "--highhead -H mode of high head...\n"
      "--searchadress -s (default) mode\n"
      "--limitfound=n -lN limit found\n"
      "--ncurses -n start ncurses interface\n"
      //"--prefix -p\n"
      "";
  puts(help);
}

void parsing(int argc, char **args) {
  int option_index;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"reg", required_argument, 0, 'r'},
      {"threads", required_argument, 0, 't'},
      {"output", required_argument, 0, 'o'},
      {"usage", no_argument, 0, 0},
      {"highhead", no_argument, 0, 'H'},
      {"searchadress", no_argument, 0, 's'},
      {"limitfound", required_argument, 0, 'l'},
      {"ncurses", no_argument, 0, 'n'},
      {0, 0, 0, 0}};

  int c;
  OptionBox ncursesoptions;
  auto setRegExp = [](const char*searchbytext){
      options.reg = true;
      options.searchtextby = new char[MAXBUF];
      memcpy(options.searchtextby, searchbytext, strlen(searchbytext) );
      std::cout << "RegExp pattern: "<<searchbytext << std::endl;	
      options.regex = std::regex( options.searchtextby );
  };
  while ((c = getopt_long(argc, args, "hr:t:so:Hsl:n", long_options,
                          &option_index)) != -1) {
    switch (c) {
    case 0:
      if (std::string(long_options[option_index].name) ==
          std::string("usage")) {
        usage();
        exit(1);
      }
    case 'n':
      ncursesoptions = getOption();
      switch (ncursesoptions.engine) {
      case HighHead:
        options.mode = ProgramMode::high;
        options.outputpath = defaultHighSearchFileName;
        break;
      case RegExp:
        options.reg = true;
        setRegExp(ncursesoptions.searchtext);
	//goto REGEXP;
        break;
      default:
      	 options.searchtextby = new char[MAXBUF];
     	 memcpy(options.searchtextby, ncursesoptions.searchtext, MAXBUF);       
      }
      break;

    case 'l':
      options.limit = atoi(optarg);
      break;
    case 'H':
      options.mode = ProgramMode::high;
      options.outputpath = defaultHighSearchFileName;
      break;
    case 's':
      options.mode = ProgramMode::search;
      options.outputpath = defaultSearchFileName;
      break;
    case 'h':
      usage();
      exit(0);
      break;
    case 'r':
      setRegExp(optarg);
      break;
    case 't':
      options.threads = atoi(optarg);
      break;
    case 'o':
      options.outputpath = optarg;
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

BoxKeys getKeyPair(void) {
  BoxKeys keys;
  size_t lenpub = KEYSIZE;
  size_t lenpriv = KEYSIZE;

  EVP_PKEY_CTX *Ctx;
  EVP_PKEY *Pkey = nullptr;
  Ctx = EVP_PKEY_CTX_new_id(NID_X25519, NULL);

  EVP_PKEY_keygen_init(Ctx);
  EVP_PKEY_keygen(Ctx, &Pkey);

  EVP_PKEY_get_raw_public_key(Pkey, keys.PublicKey, &lenpub);
  EVP_PKEY_get_raw_private_key(Pkey, keys.PrivateKey, &lenpriv);

  EVP_PKEY_CTX_free(Ctx);
  EVP_PKEY_free(Pkey);

  return keys;
}
void getSHA512(void *data, unsigned char *hash) {
  SHA512_CTX sha512;
  SHA512_Init(&sha512);
  SHA512_Update(&sha512, data, KEYSIZE);
  SHA512_Final(hash, &sha512);
}
void convertSHA512ToSum(unsigned char hash[SHA512_DIGEST_LENGTH],
                        char outputBuffer[128]) {
  for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
    sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
  }
}
char *convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH],
                          BoxKeys myKeys, int &cOnes) {
  // char hash[128];
  // convertSHA512ToSum(h, hash);
  unsigned char byte;
  bool done;
  unsigned char lOnes = 0;
  unsigned char nBits = 0;
  unsigned char temp[SHA512_DIGEST_LENGTH];
  memset(temp, 0, sizeof(temp));
  int z = 0;
  std::vector<std::bitset<8>> bytes;
  for (auto i = 0; i < SHA512_DIGEST_LENGTH; i++)
    bytes.push_back(hash[i]);

  for (auto bits : bytes) {
    for (int i = 7; i >= 0; i--) {
      if (bits[i] == 1 && !done) {
        lOnes++;
        continue;
      }
      if (!done && bits[i] == 0) {
        done = true;
        continue;
      }
      byte = (byte << 1) | (bits[i] > 0 ? 1 : 0);
      nBits++;
      if (nBits >= 8) {
        nBits = 0;
        temp[z++] = byte;
        // std::cout << "val:" << int(temp[i-1]) << std::endl;
      }
    }
  }
  struct in6_addr tmpAdr;
  tmpAdr.s6_addr[0] = 0x02;
  tmpAdr.s6_addr[1] = lOnes;
  for (int i = 2; i < 16; i++)
    tmpAdr.s6_addr[i] = temp[i - 2];
  char *addr = (char *)calloc(INET6_ADDRSTRLEN, sizeof(char));
  inet_ntop(AF_INET6, &tmpAdr, addr, INET6_ADDRSTRLEN);
  cOnes = lOnes;
  return addr;
}
char *convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH],
                          BoxKeys myKeys) {
  int o;
  return convertSHA512ToIPv6(hash, myKeys, o);
}

std::mutex m_writeMutex;
static inline void addKeyPair(BoxKeys data, std::string ipv6) {
  std::lock_guard<std::mutex> guard(m_writeMutex);
  std::ofstream f(options.outputpath, std::ofstream::out | std::ofstream::app);
  if (f) {
    f << "~" << std::endl;
    f << (options.mode == ProgramMode::high ? "HighMode" : "SearchMode")
      << std::endl;
    f << "Your keys: " << std::endl;
    f << "Secret key: ";
    for (int i = 0; i < KEYSIZE; ++i) {
      f << std::setw(2) << std::setfill('0') << std::hex
        << (int)data.PrivateKey[i];
    }
    f << std::endl;

    f << "Public Key: ";
    for (int i = 0; i < 32; ++i) {
      f << std::setw(2) << std::setfill('0') << std::hex
        << (int)data.PublicKey[i];
    }

    f << std::endl;
    f << "IPv6: " << ipv6 << std::endl;
    f << "~" << std::endl;
  } else
    std::cout << "Can't create/reopen file " << options.outputpath << std::endl;
}

static inline void miner(const char *prefix) {
  if (options.reg && prefix[0] != '^' && options.mode != ProgramMode::high) {
    std::cerr << "WARNING: "
              << "IF YOU DONT KNOW REGEXP PLEASE SEE IT -> https://regexr.com/"
              << std::endl << prefix << std::endl;
    sleep(15); // magic number
  } else if (prefix[0] != '2' && options.mode != ProgramMode::high && !options.reg) {
    std::cerr << "WARNING: "
              << "YOU WANT TO FOUND ADRESS WHICH NOT EXIST IN YGGDRASIL, ARE "
                 "YOU OKEY?!"
              << std::endl;
    sleep(30); // magic number
  }
  auto clearconsole = [](int defsleep = 1) {
#ifndef __linux__
    system("clear||cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
    intro();
    std::cout << getRandomColor();
    std::cout << "\b\b\b..." << std::flush;
    usleep(defsleep);
    std::cout << "\b\b\b.U." << std::flush;
    usleep(defsleep);
    std::cout << "\b\b\b..U" << std::flush;
    usleep(defsleep);
    std::cout << "\b\b\bvvU" << std::flush;
    usleep(defsleep);
    std::cout << "\b\b\bUvU" << std::flush;
    std::cout << "\b\b\b|" << std::flush;
  };
  while (options.limit < 0 || options.limit > foundAddreses) {
    std::cout << "\b/" << std::flush;
    std::cout << "\b\\" << std::flush;
    auto myKeys = getKeyPair();
    unsigned char hash[SHA512_DIGEST_LENGTH];
    getSHA512(myKeys.PublicKey, hash);
    char *ipv6 = convertSHA512ToIPv6(hash, myKeys);
    if (options.mode == ProgramMode::high) {
      int ones;
      ipv6 = convertSHA512ToIPv6(hash, myKeys, ones);
      if (ones > maxlones) {
        clearconsole();
        maxlones = ones;
        std::cout << "Found new max high-addr: "
                  << "(" << maxlones << ") " << ipv6 << std::endl;
        addKeyPair(myKeys, ipv6);
      }
    } else {
      ipv6 = convertSHA512ToIPv6(hash, myKeys);
      if ((options.reg ? !NotThat(ipv6, options.regex)
                       : !NotThat(ipv6, prefix))) {
        clearconsole();
        std::cout << "Address found: "
                  << "(" << ++foundAddreses << ") " << ipv6;
        std::cout << std::flush;
        addKeyPair(myKeys, ipv6);
        // delete newKey.sk; // not need. not-heap..
      }
    }
    free(ipv6);
  }
}
int main(int argc, char **argv) {
  intro();
  if (argc < 2) {
    usage();
    return 0;
  }
  options.outputpath = defaultSearchFileName;
  parsing(argc, argv);


  if (options.threads < 0)
    options.threads = std::thread::hardware_concurrency();
  ;
  std::vector<std::thread> threads(options.threads);

  for (unsigned int j = options.threads; j--;) {
    threads[j] = std::thread(
        static_cast<void (*)(const char *)>(miner),
        options.searchtextby == nullptr ? argv[1] : options.searchtextby);
  }
  for (unsigned int j = 0; j < (unsigned int)options.threads; j++)
    threads[j].join();
}
