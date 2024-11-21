
#include"protocol.h"
#include<map>
#include <functional>
using namespace std;

std::map<std::string, std::function<void(std::vector<std::string>,std::unordered_map<std::string,int>)>> commandMap={
{"opidr",opidr_protocol},
{"pi_sum",pi_sum_protocol}
};

mpz_t p,g;
gmp_randstate_t state;

int main(int argc, char* argv[])
{   
    if(argc<4)
    {
        cout<<"The cmd is wrong."<<endl
        <<"Please select a function:"<<endl
        <<"opidr <alice input path> <bob input path>";
        return 0;
    }
    string func,alice_inp_path,bob_inp_path;
    func=argv[1];
    alice_inp_path=argv[2];
    bob_inp_path=argv[3];
    std::vector<std::string>alice_id;
    std::unordered_map<std::string,int> bob_id_value;
    get_input(alice_id,bob_id_value,alice_inp_path,bob_inp_path);
    auto it = commandMap.find(func);
    if (it != commandMap.end()) {
        cout<<"executing "<< func<<"..."<<endl;
        it->second(alice_id,bob_id_value);
    } else {
        cout << "the function doesn't exist: " << std::endl;
        return 1;
    }
    return 0;
}