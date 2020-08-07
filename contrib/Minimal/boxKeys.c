#include"boxKeys.h"
BoxKeys getKeyPair(void) {
  BoxKeys keys;
  size_t lenpub = KEYSIZE;
  size_t lenpriv = KEYSIZE;

  EVP_PKEY_CTX *Ctx;
  EVP_PKEY *Pkey = NULL;
  Ctx = EVP_PKEY_CTX_new_id(NID_X25519, NULL);

  EVP_PKEY_keygen_init(Ctx);
  EVP_PKEY_keygen(Ctx, &Pkey);

  EVP_PKEY_get_raw_public_key(Pkey, keys.PublicKey, &lenpub);
  EVP_PKEY_get_raw_private_key(Pkey, keys.PrivateKey, &lenpriv);

  EVP_PKEY_CTX_free(Ctx);
  EVP_PKEY_free(Pkey);

  return keys;
}
