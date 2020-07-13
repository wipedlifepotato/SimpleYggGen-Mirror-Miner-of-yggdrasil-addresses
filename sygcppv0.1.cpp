#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

/*
 * В первой бэта-версии майнер ищет исключительно двойное двоеточие.
 * По опыту скажу, это месяцы. Однако для энтузиастов предоставляю исходный текст
 * программы. Цель майнинга можно изменить в строке №73.
 *
 * Предложения по коду присылайте на почту: acetone-at-disroot.org
 *
 */

int main() {
  SetConsoleOutputCP(65001) ; // Необходимо в Windows для корректного отображения вывода

  std::cout << "+-----------------------------------------------+\n"
            << "| ▒▒▒▒▒▒ ▒    ▒ ▒▒▒▒▒▒     SimpleYggGen CPP     |\n"
            << "| ▒      ▒    ▒ ▒          Version: public_0.1  |\n"
            << "| ▒▒▒▒▒▒ ▒▒▒▒▒▒ ▒  ▒▒▒                          |\n"
            << "|      ▒      ▒ ▒    ▒     acetone, 2020        |\n"
            << "| ▒▒▒▒▒▒ ▒▒▒▒▒▒ ▒▒▒▒▒▒     © GPLv3 or later     |\n"
            << "+-----------------------------------------------+\n\n" ;

  /* Чтение файла yggbin.conf для получения пути до бинарника Yggdrasil */
  std::string bin ;
  std::ifstream in("yggbin.conf") ;

  if( !in ) {
    std::cerr << "[ERR] #1: yggbin.conf not found\n\n";
    system("PAUSE") ;
    return 1 ;

  } else {
    while(getline(in, bin)) {}
      }
    std::cout << "[OK ] Config yggbin.conf detected\n";

    if(bin.empty()) {
      std::cerr << "[ERR] #2: The last line in yggbin.conf turned out to be empty\n\n" ;
      system("PAUSE") ;
      return 2;
    }

    { // Дополнительный блок, в котором создается и умирают add_version и
      // bin_with_version. Стринги для тестирования бинарника.
     std::string add_version = " -version" ;
     std::string bin_with_version ;
     bin_with_version = bin + add_version ;
     if (system(bin_with_version.c_str()) == 0) {
     std::cout << "[OK ] Binary file tested successfully\n\n" ;
     } else {
       std::cerr << "[ERR] #3: Bad binary file: " << bin.c_str() <<"\n\n" ;
       system("PAUSE") ;
       return 3 ;
       }
    }
  in.close() ;

  std::cout << std::endl ;

  std::string add_genconf = " -genconf > yggdrasil_syg.conf" ;
  std::string bin_genconf;
  bin_genconf = bin + add_genconf ;

  std::string add_useconffile_address = " -address -useconffile yggdrasil_syg.conf > sygaddr.tmp" ;
  std::string bin_useconffile_address ;
  bin_useconffile_address = bin + add_useconffile_address ;

  std::string test_address ;

  bool fartune = false;

  for( int k = 1; fartune == false; ++k ) {
    system(bin_genconf.c_str()) ;
    system(bin_useconffile_address.c_str()) ;

    std::ifstream test_address_input("sygaddr.tmp") ;
    getline(test_address_input, test_address) ;
        std::cout << "# " << k << " #\t"<< test_address << "\t #\n";
    if ( test_address.find("12") != std::string::npos ) {
      std::cout << "\n[OK ] MATCH FOUND!\n" ;
      test_address_input.close() ;
      system("del sygaddr.tmp") ;
      fartune = true ;
    }
  }
}
