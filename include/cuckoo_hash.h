#pragma once
#include "murmurhash.h"
#include<gmp.h>


class cuckoo_hash
{
    public:
    cuckoo_hash(int table_size,int kick_num);
    void insert(mpz_t  value,int kick_num);
    void show_table();
    void export_table(std::vector<std::string>& out_t);
    std::vector<int> simple_hash(std::string & value);
    int failuer_num();
    private:
    int kick_num;
    int table_size;
    int stash_size;
    std::vector<std::string> table;
    std::vector<std::string> stash;
};
