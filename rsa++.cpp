#include "rsa++.h"

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
string sha256(const string input)
{
	char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.size());
    SHA256_Final(hash, &sha256);
    std::string NewString = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(buf,"%02x",hash[i]);
        NewString = NewString + buf;
    }
	return NewString;
}
