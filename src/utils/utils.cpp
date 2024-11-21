#include"utils.h"
#include <ctime>       
#include <random>
#include<algorithm>
#include<cctype>
#include <iomanip>  // std::hex, std::setw, std::setfill
#include <sstream>
#include <fstream>
#include<iostream>

extern gmp_randstate_t state;
  

void gen_prime( int LAMBDA, mpz_t p)
{
  mpz_init(p);
  while(1){
    mpz_urandomb(p,state,LAMBDA);
    if(mpz_probab_prime_p(p,10) != 0)return;
  }
  return;
}

void gen_generator(mpz_t p, mpz_t g)
{
const char* str = "1";
  const char* str2 = "2";
  mpz_t a,b,c;
  mpz_init(a);
  mpz_init(b);
  mpz_init(c);
  mpz_init_set_str(b,str,10);// b = 1; b作为累加的值
  mpz_init_set_str(a,str2,10);// a = 2;相当于累加器
  mpz_init_set_str(c,str2,10);// c = a;用于计算
  const int tempNum = 100;
  mpz_t temp[tempNum];
  for(int i = 0 ; i < tempNum ; i++){
    mpz_init(temp[i]);
  }
  int i,tempLength;//tempLength = 0;// temp数组中有意义的值的数量
  while(1){
    // 判断是否是本原根，如果是则返回，否则判断下一个数
    mpz_init_set(temp[0],a);
    mpz_init_set(c,a);
    for(i = 0 , tempLength = 1 ; i < tempNum ; ){
      mpz_mul(c,a,c);
      for(i = 0 ; i < tempLength ; i++){
        if(mpz_cmp(temp[i],c) ==0){
          break;
        }
      }
      if(i == tempLength){
        mpz_init_set(temp[tempLength++],c);
      }else {
        for(int k = 0 ; k < tempLength ; k++){
          mpz_init(temp[k]);
        }
        break;
      }
      if(tempLength == tempNum){
        mpz_init_set(g,c);
        return;
      }
    }
    mpz_add(a,a,b);
  }
}
std::string gen_rand(int n)
{
    mpz_t res;
    mpz_init(res);   
    mpz_urandomb(res, state, n); 
    std::string res_str;
    res_str=mpz_get_str(NULL,16,res);
    return res_str;
}
void gen_rand_element(mpz_t v,mpz_t p)
{

    mpz_init(v);
    mpz_urandomm(v,state,p);
}
void gen_parameters(mpz_t g, mpz_t p,int n)
{
   mpz_t q;
   mpz_init(q);
   mpz_init(p);
   mpz_init(g);//全局参数：p为prime number;a为 primitive root of p;
     mpz_urandomb(q,state,n);//生成128bit的随机数
    mpz_nextprime(q,q);//生成素数q
    //p=2*q+1,构造一个素数p，使得p-1的因子只有1,2,q，p-1四个
    mpz_mul_ui(p,q,2);
    mpz_add_ui(p,p,1);
    //求生成元，1到p-1之间的随机数a不是单位元、阶不为2和q，
    //则其阶为p-1，进而证明得a为p的生成元
    mpz_t a_2,a_q;
    mpz_init(a_2);
    mpz_init(a_q);
    do{
        mpz_urandomm(g,state,p);//生成1到p-1的随机数
        mpz_powm_ui(a_2,g,2,p);//a_2=a^2 % p
        mpz_powm(a_q,g,q,p);//a_q=a^q % p
        if((mpz_cmp_ui(g,1)!=0)&&(mpz_cmp_ui(a_2,1)!=0)&&(mpz_cmp_ui(a_q,1)!=0))
        break;
    }while(1);
}

std::string stringToHex(const std::string& input) {
    std::stringstream ss;
    for (unsigned char c : input) {
        // 将每个字符转换为 16 进制，格式化为 2 位并填充
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)c;
    }
    return ss.str();
}

void lower(std::string &input)
{
    for(auto &c : input)
    {
        if(c<='Z'&&c>='A') c+=32;
    }
}

bool is_hex(std::string & str)
{
return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isxdigit(c); // 判断是否是16进制字符（0-9, a-f, A-F）
    });
}
std::string hexToString(std::string & input)
{
    std::string res="";
    if(input.length()%2!=0)input='0'+input;
    for(int i=0;i<input.length();i+=2)
    {
        std::string byte_str=input.substr(i,2);
         res+=(unsigned char)(strtol(byte_str.c_str(),nullptr,16));
         //res.push_back(byte);
    }
    return res;
}

int gen_num_under_v(int v)
{
    if(v<=1) return v;
    std::random_device rd;
    std::mt19937 gen(rd());  
    std::uniform_int_distribution<> dis(1, v-1); 
    return dis(gen);  
}

std::vector<int> gen_share(int v,int ShareNum)
{
    int share_num=gen_num_under_v(ShareNum);
    //std::cout<<"share nums:"<<share_num<<std::endl;
    std::vector<int>shares;
    for(int i=share_num;i>1;--i)
    {
       int s=gen_num_under_v(v-i);
       shares.push_back(s);
       v-=s;
    }
    shares.push_back(v);
    return shares;
}

void get_input(std::vector<std::string> &alice_id, std::unordered_map<std::string,int> &bob_id_value,std::string alice_inp_path,std::string bob_inp_path)
{
    std::ifstream ids_file(alice_inp_path);
    if (!ids_file) {
        std::cout<<"The alice input path doesn't exist."<<std::endl;
        return ;
    }

    std::cout << "Reading input data from "<<alice_inp_path<<" and "<<bob_inp_path << std::endl;
    std::string id;
    while (std::getline(ids_file, id)) {
        alice_id.push_back(stringToHex(id));
    }

    // 读取 id_values.txt
    std::ifstream id_values_file(bob_inp_path);
    if (!id_values_file) {
        std::cout<<"The bob input path doesn't exist."<<std::endl;
        return ;
    }

    std::string line;
    while (std::getline(id_values_file, line)) {
        std::istringstream iss(line);
        std::string id;
        int value;

        if (std::getline(iss, id, ':') && (iss >> value)) {
            bob_id_value[stringToHex(id)] = value;      
        }
    }

}