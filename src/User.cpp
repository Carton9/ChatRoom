#include "User.h"

User::User(string name)
{
    this->name=name;
}
void User::addMessageToDialogue(Message m){
    ms.push_back(m);
}
vector<Message>* User::getDialogue(){
    vector<Message>* result=new vector<Message>(ms);
    return result;
}
User::~User()
{
    //dtor
}
