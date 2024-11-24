#include"protocol.h"
#include<chrono>
#include<fstream>

extern mpz_t g,p;
extern gmp_randstate_t state;
void pi_sum_protocol(std::vector<std::string>alice_id,std::unordered_map<std::string,int> bob_id_value)
{
   std::ofstream ofs;

   Paillier pa;
    gmp_randinit_default(state);
     __gmp_randseed_ui(state, time(nullptr));
    auto start_whole = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::high_resolution_clock::now();
   std::cout<<"Bob: generate homomorphic encryption key..."<<std::endl;
   pa.keygen(1024);
   gen_prime(1024,p);
   mpz_t k1,k2;
   std::cout<<"Alice & Bob: generate secret components..."<<std::endl;
   gen_rand_element(k1,p);
   gen_rand_element(k2,p);
   std::cout<<"Alice: encrypt her ids using k1..."<<std::endl;
   alice_round(alice_id,k1);

   std::vector<std::string>hash_bob_k2,hom_cipher;
   auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout<<"Alice round1 time:"<<elapsed.count()<<"ms"<<std::endl;
   std::cout<<"Bob: encrypt his ids  and values using k2 and homomorphic encryption schene..."<<std::endl;
   start=std::chrono::high_resolution_clock::now();
   bob_round(bob_id_value,alice_id,k2,hash_bob_k2,hom_cipher,pa);
 
   end = std::chrono::high_resolution_clock::now();
   elapsed = end - start;
    std::cout<<"Bob round1 time:"<<elapsed.count()<<"ms"<<std::endl;
   std::string enc_intersection_sum;
   std::cout<<"Alice: find intersection and output intersection cardinality.."<<std::endl;
    start=std::chrono::high_resolution_clock::now();
   alice_round2(k1,alice_id,hash_bob_k2,hom_cipher,enc_intersection_sum,pa);  
   end = std::chrono::high_resolution_clock::now();
   elapsed = end - start;
    std::cout<<"Alice round2 time:"<<elapsed.count()<<"ms"<<std::endl;
   std::string intersection_sum;
   std::cout<<"Bob: decrpt the enc_intersection_sum and output intersection sum..."<<std::endl;
   start=std::chrono::high_resolution_clock::now();
   bob_output_intersection_sum(pa,enc_intersection_sum,intersection_sum);
   std::cout<<"intersection sum is: "<<intersection_sum<<std::endl;
   end = std::chrono::high_resolution_clock::now();
   elapsed = end - start;
   std::cout<<"Bob round2 time:"<<elapsed.count()<<"ms"<<std::endl;
   auto enc_whole = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double, std::milli> excution_time = enc_whole - start_whole;
   std::cout<<"Excution time: "<<excution_time.count()<<"ms"<<std::endl;
   
}