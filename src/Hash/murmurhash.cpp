#include "../../include/murmurhash.h"
#include <vector>


/**
 * MurmurHash3 was written by Austin Appleby, and is placed in the public
 * domain.
 * 
 * Adapted from code found at https://github.com/PeterScott/murmur3
 * 
 * @param input
 *    A std::vector of unsigned bytes that will be checked
 * @return 
 *    The 32bit Murmur3 of the input
 */
uint32_t MurMurHash::MurMur3_32(std::vector<uint8_t>& input) {
   if (input.empty()) {
      return 0;
   }
   
   const int nBlocks = input.size() / 4;
   const uint32_t* blocks = reinterpret_cast<const uint32_t *>(&input[0]);
   const uint8_t* tail = &input[nBlocks*4];
   
   uint32_t hash = 0;

   uint32_t k;
   for (int i = 0; i < nBlocks ; ++i) {
      k = blocks[i];
      
      k *= c1;
      k = rotl32(k,15);
      k *= c2;
      
      hash ^= k;
      hash = rotl32(hash,13);
      hash = (hash * m) + n;
   }
   
   k = 0;
   switch (input.size() & 3) {
      case 3: 
         k ^= tail[2] << 16;
      case 2: // intentionally inclusive of above
         k ^= tail[1] << 8;
      case 1: // intentionally inclusive of above
         k ^= tail[0];
         k *= c1;
         k = rotl32(k,15);
         k *= c2;
         hash ^= k;
   }
   
   hash ^= input.size();
   hash ^= hash >> 16;
   hash *= final1;
   hash ^= hash >> 13;
   hash *= final2;
   hash ^= hash  >> 16;
           
   return hash;
}

uint32_t Hash(int n, mpz_t input,uint8_t seed)//seed uint8_t
{
    MurMurHash h;
    std::vector<uint8_t> t;
    auto cstr=mpz_get_str(NULL,16,input);
    std:: string s=cstr;
    if(s.length()%2!=0) s='0'+s;
    for (size_t i = 0; i < s.length(); i += 2) {
        // 从字符串中获取每一对字符
        std::string byteString = s.substr(i, 2);
        // 将16进制字符串转换为数字
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
        t.push_back(byte);
    }
    t.push_back(seed);
    auto res =h.MurMur3_32(t);
    return res %n;
}
