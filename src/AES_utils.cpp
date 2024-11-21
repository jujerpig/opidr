#include"AES_utils.h"
#include<iostream>
#include <iomanip>  // std::hex, std::setw, std::setfill
#include <sstream>
#include<algorithm>
#include<cctype>
#include"utils.h"
using namespace CryptoPP;


std:: string encrypt(std::string key, std::string pla)
{
    
    if(key.length()!=32)
        {
            std::cout<<"the key length is incorrect"<<std::endl;
            return "0";
        }
    ECB_Mode<AES>::Encryption e((byte *)key.c_str(), key.length());
    std::string cipher;
    StringSource(pla, true, new StreamTransformationFilter(e,new HexEncoder( new StringSink(cipher))));
    //for(unsigned char c: cipher)std::cout<<std::hex<<(int)c<<' ';
    //std::cout<<"encrypt "<<pla<<"-->"<<cipher<<std::endl;
    lower(cipher);
    auto hex_cipher=stringToHex(cipher);

    //std::cout<<"original cipher "<<hexToString(hex_cipher)<<std::endl;
    return cipher;
}

std::string decrypt(std::string key, std::string cip)
{
    if(key.length()!=32)
        {
            std::cout<<"the key length is incorrect"<<std::endl;
            return "0";
        }
    if(is_hex(cip))
    {
        cip=hexToString(cip);
    }
    try
    {
    ECB_Mode<AES>::Decryption decryption((byte *)key.c_str(), key.length());
    std::string recovered;
    StringSource(cip, true, new StreamTransformationFilter(decryption,new StringSink(recovered)));
    return recovered;
    }
    catch(const std::exception& e)
    {
        //std::cout<<"decrypt failure"<<std::endl;
        //std::cout<<e.what()<<std::endl;
        return "-1";
    }
    
   
}

std::string HashToKey(std::string & input)
{
    SHA256 sha256;
    std::string dst;
    StringSource(input, true, new HashFilter(sha256, new HexEncoder(new StringSink(dst))));
    lower(dst);
    auto str1=dst.substr(0,32);
    auto str2=dst.substr(32,32);
    std::stringstream ss;
    // for(int i=0;i<32;++i)
    // {
    //     int c1,c2;
    //     if(str1[i]>='0' && str1[i]<='9') c1=str1[i]-'0';
    //     else c1=str1[i]-'a'+10;
    //     if(str2[i]>='0' && str2[i]<='9') c2=str2[i]-'0';
    //     else c2=str2[i]-'a'+10;
    //     int c=c1^c2;
    //     ss<<std::setw(1)<<std::hex<<c;

    // }
   
   // return ss.str();
   return str1;
}
