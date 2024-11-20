#ifndef PAILLIER_H
#define PAILLIER_H

#include <gmp.h>
#include <random>
#include <iostream>

class Paillier {
public:
    Paillier();
    ~Paillier();

    void keygen(unsigned int bits); // 生成密钥
    void encrypt(mpz_t& ciphertext, const mpz_t plaintext); // 加密
    void decrypt(mpz_t& plaintext, const mpz_t ciphertext); // 解密
    void homomorphic_add(mpz_t& result, const mpz_t ciphertext1, const mpz_t ciphertext2); // 同态加法

    void get_public_key(mpz_t& n, mpz_t& g); // 获取公钥
    void get_private_key(mpz_t& lambda, mpz_t& mu); // 获取私钥

private:
    mpz_t n, n2, g;    // 公钥部分：n, n^2, g
    mpz_t lambda, mu;  // 私钥部分：λ, μ

    void l_function(mpz_t& result, const mpz_t x); // L 函数
};

#endif // PAILLIER_H