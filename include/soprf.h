#pragma once
#include<gmp.h>
#include<vector>
#include<unordered_map>
#include"cuckoo_hash.h"
void alice_round(std::vector<std::string>& alice_id, std::vector<std::string>& alice_enc_id,std::vector<std::string>& alice_half_key);

void bob_round(std::unordered_map<int,std::string> bob_id,
std::vector<std::string>& alice_half_key,
std::vector<std::string>& alice_enc_id,
std::vector<std::string>& alice_key,
std::vector<std::string>& symmetric_cipher,
std::vector<std::string>& bob_key);
