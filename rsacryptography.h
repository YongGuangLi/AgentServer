#ifndef RSACRYPTOGRAPHY_H
#define RSACRYPTOGRAPHY_H


#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

#define OPENSSLKEY "rsa_private.key"
#define PUBLICKEY "rsa_public.key"

class RSACryptography
{
public:
    RSACryptography();
    int RSA_Encrypt(const char *plainText, char *&cipherText, const char *publicKeyPath);
    int RSA_Decrypt(const char *cipherText, char *&plainText, const char *privateKeyPath);

};

#endif // RSACRYPTOGRAPHY_H
