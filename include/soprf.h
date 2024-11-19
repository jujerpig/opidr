#pragma once
#include<gmp.h>
#include<vector>
#include<unordered_map>
#include"cuckoo_hash.h"
void alice_round(std::vector<std::string>& alice_id, std::vector<std::string>& alice_enc_id,std::vector<std::string>& alice_half_key,mpz_t t);

void bob_round(std::unordered_map<std::string,int> bob_id_value,
std::vector<std::string>& alice_half_key,
std::vector<std::string>& alice_enc_id,
std::vector<std::string>& alice_key,
std::vector<std::string>& symmetric_cipher,
std::vector<std::string>& bob_key,
std::vector<std::string>& mac);

void construct_alice_key(mpz_t t, std::vector<std::string> &alice_key);

int alice_retrieva_data(mpz_t t, std::vector<std::string> &alice_key, std::vector<std::string>& symmetric_cipher,std::vector<std::string>& mac);
