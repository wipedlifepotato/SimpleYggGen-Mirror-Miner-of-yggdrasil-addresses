#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <algorithm>
#include <iostream>
#include <cstring>
	static auto q = BN_new ();
	__attribute__((constructor)) void init_q(void){ 
		// 2^255-19
		BN_set_bit (q, 255); // 2^255
		BN_sub_word (q, 19);	
	}

	bool bn2buf (const BIGNUM * bn, uint8_t * buf, size_t len)
	{
		int offset = len - BN_num_bytes (bn);
		if (offset < 0) return false;
		BN_bn2bin (bn, buf + offset);
		memset (buf, 0, offset);
		return true;
	}

	template<int len>
	BIGNUM * DecodeBN (const uint8_t * buf) 
	{
		// buf is Little Endian convert it to Big Endian
		uint8_t buf1[len];
		for (size_t i = 0; i < len/2; i++) // invert bytes
		{
			buf1[i] = buf[len -1 - i];
			buf1[len -1 - i] = buf[i];
		}
		BIGNUM * res = BN_new ();
		BN_bin2bn (buf1, len, res);
		return res;
	}

	template<size_t len>
	void EncodeBN (const BIGNUM * bn, uint8_t * buf) 
	{
		bn2buf (bn, buf, len);
		// To Little Endian
		for (size_t i = 0; i < len/2; i++) // invert bytes
		{
			uint8_t tmp = buf[i];
			buf[i] = buf[len -1 - i];
			buf[len -1 - i] = tmp;
		}
	}

	BIGNUM * ScalarMul (const BIGNUM * u, const BIGNUM * k, BN_CTX * ctx) 
	{

		BN_CTX_start (ctx);
		auto x1 = BN_CTX_get (ctx); BN_copy (x1, u);
		auto x2 = BN_CTX_get (ctx); BN_one (x2);
		auto z2 = BN_CTX_get (ctx); BN_zero (z2);
		auto x3 = BN_CTX_get (ctx); BN_copy (x3, u);
		auto z3 = BN_CTX_get (ctx); BN_one (z3);
		auto c121666 = BN_CTX_get (ctx); BN_set_word (c121666, 121666);
		auto tmp0 = BN_CTX_get (ctx); auto tmp1 = BN_CTX_get (ctx);
		unsigned int swap = 0;
		auto bits = BN_num_bits (k);
		while(bits)
		{
			--bits;
			auto k_t = BN_is_bit_set(k, bits) ? 1 : 0;
			swap ^= k_t;
			if (swap)
			{
				std::swap (x2, x3);
				std::swap (z2, z3);
			}
			swap = k_t;
			BN_mod_sub(tmp0, x3, z3, q, ctx);
			BN_mod_sub(tmp1, x2, z2, q, ctx);
			BN_mod_add(x2, x2, z2, q, ctx);
			BN_mod_add(z2, x3, z3, q, ctx);
			BN_mod_mul(z3, tmp0, x2, q, ctx);
			BN_mod_mul(z2, z2, tmp1, q, ctx);
			BN_mod_sqr(tmp0, tmp1, q, ctx);
			BN_mod_sqr(tmp1, x2, q, ctx);
			BN_mod_add(x3, z3, z2, q, ctx);
			BN_mod_sub(z2, z3, z2, q, ctx);
			BN_mod_mul(x2, tmp1, tmp0, q, ctx);
			BN_mod_sub(tmp1, tmp1, tmp0, q, ctx);
			BN_mod_sqr(z2, z2, q, ctx);
			BN_mod_mul(z3, tmp1, c121666, q, ctx);
			BN_mod_sqr(x3, x3, q, ctx);
			BN_mod_add(tmp0, tmp0, z3, q, ctx);
			BN_mod_mul(z3, x1, z2, q, ctx);
			BN_mod_mul(z2, tmp1, tmp0, q, ctx);
		}
		if (swap)
		{
			std::swap (x2, x3);
			std::swap (z2, z3);
		}
		BN_mod_inverse (z2, z2, q, ctx);
		BIGNUM * res =  BN_new (); // not from ctx
		BN_mod_mul(res, x2, z2, q, ctx);
		BN_CTX_end (ctx);
		return res;
	}

	void ScalarMul (const uint8_t * p, const  uint8_t * e, uint8_t * buf, BN_CTX * ctx) 
	{
		BIGNUM * p1 = DecodeBN<32> (p);
		uint8_t k[32];
		memcpy (k, e, 32);
		k[0] &= 248; k[31] &= 127; k[31] |= 64;
		BIGNUM * n = DecodeBN<32> (k);
		BIGNUM * q1 = ScalarMul (p1, n, ctx);
		EncodeBN<32> (q1, buf );
		BN_free (p1); BN_free (n); BN_free (q1);
	}

	void ScalarMulB (const  uint8_t * e, uint8_t * buf, BN_CTX * ctx) 
	{
		BIGNUM *p1 = BN_new (); BN_set_word (p1, 9);
		uint8_t k[32];
		memcpy (k, e, 32);
		k[0] &= 248; k[31] &= 127; k[31] |= 64;
		BIGNUM * n = DecodeBN<32> (k);
		BIGNUM * q1 = ScalarMul (p1, n, ctx);
		EncodeBN<32> (q1, buf);
		BN_free (p1); BN_free (n); BN_free (q1);
	}
int main(void){

}
