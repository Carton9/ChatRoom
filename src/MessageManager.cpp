#include "MessageManager.h"

MessageManager::MessageManager(string name){
    //ctor
    this->name=name;
}

Message MessageManager::buildMessage(string data){
    struct tm *info;
    time_t rawTime=time(NULL);
    info = localtime(&rawTime);
    string time="";
    time+=(info->tm_year+1900);
    time+="-";
    time+=(info->tm_mon+"");
    time+=(info->tm_mday+"");
    time+=(info->tm_hour+"");
    time+=(info->tm_min+"");
    time+=(info->tm_sec+"");
    Message ms;
    ms.name=name;
    ms.data=time;
    ms.data=data;
    return ms;
}
string MessageManager::formateMessage(string data){
    struct tm *info;
    time_t rawTime=time(NULL);
    info = localtime(&rawTime);
    string time="";
    time+=(info->tm_year+1900);
    time+="-";
    time+=(info->tm_mon+"");
    time+=(info->tm_mday+"");
    time+=(info->tm_hour+"");
    time+=(info->tm_min+"");
    time+=(info->tm_sec+"");
    string result = name+"$"+time+"$"+data;
    return result;
}
string MessageManager::formateMessage(Message* data){
    string result = data->name+"$"+data->time+"$"+data->data;
    return result;
}
string MessageManager::linkMessage(vector<Message> ms){
    string result=ms.size()+"$"+formateMessage(&(ms[0]));
    for(int i=1;i<ms.size();i++){
        result+="$"+formateMessage(&(ms[i]));
    }
    return result;
}
string MessageManager::linkMessage(vector<Message>* ms){
    string result=linkMessage(*ms);
    delete ms;
    return result;
}
vector<Message> MessageManager::divideMessage(vector<string> variable){
    vector<Message> result;
    if(variable.size()<2)
        return result;
    if(variable[0]==MESSAGE){
        int messageCount=0;
        try{
            messageCount=std::stoi(variable[1]);
        }
        catch(...){
        
        }
        for(int i=0;i<messageCount;i++){
            int readPointer=2+messageCount*3;
           
            try{
                Message data;
                data.name=std::stoi(variable[readPointer]);
                data.data=std::stoi(variable[readPointer+1]);
                data.time=std::stoi(variable[readPointer+2]);
                result.push_back(data);
            }
            catch(...)
            {
            }
        }
    }
    return result;
}

MessageManager::~MessageManager(){
    //dtor
}
