#ifndef USER_H
#define USER_H
#include "Secret.h"
#include <vector>
#include <string>
using namespace std;
class User
{
    public:
        User(string name);
        string getName(){return name;}
        void addMessageToDialogue(Message m);
        vector<Message>* getDialogue();
        bool operator==(const User& usr){return usr.name==name;}
        bool operator==(const string usr){return usr==name;}
        virtual ~User();

    protected:

    private:
        vector<Message> ms;
        string name;
};

#endif // USER_H
