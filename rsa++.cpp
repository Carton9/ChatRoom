#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
 
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
using namespace std;
struct DataPkg{
    int size;
    char* cData;
}

int loadKey(const string& fileName, RSA* loadedKey){
    if (fileName.empty()){
		return -1;  // error 1: empty string
	}
    FILE* keyFile = fopen(fileName.c_str(), "rb");
    if(keyFile == NULL){
        return -2; // error 2: can not open file
    }
    std::string strRet;
	RSA* pRSAPublicKey = RSA_new();
    if(PEM_read_RSA_PUBKEY(keyFile, &loadedKey, 0, 0) == NULL){
		return -3; // error 3: key file error
	}
    fclose(keyFile);
}
int encode(DataPkg* data,RSA* loadedKey){
    int nLen = RSA_size(loadedKey);
	char* encodedData = new char[nLen + 1];
	int ret = RSA_public_encrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, pRSAPublicKey, RSA_PKCS1_PADDING);
    data->cData=encodedData;
    data->size=ret;
    CRYPTO_cleanup_all_ex_data();
}
int decode(DataPkg* data,RSA* loadedKey){
    int nLen = RSA_size(loadedKey);
	char* encodedData = new char[nLen + 1];
	int ret = RSA_private_decrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, pRSAPublicKey, RSA_PKCS1_PADDING);
    data->cData=encodedData;
    data->size=ret;
    CRYPTO_cleanup_all_ex_data();
}
void clear(RSA* loadedKey){
    RSA_free(pRSAPriKey);
}