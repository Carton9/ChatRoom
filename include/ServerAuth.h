#ifndef SERVERAUTH_H
#define SERVERAUTH_H
#include "AuthKey.h"
#include "Secret.h"
#include "MessageManager.h"
#include <vector>
#include <string>
#include "ProcessCore.h"
using namespace std;
class ServerAuth{
    public:
        ServerAuth(ServerAuth* previous,string userName,ProcessCore* core);
        string answerCall(string reply);
        vector<Message>* decodePackage(string packet);
        void addUnitToChain(ServerAuth* unit);
        ServerAuth* operator<<(ServerAuth* unit){addUnitToChain(unit);return this;}
        void removeUnitFromChain(string username);
        void operator>>(string username){removeUnitFromChain(username);}
        void operator>>(ServerAuth* unit){removeUnitFromChain(unit->userName);}
        vector<string> splite(string in,char key);
        void removeAllFromChain();
        ServerAuth* getPrevious(){return previous;}
        ServerAuth* getNext(){return next;}
        ServerAuth* operator[](int k){
            ServerAuth* index=this;
            for(int i=0;i<k;i++){
                if(index->next!=nullptr)
                    index=index->next;
            }
            return index;
        }

        ServerAuth* operator[](string name){
            if(userName==name)
                return this;
            else if(next!=nullptr)
                return (*next)[name];
            else 
                return nullptr;
        }
        virtual ~ServerAuth();

    protected:

    private:
        int status;
            //0: init/close connection
            //1: first handshake finish
            //2: second handshake finish
            //3: normal transmission
        string userName;
        string password;
        string sessionSecret_Client;
        string sessionSecret_Server;
        AuthKey* key=nullptr;
        ServerAuth* next=nullptr;
        ServerAuth* previous=nullptr;
        string currentRoom;
        MessageManager* generator=nullptr;
        string answerFirstHandShake(string input);
        string answerSecondHandShake(string input);
        string encodePackage(string packet);
        ProcessCore* core=nullptr;
};

#endif // SERVERAUTH_H
