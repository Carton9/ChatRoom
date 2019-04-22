#ifndef MESSAGEGENERATOR_H
#define MESSAGEGENERATOR_H
#include <time.h>
#include <string>
#include <vector>
#include "Secret.h"

using namespace std;

class MessageManager
{
    public:
        MessageManager(string name);
        string formateMessage(string data);
        string formateMessage(Message* data);
        string linkMessage(vector<Message> ms);
        string linkMessage(vector<Message>* ms);
        vector<Message> divideMessage(vector<string> variable);
        Message buildMessage(string data);
        virtual ~MessageManager();
    protected:

    private:
    string name;
};

#endif // MESSAGEGENERATOR_H
/*

*/
