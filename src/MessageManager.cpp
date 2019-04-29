#include "MessageManager.h"

MessageManager::MessageManager(string name){
    //ctor
    this->name=name;
}

Message MessageManager::buildMessage(string data){
    struct tm *info;
    time_t rawTime=time(NULL);
    string c_time=ctime(&rawTime);
    Message* ms=new Message();
    (*ms).name=name;
    (*ms).time=c_time.substr(0,c_time.size()-1);
    (*ms).data=data;
    return (*ms);
}
string MessageManager::formateMessage(string data){
    struct tm *info;
    time_t rawTime=time(NULL);
    string c_time=ctime(&rawTime);
    string result = name+"$"+c_time.substr(0,c_time.size()-1)+"$"+data;
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
                data.name=variable[readPointer];
                data.data=variable[readPointer+1];
                data.time=variable[readPointer+2];
                result.push_back(data);
            }
            catch(...)
            {
            }
        }
    }
    return result;
}
Message MessageManager::unpackMessage(string data){
    Message info;
    vector<string> vs=splite(data,'$');
    if (vs.size()>2)
    {
        info.name=vs[0];
        info.data=vs[0+2];
        info.time=vs[0+1];
    }
    return info;

}

MessageManager::~MessageManager(){
    //dtor
}
