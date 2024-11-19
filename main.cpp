#include<iostream>
#include<vector>
//#include"include/murmurhash.h"
#include<gmp.h>
//#include"cuckoo_hash.h"
#include"AES_utils.h"
#include"utils.h"
#include"soprf.h"
using namespace std;
using namespace CryptoPP;
mpz_t g;
mpz_t p;
int LAMBDA=2048;
int main()
{
    gen_prime(LAMBDA,p);
    gen_generator(p,g);
    vector<string>id;
    for(int i=100;i<=1000;++i)
    {
        id.push_back(to_string(i));
    }
    unordered_map<string,int> bob_id_value;
  
     for(int i=500;i<=1000;++i)
    {
       bob_id_value[to_string(i)]=i;
    }
    vector<string>alice_enc_id,alice_half_key,mac;
    mpz_t t;
    alice_round(id,alice_enc_id,alice_half_key, t);
    vector<string>bob_key,alice_key,symmtric_cipher;
    bob_round(bob_id_value,alice_half_key,alice_enc_id,alice_key,symmtric_cipher,bob_key,mac);
    //construct_alice_key(t,alice_key);
    int intersection=0;
    
    int intersection_sum=alice_retrieva_data(t,alice_key,symmtric_cipher,mac);
    cout<<"intersection sum is: "<<intersection_sum<<endl;
    return 0;
}