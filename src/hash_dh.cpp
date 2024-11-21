#include"hash_dh.h"

using namespace CryptoPP;
extern mpz_t p;

void powm(std::string &src_str,mpz_t exponent,mpz_t module)
{
    mpz_t tmp;
    mpz_init(tmp);
    mpz_set_str(tmp,src_str.c_str(),16);
    mpz_powm(tmp,tmp,exponent,module);
    src_str=mpz_get_str(NULL,16,tmp);
    mpz_clear(tmp);
}

std::string sha256(std::string &src_str)
{
    SHA256 sha256;
    std::string dst;
    StringSource(src_str, true, new HashFilter(sha256, new HexEncoder(new StringSink(dst))));
    lower(dst);
    return dst;
}
void alice_round(std::vector<std::string> &alice_id, mpz_t k1)
{
    for(auto &id : alice_id)
    {
        auto hashed_str=sha256(id);
        powm(hashed_str,k1,p);
        id=hashed_str;
    }
}
void bob_round(std::unordered_map<std::string,int> &bob_id_value,std::vector<std::string> &hash_aliceid_k1, mpz_t k2, std::vector<std::string> &hash_bobid_k2, 
                std::vector<std::string> &hom_cipher,Paillier pa)
{
    for(auto &id_value : bob_id_value)
    {
        std::string id= id_value.first;
        int value = id_value.second;
        std::string hash_str=sha256(id);
        powm(hash_str,k2,p);
        hash_bobid_k2.push_back(hash_str);
        mpz_t hom_cipher_v,value_v;
        mpz_inits(hom_cipher_v,value_v,nullptr);
        mpz_set_ui(value_v,value);
        pa.encrypt(hom_cipher_v,value_v);
        std::string t=mpz_get_str(NULL,16,hom_cipher_v);
        hom_cipher.push_back(t);
    }
    for(auto &id : hash_aliceid_k1)
    {
        powm(id,k2,p);
    }
}
void alice_round2(mpz_t k1,std::vector<std::string> &hash_aliceid_k1_k2, std::vector<std::string>& hash_bob_k2, std::vector<std::string> & homo_cipher,
                  std::string& enc_intersection_sum,Paillier pa)
{
    std::unordered_map<std::string,std::string>key_cipher;
    for(auto &id :hash_bob_k2)
    {
        powm(id,k1,p);
    }
    
    auto & hash_bob_k1_k2=hash_bob_k2;
    int intersection_cardinality=0;
    mpz_t res;
    mpz_init(res);
    for(int i=0;i<hash_bob_k1_k2.size();++i)
    {
        key_cipher[hash_bob_k1_k2[i]]=homo_cipher[i];
    }
    
    for(auto  & id :hash_aliceid_k1_k2)
    {
        if(key_cipher.find(id)!=key_cipher.end())
        {
            intersection_cardinality++;
            if(__gmpz_cmp_ui(res,0)==0)
               {
                 auto cipher=key_cipher[id];
                 mpz_set_str(res,cipher.c_str(),16);    
               }
            else
                {
                    auto cipher=key_cipher[id];
                    mpz_t cipher_v;
                    mpz_init(cipher_v);
                    mpz_set_str(cipher_v,cipher.c_str(),16);  
                    pa.homomorphic_add(res,cipher_v,res);
                }    
        }
    }  
    std::cout<<"intersection cardinality is: "<<intersection_cardinality<<std::endl;
    enc_intersection_sum=mpz_get_str(NULL,16,res);
}

void bob_output_intersection_sum(Paillier pa, std::string enc_intersection_sum, std::string & intersection_sum)
{
    mpz_t enc_intersection_sum_v,intersection_sum_v;
    mpz_inits(enc_intersection_sum_v,intersection_sum_v,nullptr);
    mpz_set_str(enc_intersection_sum_v,enc_intersection_sum.c_str(),16);
    pa.decrypt(intersection_sum_v,enc_intersection_sum_v);
    intersection_sum=mpz_get_str(NULL,10,intersection_sum_v);
}

