#include"ipv6.h"
char *convertSHA512ToIPv6(unsigned char hash[SHA512_DIGEST_LENGTH]) {

  BitSet bitset = countMyBits(hash, SHA512_DIGEST_LENGTH);
  struct in6_addr tmpAdr;
  tmpAdr.s6_addr[0] = 0x02;
  tmpAdr.s6_addr[1] = bitset.ones;
  for (int i = 2; i < 16; i++)
    tmpAdr.s6_addr[i] = bitset.bytes[i-2];
  char *addr = (char *)calloc(INET6_ADDRSTRLEN, sizeof(char));
  inet_ntop(AF_INET6, &tmpAdr, addr, INET6_ADDRSTRLEN);
 // if(bitset.ones > maxlones)	 maxlones=bitset.ones;
  return addr;
}
