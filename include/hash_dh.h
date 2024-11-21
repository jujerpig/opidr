#pragma once
#include"AES_utils.h"
#include"gmp.h"
#include<unordered_map>
#include"paillier.h"
#include"utils.h"
void powm(std::string &src_str,mpz_t exponent,mpz_t module);

std::string sha256(std::string &src_str);

void alice_round(std::vector<std::string> &alice_id, mpz_t k1);

void bob_round(std::unordered_map<std::string,int> &bob_id_value,std::vector<std::string> &hash_aliceid_k1, mpz_t k2, std::vector<std::string> &hash_bobid_k2, 
                std::vector<std::string> &hom_cipher,Paillier pa);

void alice_round2(mpz_t k1,std::vector<std::string> &hash_aliceid_k1_k2, std::vector<std::string>& hash_bob_k1_k2, std::vector<std::string> & homo_cipher,
                  std::string& enc_intersection_sum,Paillier pa);

void bob_output_intersection_sum(Paillier pa, std::string enc_intersection_sum, std::string & intersection_sum);

