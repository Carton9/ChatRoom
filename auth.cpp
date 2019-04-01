#include <iostream>
#include <string>
using namespace std;

int status; // 0: init state
            // 1: finish init, constucte first handshake, verify private key
            // 2: finish first handshake, construt secret exchange, connection pass
            // 3: finish second handshake, generate pass, send out checkout package setup live time
            // 4: auth object init finish, into normal mode
string generatePacket(){

}
string generateFirstHandshake(){
    
}