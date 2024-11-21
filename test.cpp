#include<iostream>
#include"paillier.h"
using namespace std;
#include<vector>
#include <chrono>
#include"utils.h"
#include"hash_dh.h"
#include<chrono>
#include"AES_utils.h"
#include"cuckoo_hash.h"

//gmp_randstate_t state;
//mpz_t g,p;
int main()
{
    
    gmp_randinit_default(state);
  __gmp_randseed_ui(state, time(nullptr));
   Paillier pa;
   
   pa.keygen(1024);

    gen_generator(g,p);
    mpz_t k;
    gen_rand_element(k,p);
    auto start = std::chrono::high_resolution_clock::now();
   // mpz_powm(k,g,k,p);
    string t="23edf231";
    powm(t,k,p);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout<<"powm time:"<<elapsed.count()<<"ms"<<std::endl;
    
    string pla="hello world";
    string key="12312312311111111111111111111111";
    start = std::chrono::high_resolution_clock::now();
    encrypt(key,pla);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"aes encryption time:"<<elapsed.count()<<"ms"<<std::endl;

    start = std::chrono::high_resolution_clock::now();
    pa.encrypt(k,k);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"paliier encryption time:"<<elapsed.count()<<"ms"<<std::endl;

    cuckoo_hash cuc(2000,100);
    vector<mpz_t> data;
    for(int i=0;i<1000;++i)
    {
        mpz_t t;
        mpz_init_set_ui(t,i);
    }

    start = std::chrono::high_resolution_clock::now();
    for(auto i: data)
        cuc.insert(i,100);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"cuckoo hash insertion time:"<<elapsed.count()<<"ms"<<std::endl;

    start = std::chrono::high_resolution_clock::now();
    gen_rand(1024);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"gen_rand time:"<<elapsed.count()<<"ms"<<std::endl;

    mpz_t v;
    start = std::chrono::high_resolution_clock::now();
    gen_rand_element(v,p);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"gen_rand_element time:"<<elapsed.count()<<"ms"<<std::endl;
    cout<<"random v: ";
    gmp_printf("%Zd",v);
    cout<<endl;

    return 0;
    
}