#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
 
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <openssl/sha.h>
using namespace std;
struct DataPkg{
    int size;
    char* cData;
}

int loadKey(const string& fileName, RSA* loadedKey);
int encode(DataPkg* data,RSA* loadedKey);
int decode(DataPkg* data,RSA* loadedKey);
void clear(RSA* loadedKey);
string sha256(const string input);