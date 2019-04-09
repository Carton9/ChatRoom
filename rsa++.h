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
#define MSG_LEN 128
#define AES_BITS 128
using namespace std;
struct DataPkg{
    int size;
    char* cData;
};
class AuthKey{
    public:
        AuthKey(const string fileName);
        bool isavaliable();
        int loadKey(const string fileName);
        int encode(DataPkg* data);
        int decode(DataPkg* data);
        string encode(string data);
        string decode(string data);
        static string aes_encrypt(string in,string key,string initVector);
        static string aes_decrypt(string in,string key,string initVector);
        void clear();
        static string sha256(const string input);
        ~AuthKey();
    private:
        RSA* loadedKey;
        int state;
};
