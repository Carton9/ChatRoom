#include <iostream>
#include <string>
#include <time.h>
#include "rsa++.h"
using namespace std;

int status; // 0: init state
            // 1: finish init, constucte first handshake, verify private key
            // 2: finish first handshake, construt secret exchange, connection pass
            // 3: finish second handshake, generate pass, send out checkout package setup live time
            // 4: auth object init finish, into normal mode
string userName;
string password;
string secret;
string generatePacket(string chatRoomNumber){
    string packet=userName+chatRoomNumber;
    return packet+sha256(packet);
}
string generateFirstHandshake(string reply){
    srand (time(NULL));
    
}