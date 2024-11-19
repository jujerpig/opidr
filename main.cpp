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
int LAMBDA=1024;
int main()
{
    gen_prime(LAMBDA,p);
    gen_generator(p,g);
    vector<string>id;
    for(int i=1;i<=4;++i)
    {
        id.push_back(to_string(i));
    }
    
    vector<string>alice_enc_id,alice_half_key;

    alice_round(id,alice_enc_id,alice_half_key);
    cout<<"alice enc id:"<<endl;
    for(auto i:alice_enc_id) cout<<i<<endl;
    cout<<"alice half key:"<<endl;
    for(auto i:alice_half_key) cout<<i<<endl;

    return 0;
}