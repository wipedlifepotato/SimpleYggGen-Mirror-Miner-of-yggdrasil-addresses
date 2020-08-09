#pragma once
#define NS_IN6ADDRSZ        16        /*%< IPv6 T_AAAA */
#define NS_INADDRSZ        4        /*%< IPv4 T_A */
#define NS_INT16SZ        2        /*%< #/bytes of data in a uint16_t */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define AF_INET		2	/* Internet IP Protocol 	*/
#define AF_INET6	10	/* IP version 6			*/
/* Type for length arguments in socket calls.  */
#include<string.h>

#include <stdio.h>
#define INET6_ADDRSTRLEN 46
#ifdef SPRINTF_CHAR
# define SPRINTF(x) strlen(sprintf/**/x)
#else
# define SPRINTF(x) ((size_t)sprintf x)
#endif	
# define __set_errno(val) (errno = (val))
#include <errno.h>

struct in6_addr {
               unsigned char   s6_addr[16];   /* IPv6 address */
           };
struct sockaddr_in6 {
          unsigned char           sin6_len;      /* length of this structure */
          unsigned char           sin6_family;   /* AF_INET6                 */
          unsigned int __attribute((mode(HI)))       sin6_port;     /* Transport layer port #   */
          unsigned int __attribute((mode(SI)))       sin6_flowinfo; /* IPv6 flow information    */
          struct in6_addr  sin6_addr;     /* IPv6 address             */
};
struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

static const char *
	inet_ntop4 (const unsigned *src, char *dst, ssize_t size);
static const char *
	inet_ntop6 (const unsigned char *src, char *dst, ssize_t size);
const char *
	inet_ntop (int af, const void *src, char *dst, ssize_t size);

