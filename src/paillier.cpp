#include "Paillier.h"

// 构造函数
Paillier::Paillier() {
    mpz_inits(n, n2, g, lambda, mu, nullptr);
}

// 析构函数
Paillier::~Paillier() {
    //mpz_clears(n, n2, g, lambda, mu);
   // mpz_clear(n);

}

// L(x) = (x - 1) / n
void Paillier::l_function(mpz_t& result, const mpz_t x) {
    mpz_sub_ui(result, x, 1);
    mpz_div(result, result, n);
}

// 生成密钥
void Paillier::keygen(unsigned int bits) {
    mpz_t p, q, phi, temp;

    mpz_inits(p, q, phi, temp, nullptr);

    // 随机生成两个大素数 p 和 q
    gmp_randstate_t state_pa;
    gmp_randinit_default(state_pa);
    gmp_randseed_ui(state_pa, time(nullptr));

    mpz_urandomb(p, state_pa, bits / 2);
    mpz_nextprime(p, p);

    mpz_urandomb(q, state_pa, bits / 2);
    mpz_nextprime(q, q);

    // 计算 n = p * q
    mpz_mul(n, p, q);

    // 计算 n^2
    mpz_mul(n2, n, n);

    // 设置 g = n + 1
    mpz_add_ui(g, n, 1);

    // 计算 λ = lcm(p-1, q-1)
    mpz_sub_ui(temp, p, 1);
    mpz_sub_ui(phi, q, 1);
    mpz_lcm(lambda, temp, phi);

    // 计算 μ = (L(g^λ mod n^2))^-1 mod n
    mpz_powm(temp, g, lambda, n2); // temp = g^λ mod n^2
    l_function(mu, temp);         // mu = L(temp)
    mpz_invert(mu, mu, n);        // μ = μ^-1 mod n

    // 清理
    mpz_clears(p, q, phi, temp, nullptr);
}

// 加密
void Paillier::encrypt(mpz_t& ciphertext, const mpz_t plaintext) {
    mpz_t r, rn, temp;

    mpz_inits(r, rn, temp, nullptr);

    // 生成随机数 r ∈ [1, n)
    gmp_randstate_t state_pa;
    gmp_randinit_default(state_pa);
    gmp_randseed_ui(state_pa, time(nullptr));

    do {
        mpz_urandomm(r, state_pa, n);
    } while (mpz_cmp_ui(r, 0) == 0);

    // 计算密文 c = g^m * r^n mod n^2
    mpz_powm(temp, g, plaintext, n2); // temp = g^m mod n^2
    mpz_powm(rn, r, n, n2);           // rn = r^n mod n^2
    mpz_mul(ciphertext, temp, rn);    // c = g^m * r^n
    mpz_mod(ciphertext, ciphertext, n2);

    mpz_clears(r, rn, temp, nullptr);
}

// 解密
void Paillier::decrypt(mpz_t& plaintext, const mpz_t ciphertext) {
    mpz_t temp;

    mpz_init(temp);

    // 计算 m = L(c^λ mod n^2) * μ mod n
    mpz_powm(temp, ciphertext, lambda, n2); // temp = c^λ mod n^2
    l_function(plaintext, temp);           // plaintext = L(temp)
    mpz_mul(plaintext, plaintext, mu);     // plaintext = L(temp) * μ
    mpz_mod(plaintext, plaintext, n);      // plaintext = plaintext mod n
    mpz_clear(temp);
}

// 同态加法
void Paillier::homomorphic_add(mpz_t& result, const mpz_t ciphertext1, const mpz_t ciphertext2) {
    mpz_mul(result, ciphertext1, ciphertext2); // c1 * c2 mod n^2
    mpz_mod(result, result, n2);
}

// 获取公钥
void Paillier::get_public_key(mpz_t& out_n, mpz_t& out_g) {
    mpz_set(out_n, n);
    mpz_set(out_g, g);
}

// 获取私钥
void Paillier::get_private_key(mpz_t& out_lambda, mpz_t& out_mu) {
    mpz_set(out_lambda, lambda);
    mpz_set(out_mu, mu);
}