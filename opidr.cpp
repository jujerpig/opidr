#include"protocol.h"
#include"laplace_noise.h"
#include <chrono>
#include<fstream>
extern mpz_t p,g;
extern gmp_randstate_t state;
int LAMBDA=1024;

void opidr_protocol(std::vector<std::string>id,std::unordered_map<std::string,int> bob_id_value)
{
    // std::cout<<"add noise to bob's value..."<<std::endl;
    // for(auto & id_value : bob_id_value)
    // {
    //     int value=id_value.second;
    //     value=addDifferentialPrivacy(value);
    //     id_value.second=value;
    //    // std::cout<<id_value.second<<std::endl;
    // }
    
    gmp_randinit_default(state);
     __gmp_randseed_ui(state, time(nullptr));
    std::cout<<"Generating parameter..."<<std::endl;
    gen_prime(LAMBDA,p);
    gen_generator(p,g);
    std::vector<std::string>alice_enc_id,alice_half_key,mac;
    mpz_t t;
    std::cout<<"Alice: generate half keys and enc her ids..."<<std::endl;
    auto start_whole = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::high_resolution_clock::now();
    std::ofstream ofs;
    

    alice_round(id,alice_enc_id,alice_half_key, t);
   
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout<<"Alice round1 time:"<<elapsed.count()<<"ms"<<std::endl;

    std::vector<std::string>bob_key,alice_key,symmtric_cipher;
    std::cout<<"Bob: generate symmetrci keys and ciphertexts"<<std::endl;

    start = std::chrono::high_resolution_clock::now();
    bob_round(bob_id_value,alice_half_key,alice_enc_id,alice_key,symmtric_cipher,bob_key,mac);

    ofs.open("opidr_bob_round.txt",std::ios::app);
   
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"Bob round2 time:"<<elapsed.count()<<"ms"<<std::endl;

    int intersection=0;
    std::cout<<"Alice: retrieve data corresponding to the intersection"<<std::endl;

    start = std::chrono::high_resolution_clock::now();
    int intersection_sum=alice_retrieva_data(t,alice_key,symmtric_cipher,mac);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"Alice retrieve data time:"<<elapsed.count()<<"ms"<<std::endl;

    std::cout<<"intersection sum is: "<<intersection_sum<<std::endl;
    auto end_whole = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> excution_time = end_whole - start_whole;
    std::cout<<"Excution time: "<<excution_time.count()<<"ms"<<std::endl;
   
}