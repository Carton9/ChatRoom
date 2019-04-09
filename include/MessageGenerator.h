#ifndef MESSAGEGENERATOR_H
#define MESSAGEGENERATOR_H
#include "Auth.h"
#include <time.h>
struct message{
    string name;
    string time;
    string data;
}
class MessageGenerator
{
    public:
        MessageGenerator(Auth* auth);
        string formateMessage(string data);
        virtual ~MessageGenerator();

    protected:

    private:
    Auth* auth;
};

#endif // MESSAGEGENERATOR_H
