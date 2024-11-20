#include<iostream>
#include"paillier.h"
using namespace std;
#include<vector>
#include <chrono>
#include"utils.h"
int main()
{
    Paillier p;
    mpz_t m1,m2;
    __gmpz_init_set_ui(m1,32);
    __gmpz_init_set_ui(m2,1);
    p.keygen(1024);
    vector<string>alice_id;
    unordered_map<string,int>bob_id_value;
    string alice_inp_path="example_input/ids.txt";
    string bob_inp_path="example_input/id_values.txt";
    get_input(alice_id,bob_id_value,alice_inp_path,bob_inp_path);
    
    auto start = std::chrono::high_resolution_clock::now();
    for(auto v: alice_id)
    {
        mpz_t pla,cipher;
        mpz_init(cipher);
        mpz_init(pla);
        mpz_set_str(pla,v.c_str(),16);
        p.encrypt(cipher,pla);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout<<"encryption time:"<<elapsed.count()<<"ms"<<std::endl;




}