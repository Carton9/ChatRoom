#include "ServerAuth.h"

ServerAuth::ServerAuth(ServerAuth* previous,string userName,ProcessCore* core){
    this->previous=previous;
    this->userName=userName;
    this->core=core;
    string keyFile=string(serverFolder)+"/user/"+userName+"/PUBKEY";
    key=new AuthKey(keyFile,true);
    generator=new MessageManager(userName);
    status=1;
}

string ServerAuth::answerCall(string input){
    string user_handshake=(*key)<<input;
    if(user_handshake==FAIL){
        if(next==nullptr)
            return "";
        else
            return next->answerCall(input);
    }
    if(status==1)
        return answerFirstHandShake(user_handshake);
    else if(status==2)
        return answerSecondHandShake(user_handshake);
    else if(status==3){
        vector<string> rawData=splite(user_handshake,'$');
        vector<Message> messages=generator->divideMessage(rawData);
        for(vector<Message>::iterator it=messages.begin();it!=messages.end();it++){
            core->addMessage(currentRoom,userName,*it);
        }
        status=core->manageUser(userName);
        string result=generator->linkMessage((core->getMessage(currentRoom,userName)));
        return encodePackage(result);
    }
    else
        return "";
}

string ServerAuth::answerFirstHandShake(string input){
    string user_handshake=input;
    vector<string> result=splite(user_handshake,'$');
    if(result.size()>3&&result[0]==HANDSHAKE){
        if (result[1]==userName) {
            if(core->tryJoinRoom(result[2],userName)){
                status=2;
                currentRoom=result[2];
                return (*key)>>(string(HANDSHAKE)+"$"+string(ACCEPTED));
            }
        }
    }
    return (*key)>>(string(HANDSHAKE)+"$"+string(REJECT));
}

string ServerAuth::answerSecondHandShake(string input){
    string user_handshake=input;
    vector<string> result=splite(user_handshake,'$');
    if(result.size()>2&&result[0]==HANDSHAKE){
        sessionSecret_Client=result[2];
        srand(time(NULL));
        for(int i=0;i<20;i++){
            sessionSecret_Server+=charSet[rand()%82];
        }
        sessionSecret_Server=key->sha512(sessionSecret_Server);
        return (*key)>>(string(HANDSHAKE)+"$"+sessionSecret_Server);
    }
    return "";
}

string ServerAuth::encodePackage(string packet){
    string chacksum=key->sha512(packet+sessionSecret_Server);
    //sessionSecret=chacksum;
    string result=string(MESSAGE)+"$"+packet+"$"+chacksum;
    return (*key)<<(packet);
}

void ServerAuth::addUnitToChain(ServerAuth* unit){
    if(next==nullptr){
        next=unit;
        unit->previous=this;
    }
    else
        next->addUnitToChain(unit);
}

void ServerAuth::removeUnitFromChain(string username){
    if (userName==username) {
        if (previous!=nullptr&&next!=nullptr) {
            previous->next=next;
            next->previous=previous;
        }else if (previous==nullptr) {
            next->previous=previous;
        }else if (next==nullptr) {
            previous->next=next;
        }
        this->~ServerAuth();
    }else{
        next->removeUnitFromChain(username);
    }

}

vector<string> ServerAuth::splite(string in,char key){
    int pos;
    string pattern="";
    pattern+=key;
    std::vector<std::string> result;
    in+=pattern;
    int s_size=in.size();
    for(int i=0; i<s_size; i++){
        pos=in.find(pattern,i);
        if(pos<s_size){
            std::string s=in.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

void ServerAuth::removeAllFromChain(){
    if(next!=nullptr)
        next->removeAllFromChain();
    this->~ServerAuth();
}

ServerAuth::~ServerAuth(){
    delete key;
    delete generator;
}
/*
    string user_handshake=(*key)<<input;
    if(user_handshake==FAIL){
        if(next==nullptr)
            return "";
        else
            return next->answerFirstHandShake(input,username);
    }
    vector<string> result=splite(user_handshake,'$');
    if(result[0]==)
    return (*key)>>(HANDSHAKE+"$"+ACCEPTED);
*/
