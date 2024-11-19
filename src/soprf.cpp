#include"soprf.h"
#include"utils.h"
#include<iostream>
extern int LAMBDA;
int EPSILON=2;
extern mpz_t g;
extern mpz_t p;

void alice_round(std::vector<std::string>& alice_id, std::vector<std::string>& alice_enc_id,std::vector<std::string>& alice_half_key)
{
   
    mpz_t t;
    gen_rand_element(t,p);
    int n=alice_id.size();
    cuckoo_hash cuc(EPSILON * n,100);
    for(auto id:alice_id)
        {
            mpz_t v;
            mpz_init(v);
            mpz_set_str(v,id.c_str(),16);
            cuc.insert(v,100);
        }
    cuc.export_table(alice_enc_id);
    for(auto &id:alice_enc_id)
    {
        if(id=="-1")
        {
            id=gen_rand(LAMBDA);
            alice_half_key.push_back(id);
        }
        else
        {
            mpz_t v,r;
            mpz_init(v);
            mpz_init(r);
            mpz_set_str(v,id.c_str(),16); 
            gen_rand_element(r,p);
            mpz_mul(v,v,r);
            mpz_powm(v,g,v,p);
            id=mpz_get_str(NULL,16,v);
            mpz_mul(r,r,t);
            mpz_powm(r,g,r,p);
            std::string half_key=mpz_get_str(NULL,16,r);
            alice_half_key.push_back(half_key);
        }
    }
}

void bob_round(std::unordered_map<int,std::string> bob_id,
std::vector<std::string>& alice_half_key,
std::vector<std::string>& alice_enc_id,
std::vector<std::string>& alice_key,
std::vector<std::string>& symmetric_cipher,
std::vector<std::string>& bob_key)
{
    return;
}
