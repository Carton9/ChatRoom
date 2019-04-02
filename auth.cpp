#include <iostream>
#include <string>
#include <time.h>
#include "rsa++.h"
#define JOIN "Sk9JTg=="
#define ACCEPTED "YWNjZXB0ZWQ="
using namespace std;

int status; 
            // 0: init state
            // 1: finish init, constucte first handshake, verify private key
            // 2: finish first handshake, construt secret exchange, connection pass
            // 3: finish second handshake, generate pass, send out checkout package setup live time
            // 4: auth object init finish, into normal mode
string userName;
string password;
const string secret="SXQgaXMgdmVyeSBzZWNyZXQ=";
RSA* key；
const string profileFolder="/etc/SCR_config";
void init(string username){
    RSA* key=new RSA(profileFolder+"/user/"+userName+"/KEY");
}
string generatePacket(string chatRoomNumber){
    string packet=userName+chatRoomNumber+JOIN;
    return packet+RSA.sha256(packet);
}
string generateFirstHandshake(string reply){
    srand (time(NULL));
}