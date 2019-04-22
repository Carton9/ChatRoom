#ifndef AUTH_H
#define AUTH_H
#include "AuthKey.h"
#include "MessageManager.h"
#include "Secret.h"
#include <vector>
#include <string>

using namespace std;

class ClientAuth{
    public:
        ClientAuth(string username);
        void init(string username);
        string generateFirstHandshake(string chatRoomNumber);
        string generateSecondHandshake(string reply);
        string encodeMessage(string message){return encodePackage(generator->formateMessage(message));}
        string encodePackage(string packet);
        vector<Message>* decodePackage(string packet);
        vector<string> splite(string in,char key);
        void finishHandshake(string reply);
        string getUserName(){return userName;}
        virtual ~ClientAuth();

    protected:

    private:
        int status;
                    // 0: init state
                    // 1: finish init, constucte first handshake, verify private key
                    // 2: finish first handshake, construt secret exchange, connection pass
                    // 3: finish second handshake, generate pass, send out checkout package setup live time
                    // 4: auth object init finish, into normal mode
        string userName;
        string password;
        string sessionSecret_Client;
        string sessionSecret_Server;

        AuthKey* key;

        MessageManager* generator;
};

#endif // AUTH_H
