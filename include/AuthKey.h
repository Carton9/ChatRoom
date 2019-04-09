#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#define RSA_EN_LEN 117
#define RSA_DC_LEN 128
#define FAIL "c"

using namespace std;
struct DataPkg{
    int size;
    char* cData;
};
class AuthKey{
    public:

        AuthKey(const string fileName,bool isPub);
        bool isavaliable();
        int loadPubKey(const string fileName);
        int loadPriKey(const string fileName);
        int encode(DataPkg* data);
        int decode(DataPkg* data);
        string encode(string data);
        string decode(string data);
        void clear();
        static string sha512(const string input);
        string operator<<(const string str){return encode(str);}
        string operator>>(const string str){return decode(str);}
        ~AuthKey();
    private:
        RSA* loadedKey;
        int state;
        bool isPub;
};
