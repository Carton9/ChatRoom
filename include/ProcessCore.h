#ifndef PROCESSCORE_H
#define PROCESSCORE_H
#include <vector>
#include <string>
#include <map>
#include "Secret.h"
using namespace std;

class ProcessCore{
    public:
        virtual bool tryJoinRoom(string room,string username)=0;
        virtual void addMessage(string room,string username,Message message)=0;
        virtual vector<Message>* getMessage(string room,string username)=0;
        virtual int manageUser(string username)=0;
    protected:
    private:
};

#endif // PROCESSCORE_H
