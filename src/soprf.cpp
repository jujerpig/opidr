#include"soprf.h"
#include"utils.h"
#include<iostream>
#include"AES_utils.h"
extern int LAMBDA;
int EPSILON=2;
extern mpz_t g;
extern mpz_t p;
int SHARENUM=3;
void alice_round(std::vector<std::string>& alice_id, std::vector<std::string>& alice_enc_id,std::vector<std::string>& alice_half_key,mpz_t t)
{
   
    //mpz_t t;
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



void bob_round(std::unordered_map<std::string,int> bob_id_value,
std::vector<std::string>& alice_half_key,
std::vector<std::string>& alice_enc_id,
std::vector<std::string>& alice_key,
std::vector<std::string>& symmetric_cipher,
std::vector<std::string>& bob_key,
std::vector<std::string>& mac)
{
    if((alice_enc_id.size()!=alice_half_key.size()))
    {
        std::cout<<"there is something wrong in the alice round: alice key size is unequal to alice enc id size"<<std::endl;
        return;
    }
    int n=alice_enc_id.size();
    cuckoo_hash cuc(n,100);
    
    for(auto &id_value : bob_id_value)
    {
        std::string id=id_value.first;
        int value=id_value.second;
        std::vector<int>shares=gen_share(value,SHARENUM);
        mpz_t bob_id_v;
        mpz_init(bob_id_v);
        std::vector<int> ind=cuc.simple_hash(id);
        for(auto i: ind)
        {
            std::string& alice_hk=alice_half_key[i];
            std::string& alice_ei=alice_enc_id[i];
            mpz_t alice_hk_v,alice_ei_v;
            mpz_init(alice_hk_v);
            mpz_init(alice_ei_v);
            mpz_set_str(alice_hk_v,alice_hk.c_str(),16);
            mpz_set_str(alice_ei_v,alice_ei.c_str(),16);
            for(int s : shares)
            {    
                mpz_t u;
                gen_rand_element(u,p);
                mpz_t bob_key_v, alice_key_v;
                mpz_init(bob_key_v);
                mpz_init(alice_key_v);
                mpz_set_str(bob_id_v,id.c_str(),16);
                mpz_mul(bob_id_v,bob_id_v,u);
                mpz_powm(bob_key_v,alice_hk_v,bob_id_v,p);
                mpz_powm(alice_key_v,alice_ei_v,u,p);
                std::string bob_key_str=mpz_get_str(NULL,16,bob_key_v);
                std::string alice_key_str=mpz_get_str(NULL,16,alice_key_v);
                bob_key.push_back(bob_key_str);
                alice_key.push_back(alice_key_str);
                std::string key=HashToKey(bob_key_str);
                std::string cipher=encrypt(key,std::to_string(s));
                symmetric_cipher.push_back(cipher);
                auto mac_str=std::to_string(s);
                mac.push_back(HashToKey(mac_str));
            }    
        }

    }   
}
void construct_alice_key(mpz_t t, std::vector<std::string> &alice_key)
{
    for(auto &key:alice_key)
    {
        mpz_t key_v;
        mpz_init_set_str(key_v,key.c_str(),16);
        mpz_powm(key_v,key_v,t,p);
        key=mpz_get_str(NULL,16,key_v);
    }
}

int dec_str_to_int(std::string inp)
{
    int res=0;
    for(char c :inp)
    {
        res*=10;
        res+=c-'0';
    }
    return res;
}
int alice_retrieva_data(mpz_t t, std::vector<std::string> &alice_key, std::vector<std::string>& symmetric_cipher,std::vector<std::string>& mac)
{   
    int n=alice_key.size();
    if(n!=symmetric_cipher.size())
    {
        std::cout<<"key size is unequal to ciphertexts size; please check"<<::std::endl;
        return -1;
    }
    construct_alice_key(t,alice_key);
    int intersection_sum=0;
    try{
    for(int i=0;i<n;++i)
    {
        std::string key=HashToKey(alice_key[i]);
        std::string rec_data=decrypt(key,symmetric_cipher[i]);
   
        if(rec_data=="-1")
            continue;
         else if(HashToKey(rec_data)!=mac[i])
            {std::cout<<"garbage data"<<std::endl; 
             continue;}
        intersection_sum+=dec_str_to_int(rec_data);
    }
    }
    catch (const std::exception& e)
    {
        std::cout<<"garbage data"<<std::endl;
        std::cout<< e.what()<<std::endl;

    }
    return intersection_sum;
}
