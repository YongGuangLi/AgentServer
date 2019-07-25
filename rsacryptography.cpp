#include "rsacryptography.h"

RSACryptography::RSACryptography()
{
}

int RSACryptography::RSA_Encrypt(const char *plainText, char *&cipherText, const char *publicKeyPath)
{
    RSA *p_rsa;
    FILE *file;

    if ((file = fopen(publicKeyPath, "r")) == NULL) {
        perror("open key file error");
        return NULL;
    }
    if ((p_rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }

    int rsa_len = RSA_size(p_rsa);
    cipherText = (char *) malloc(rsa_len + 1);
    memset(cipherText, 0, rsa_len + 1);

    if (RSA_public_encrypt(rsa_len, (unsigned char *) plainText, (unsigned char*) cipherText, p_rsa, RSA_NO_PADDING) < 0)
        return NULL;

    RSA_free(p_rsa);
    fclose(file);

    return rsa_len;
}

int RSACryptography::RSA_Decrypt(const char *cipherText, char *&plainText, const char *privateKeyPath)
{
    FILE *file = NULL;
    if ((file = fopen(privateKeyPath, "r")) == NULL) {
        perror("open key file error");
        return NULL;
    }

    RSA *p_rsa = NULL;
    if ((p_rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    int rsa_len = RSA_size(p_rsa);
    plainText = (char *) malloc(rsa_len + 1);
    memset(plainText, 0, rsa_len + 1);

    if (RSA_private_decrypt(rsa_len, (unsigned char *) cipherText, (unsigned char*) plainText, p_rsa, RSA_NO_PADDING) < 0)
        return NULL;

    RSA_free(p_rsa);
    fclose(file);
    return rsa_len;
}
