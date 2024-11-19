#include"cuckoo_hash.h"
#include<random>
#include<iostream>

static const uint8_t SEED1=0x36;
static const uint8_t SEED2=0xf2;
cuckoo_hash::cuckoo_hash(int table_size,int kick_num)
{
    this->table.resize(table_size,"-1");
    this->kick_num=kick_num;
    this->table_size=table_size;
}

 void cuckoo_hash:: insert(mpz_t  value, int kick_num)
{
    auto &T=this->table;
    auto &S=this->stash;
    std::string v=mpz_get_str(NULL,16,value);
    if(kick_num<0)
    {
        S.push_back(v);
       std:: cout<<v<<"-->"<<"failure to insert"<<std::endl;
        return;
    }
    int n=this->table.size();
    int ind1= Hash(n,value,SEED1);
    int ind2=Hash(n,value,SEED2);
    
    if(T[ind1]=="-1") 
    {
        T[ind1]=v;
        //std::cout<<v<<"-->"<<"successfully insert in position: "<<ind1<<std::endl;
        return;
    }
    if(T[ind2]=="-1") 
    {
        T[ind2]=v;
        //std::cout<<v<<"-->"<<"successfully insert in position: "<<ind2<<std::endl;
        return;
    }
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(0, 1);
    if(dis(gen)==0)
    {
        auto kick= T[ind1];
        T[ind1]=v;
        mpz_t k;
        mpz_init(k);
        __gmpz_set_str(k,kick.c_str(),16);
        //std::cout<<v<<"-->kick "<<kick<<" and inserted into: "<<ind1<<std::endl;
        insert(k,--kick_num);
        
    }
    else
    {
        auto kick= T[ind2];
        T[ind2]=v;
         mpz_t k;
        mpz_init(k);
        __gmpz_set_str(k,kick.c_str(),16);
        //std::cout<<v<<"-->kick "<<kick <<" and inserted into: "<<ind1<<std::endl;
        insert(k,--kick_num);
    }
}
void cuckoo_hash::show_table()
{
    int i=0;
    for(auto v: this->table) {std::cout<<i<<":"<<v<<std::endl;++i;}
}

void cuckoo_hash::export_table(std::vector<std::string>& out_t)
{
    out_t.assign(this->table.begin(),this->table.end());
}
int cuckoo_hash::failuer_num()
{
    return this->stash.size();
}
std::vector<int> cuckoo_hash::simple_hash(std::string &value)
{
    mpz_t v;
    
    mpz_init_set_str(v,value.c_str(),16);
    int ind1= Hash(this->table_size,v,SEED1);
    int ind2=Hash(this->table_size,v,SEED2);
  
    std::vector<int> res{ind1,ind2};
    return res;
}