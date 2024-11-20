#pragma once
#include "gmp.h"
#include<string>
#include<vector>
#include<unordered_map>
void gen_prime( int LAMBDA, mpz_t p);

void gen_generator(mpz_t p, mpz_t g);

std::string gen_rand(int n);

void gen_parameters(mpz_t g, mpz_t p,int n);

void gen_rand_element(mpz_t v,mpz_t p);

std::string stringToHex(const std::string& input);

void lower(std::string &input);

bool is_hex(std::string & str);

std::string hexToString(std::string & input);

int gen_num_under_v(int v);

std::vector<int> gen_share(int v,int ShareNum);

std::string encode_id_to_hex(std::string id);

void get_input(std::vector<std::string> &alice_id, std::unordered_map<std::string,
int> &bob_id_value,std::string alice_inp_path="../alice_inp.txt",std::string bob_inp_path="../bob_inp.txt");