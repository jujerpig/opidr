#include"protocol.h"
#include"mix_protoco.h"
#include<chrono>
#include<fstream>

extern mpz_t p,g;
extern gmp_randstate_t state;

void mix_protocol(std::vector<std::string>id,std::unordered_map<std::string,int> bob_id_value)
{
    gmp_randinit_default(state);
     __gmp_randseed_ui(state, time(nullptr));
    std::cout<<"Generating parameter..."<<std::endl;
    gen_prime(1024,p);
    std::vector<std::string>alice_enc_id,mac;
    mpz_t k1;
    gen_rand_element(k1,p);
    std::cout<<"Alice: generate half keys and enc her ids..."<<std::endl;
    auto start_whole = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::high_resolution_clock::now();
    mix_alice_round(id,alice_enc_id, k1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout<<"Alice round1 time:"<<elapsed.count()<<"ms"<<std::endl;

    std::vector<std::string>bob_key,alice_key,symmtric_cipher;
    std::cout<<"Bob: generate symmetrci keys and ciphertexts"<<std::endl;

    start = std::chrono::high_resolution_clock::now();
    mix_bob_round(bob_id_value,alice_enc_id,symmtric_cipher,alice_key,bob_key,mac);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"Bob round2 time:"<<elapsed.count()<<"ms"<<std::endl;
    int intersection=0;
    std::cout<<"Alice: retrieve data corresponding to the intersection"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    int intersection_sum=mix_alice_retrieva_data(k1,alice_key,symmtric_cipher,mac);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"Alice retrieve data time:"<<elapsed.count()<<"ms"<<std::endl;
    std::cout<<"intersection sum is: "<<intersection_sum<<std::endl;
    auto end_whole = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> excution_time = end_whole - start_whole;
    std::cout<<"Excution time: "<<excution_time.count()<<"ms"<<std::endl;

    std::ofstream ofs;
    ofs.open("results.txt",std::ios::app);
    ofs<<"mix_protocol: input cardinality-->"<<id.size()<<" excution time: "<<excution_time.count()<<std::endl;
}