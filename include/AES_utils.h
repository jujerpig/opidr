#pragma once
#include "cryptlib.h"
#include <aes.h>
#include <filters.h>
#include <modes.h>
#include <hex.h>
#include <string>
#include<sha.h>
std:: string encrypt(std::string key, std::string pla);

std::string decrypt(std::string key, std::string cip);

std::string HashToKey(std::string & input);