#ifndef AUTH_H
#define AUTH_H
#include "AuthKey.h"
#include "MessageGenerator.h"
#include <vector>

using namespace std;

class Auth
{
    public:
        Auth();
        void init(string username);
        string generateFirstHandshake(string chatRoomNumber);
        string generateSecondHandshake(string reply);
        string encodePackage(string packet);
        vector<message> decodePackage(string packet);
        vector<string> splite(string in,char key);
        void finishHandshake(string reply);
        string getUserName(){return userName;}
        virtual ~Auth();

    protected:

    private:
        const string JOIN = "Sk9JTg==";
        const string HANDSHAKE = "aGFuZHNoYWtlCg==";
        const string MESSAGE = "bWVzc2FnZQoK==";
        const string charSet="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#+^&-=\\][';/.,<>?:*";
        int status;
                    // 0: init state
                    // 1: finish init, constucte first handshake, verify private key
                    // 2: finish first handshake, construt secret exchange, connection pass
                    // 3: finish second handshake, generate pass, send out checkout package setup live time
                    // 4: auth object init finish, into normal mode
        string userName;
        string password;
        string sessionSecret;
        string sessionSecret2;
        const string secret="SXQgaXMgdmVyeSBzZWNyZXQ=";
        AuthKey* key;
        string profileFolder="/etc/SCR_config";
};

#endif // AUTH_H
