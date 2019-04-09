#include "MessageGenerator.h"

MessageGenerator::MessageGenerator(Auth* auth)
{
    //ctor
    this->auth=auth;
}

string MessageGenerator::formateMessage(string data){
    struct tm *info;
    time_t rawTime=time(NULL);
    info = localtime(&rawTime);
    string time=(info.tm_year+1900)+"-"+(info.tm_mon+"")+(info.tm_mday+"")+(info.tm_hour+"")+(info.tm_min+"")+(info.tm_sec+"");
    string result = auth->getUserName()+"$"+time+"$"+data;
    return result;
}

MessageGenerator::~MessageGenerator()
{
    //dtor
}
