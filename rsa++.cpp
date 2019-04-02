#include "rsa++.h"
AuthKey::AuthKey(const string fileName){
    state=loadKey(fileName);
}
AuthKey::~AuthKey(){
    clear();
    state=-1;
}
bool AuthKey::isavaliable(){
    if(state!=0)return false;
    return true;
}
int AuthKey::loadKey(const string fileName){
    if (fileName.empty()){
		return -1;  // error 1: empty string
	}
    FILE* keyFile = fopen(fileName.c_str(), "rb");
    if(keyFile == NULL){
        return -2; // error 2: can not open file
    }
    std::string strRet;
	loadedKey = RSA_new();
    if(PEM_read_RSA_PUBKEY(keyFile, &loadedKey, 0, 0) == NULL){
		return -3; // error 3: key file error
	}
    fclose(keyFile);
    return 0;
}
string AuthKey::encode(string data){
    DataPkg packet;
    packet.cData=data.data();
    packet.size=data.size();
    encode(&packet);
    string result(packet.size,packet.cData);
    return result;
}
int AuthKey::encode(DataPkg* data){
    int nLen = RSA_size(loadedKey);
	char* encodedData = new char[nLen + 1];
	int ret = RSA_public_encrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, loadedKey, RSA_PKCS1_PADDING);
    data->cData=encodedData;
    data->size=ret;
    CRYPTO_cleanup_all_ex_data();
}
string AuthKey::decode(string data){
    DataPkg packet;
    packet.cData=data.data();
    packet.size=data.size();
    decode(&packet);
    string result(packet.size,packet.cData);
    return result;
}
int AuthKey::decode(DataPkg* data){
    if(data==nullptr)
        return -1;
    int nLen = RSA_size(loadedKey);
	char* encodedData = new char[nLen + 1];
	int ret = RSA_private_decrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, loadedKey, RSA_PKCS1_PADDING);
    data->cData=encodedData;
    data->size=ret;
    CRYPTO_cleanup_all_ex_data();
    return 0;
}
void AuthKey::clear(){
    RSA_free(loadedKey);
}
static string AuthKey::sha256(const string input)
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


//TODO fix those code
int aes_encrypt(char* in, char* key, char* out)//, int olen)可能会设置buf长度
{
    if(!in || !key || !out) return 0;
    unsigned char iv[AES_BLOCK_SIZE];           //加密的初始化向量
    for(int i=0; i<AES_BLOCK_SIZE; ++i)         //iv一般设置为全0,可以设置其他，但是加密解密要一样就行
        iv[i]='0';                              // 注意字符和数字的区别，key也同样要注意
    AES_KEY aes;
    if(AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }
    int len=strlen(in);
    //这里的长度是char*in的长度，但是如果in中间包含'\0'字符的话，那么就只会加密前面'\0'前面的一段，所以，这个len可以作为参数传进来，记录in的长度，至于解密也是一个道理，光以'\0'来判断字符串长度，确有不妥，后面都是一个道理。

    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_ENCRYPT);
    return 1;
}

int aes_decrypt(char* in, char* key, char* out)
{
    if(!in || !key || !out) return 0;
    unsigned char iv[AES_BLOCK_SIZE];           //加密的初始化向量
    for(int i=0; i<AES_BLOCK_SIZE; ++i)         //iv一般设置为全0,可以设置其他，但是加密解密要一样就行
        iv[i]='0';
    AES_KEY aes;
    if(AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }
    int len=strlen(in);
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_DECRYPT);
    return 1;
}