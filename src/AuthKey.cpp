#include "AuthKey.h"
//using namespace std;

AuthKey::AuthKey(const string fileName,bool isPub){
    if(isPub){
         state=loadPubKey(fileName);
    }else{
        state=loadPriKey(fileName);
    }
    this->isPub=isPub;
    cout << state << endl;
}
AuthKey::~AuthKey(){
    clear();
    state=-1;
}
bool AuthKey::isavaliable(){
    if(state!=0)return false;
    return true;
}
int AuthKey::loadPubKey(const string fileName){
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
		return -3; // error 3: key file
	}
    fclose(keyFile);
    return 0;
}
int AuthKey::loadPriKey(const string fileName){
    if (fileName.empty()){
		return -1;  // error 1: empty string
	}
    FILE* keyFile = fopen(fileName.c_str(), "rb");
    if(keyFile == NULL){
        return -2; // error 2: can not open file
    }
    std::string strRet;
	loadedKey = RSA_new();
    if(PEM_read_RSAPrivateKey(keyFile, &loadedKey, 0, 0) == NULL){
		return -3; // error 3: key file
	}
    fclose(keyFile);
    return 0;
}
string AuthKey::encode(string data){
    /*DataPkg packet;
    packet.cData=(char*)data.data();
    packet.size=data.size();
    encode(&packet);
    string result(packet.cData,packet.size);
    return result;
    */
    char tmp[RSA_EN_LEN];
    char* ptr=tmp;
    DataPkg packet;
    int pointer=0;
    string result="";
    for(int i=0;i<data.size();i++){

        if(i!=0&&i%RSA_EN_LEN==0){
            packet.cData=tmp;
            packet.size=RSA_EN_LEN;
            encode(&packet);
            if(packet.size>0){
                string newResult(packet.cData,packet.size);
                result+=newResult;
            }
            else
                return FAIL;
            ptr=tmp;
            pointer++;
        }
        *ptr=data[i];
        ptr++;
    }
    packet.cData=tmp;
    packet.size=data.size()-RSA_EN_LEN*pointer;
    encode(&packet);
    if(packet.size>0){
        string newResult(packet.cData,packet.size);
        result+=newResult;
    }
    else
        return FAIL;
    return result;
}
int AuthKey::encode(DataPkg* data){
    int nLen = RSA_size(loadedKey);
	char* encodedData = new char[nLen + 1];
	int ret=0;
	if(isPub)
        ret = RSA_public_encrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, loadedKey, RSA_PKCS1_PADDING);
    else
        ret = RSA_private_encrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, loadedKey, RSA_PKCS1_PADDING);
    data->cData=encodedData;
    data->size=ret;
    CRYPTO_cleanup_all_ex_data();
}
string AuthKey::decode(string data){
    char tmp[RSA_DC_LEN];
    char* ptr=tmp;
    DataPkg packet;
    int pointer=0;
    string result="";
    for(int i=0;i<data.size();i++){

        if(i!=0&&i%RSA_DC_LEN==0){
            packet.cData=tmp;
            packet.size=RSA_DC_LEN+0;
            decode(&packet);
           // cout << packet.cData << endl;
            if(packet.size>0){
                string newResult(packet.cData,packet.size);
                result+=newResult;
            }
            else
                return FAIL;
            ptr=tmp;
            pointer++;
            memset(tmp,0,RSA_DC_LEN);
        }
        *ptr=data[i];
        ptr++;
    }
    packet.cData=tmp;
    packet.size=data.size()-RSA_DC_LEN*pointer;
    decode(&packet);
     //cout << packet.size << " ";
    if(packet.size>0){
        string newResult(packet.cData,packet.size);
        result+=newResult;
    }
    else
        return FAIL;
    return result;
}
int AuthKey::decode(DataPkg* data){
    if(data==nullptr)
        return -1;
    int nLen = RSA_size(loadedKey);
	char* encodedData = new char[nLen + 1];
	int ret = 0;
	if(isPub)
        ret = RSA_public_decrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, loadedKey, RSA_PKCS1_PADDING);
	else
        ret = RSA_private_decrypt(data->size,(unsigned char*)data->cData, (unsigned char*)encodedData, loadedKey, RSA_PKCS1_PADDING);
    data->cData=encodedData;
    data->size=ret;
    CRYPTO_cleanup_all_ex_data();
    return 0;
}
void AuthKey::clear(){
    RSA_free(loadedKey);
}
string AuthKey::sha512(const string input){
	char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA512_CTX sha256;
    SHA512_Init(&sha256);
    SHA512_Update(&sha256, input.c_str(), input.size());
    SHA512_Final(hash, &sha256);
    std::string NewString = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(buf,"%02x",hash[i]);
        NewString = NewString + buf;
    }
	return NewString;
}
/*
string AuthKey::aes_encrypt(string in,string key,string initVector){
    std::string NewString = "";
    char buf[2];
    for(int i = 0; i < in.size(); i++){
        sprintf(buf,"%02x",in[i]);
        NewString = NewString + buf;
    }
    char* iv=(char*)malloc(sizeof(char)*AES_BLOCK_SIZE);
    for(int i=0; i<AES_BLOCK_SIZE; ++i)
        iv[i]='0';
    //initVector.copy(iv,AES_BLOCK_SIZE,0);
    AES_KEY aes;
    if(AES_set_encrypt_key((unsigned char*)(key.c_str()), 128, &aes) < 0){
        return "";
    }
    int len=in.size();
    char* out=(char*)malloc(sizeof(char)*4096);
    AES_cbc_encrypt((unsigned char*)(NewString.data()), (unsigned char*)out, len, &aes, (unsigned char*)(iv), AES_ENCRYPT);
    string result(out);
    return result;
}
string AuthKey::aes_decrypt(string in,string key,string initVector){
    char* iv=(char*)malloc(sizeof(char)*AES_BLOCK_SIZE);
    for(int i=0; i<AES_BLOCK_SIZE; ++i)
        iv[i]='0';
    //initVector.copy(iv,AES_BLOCK_SIZE,0);
    AES_KEY aes;
    if(AES_set_encrypt_key((unsigned char*)(key.c_str()), 128, &aes) < 0){
        return "";
    }
    int len=in.size();
    char* out=(char*)malloc(sizeof(char)*4096*4);
    AES_cbc_encrypt((unsigned char*)(in.data()), (unsigned char*)out, len, &aes, (unsigned char*)(iv), AES_DECRYPT);
    string result(out);
    return result;
}
*/

































