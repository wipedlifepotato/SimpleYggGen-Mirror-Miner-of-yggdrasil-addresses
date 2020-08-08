#include"ipv6.h"
//#define INET_PTON6_WITHOUT_WS32
#ifdef INET_PTON6_WITHOUT_WS32
	#define NS_IN6ADDRSZ        16        /*%< IPv6 T_AAAA */
	#define NS_INADDRSZ        4        /*%< IPv4 T_A */
	#define NS_INT16SZ        2        /*%< #/bytes of data in a uint16_t */
	#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
	#include <stdio.h>

	#ifdef SPRINTF_CHAR
	# define SPRINTF(x) strlen(sprintf/**/x)
	#else
	# define SPRINTF(x) ((size_t)sprintf x)
	#endif	
	# define __set_errno(val) (errno = (val))
	#include<errno.h>
	static const char *
	inet_ntop4 (const u_char *src, char *dst, socklen_t size)
	{
		static const char fmt[] = "%u.%u.%u.%u";
		char tmp[sizeof "255.255.255.255"];
		if (SPRINTF((tmp, fmt, src[0], src[1], src[2], src[3])) >= size) {
		        __set_errno (ENOSPC);
		        return (NULL);
		}
		return strcpy(dst, tmp);
	}
	/* const char *
	 * inet_ntop6(src, dst, size)
	 *        convert IPv6 binary address into presentation (printable) format
	 * author:
	 *        Paul Vixie, 1996.
	 */
	static const char *
	inet_ntop6 (const u_char *src, char *dst, socklen_t size)
	{
		/*
		 * Note that int32_t and int16_t need only be "at least" large enough
		 * to contain a value of the specified size.  On some systems, like
		 * Crays, there is no such thing as an integer variable with 16 bits.
		 * Keep this in mind if you think this function should have been coded
		 * to use pointer overlays.  All the world's not a VAX.
		 */
		char tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"], *tp;
		struct { int base, len; } best, cur;
		u_int words[NS_IN6ADDRSZ / NS_INT16SZ];
		int i;
		/*
		 * Preprocess:
		 *        Copy the input (bytewise) array into a wordwise array.
		 *        Find the longest run of 0x00's in src[] for :: shorthanding.
		 */
		memset(words, '\0', sizeof words);
		for (i = 0; i < NS_IN6ADDRSZ; i += 2)
		        words[i / 2] = (src[i] << 8) | src[i + 1];
		best.base = -1;
		cur.base = -1;
		best.len = 0;
		cur.len = 0;
		for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
		        if (words[i] == 0) {
		                if (cur.base == -1)
		                        cur.base = i, cur.len = 1;
		                else
		                        cur.len++;
		        } else {
		                if (cur.base != -1) {
		                        if (best.base == -1 || cur.len > best.len)
		                                best = cur;
		                        cur.base = -1;
		                }
		        }
		}
		if (cur.base != -1) {
		        if (best.base == -1 || cur.len > best.len)
		                best = cur;
		}
		if (best.base != -1 && best.len < 2)
		        best.base = -1;
		/*
		 * Format the result.
		 */
		tp = tmp;
		for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
		        /* Are we inside the best run of 0x00's? */
		        if (best.base != -1 && i >= best.base &&
		            i < (best.base + best.len)) {
		                if (i == best.base)
		                        *tp++ = ':';
		                continue;
		        }
		        /* Are we following an initial run of 0x00s or any real hex? */
		        if (i != 0)
		                *tp++ = ':';
		        /* Is this address an encapsulated IPv4? */
		        if (i == 6 && best.base == 0 &&
		            (best.len == 6 || (best.len == 5 && words[5] == 0xffff))) {
		                if (!inet_ntop4(src+12, tp, sizeof tmp - (tp - tmp)))
		                        return (NULL);
		                tp += strlen(tp);
		                break;
		        }
		        tp += SPRINTF((tp, "%x", words[i]));
		}
		/* Was it a trailing run of 0x00's? */
		if (best.base != -1 && (best.base + best.len) ==
		    (NS_IN6ADDRSZ / NS_INT16SZ))
		        *tp++ = ':';
		*tp++ = '\0';
		/*
		 * Check for overflow, copy, and we're done.
		 */
		if ((socklen_t)(tp - tmp) > size) {
		        __set_errno (ENOSPC);
		        return (NULL);
		}
		return strcpy(dst, tmp);
	}
	const char *
	inet_ntop (int af, const void *src, char *dst, socklen_t size)
	{
		switch (af) {
		case AF_INET:
		        return (inet_ntop4(src, dst, size));
		case AF_INET6:
		        return (inet_ntop6(src, dst, size));
		default:
		        __set_errno (EAFNOSUPPORT);
		        return (NULL);
		}
		/* NOTREACHED */
	}

#endif
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
