#include <iostream>
#include <string>
#include <time.h>
#include "rsa++.h"
#define JOIN "Sk9JTg=="
#define ACCEPTED "YWNjZXB0ZWQ="
using namespace std;
const string charSet="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#+^&-=\\][';/.,<>?:*";
string AES_key="";
string AES_iv="";
int status; 
            // 0: init state
            // 1: finish init, constucte first handshake, verify private key
            // 2: finish first handshake, construt secret exchange, connection pass
            // 3: finish second handshake, generate pass, send out checkout package setup live time
            // 4: auth object init finish, into normal mode
string userName;
string password;
const string secret="SXQgaXMgdmVyeSBzZWNyZXQ=";
AuthKey* key；
string profileFolder="/etc/SCR_config";
void init(string username){
    RSA* key=new RSA(profileFolder+"/user/"+userName+"/KEY");
    status=0;
}
string generatePacket(string chatRoomNumber){
    string packet=userName+"$"+chatRoomNumber+"$"+JOIN;
    status=1;
    packet=key->encode(packet+AuthKey.sha256(packet));
    return packet;
}
string generateFirstHandshake(string reply){
    if(secret！=key->decode(replay)){
        return "fail";
    }
    srand(time(NULL));
    for(int i=0;i<16;i++){
        AES_key+=charSet[rand()%82];
        AES_iv+=charSet[rand()%82];
    }
    string packet=key->encode(AES_key+"$"+AES_iv+"$"+AuthKey.sha256(AES_key+AES_iv));
    status=2;
    return packet;
}
string generateSecondHandshake(string reply){
    if(RSA.sha256(AES_key+AES_iv)！=key->decode(replay)){
        return "fail";
    }
//    string packet=key->encode(key+"$"+iv+"$"+AuthKey.sha256(AES_key+iv));
//    status=2;
    return "";
}