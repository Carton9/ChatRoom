#include "ClientAuth.h"

ClientAuth::ClientAuth(string username){
    //ctor
    init(username);
}
void ClientAuth::init(string username){
    this->userName=userName;
    string keyFile=string(profileFolder)+"/user/"+userName+"/KEY";
    key=new AuthKey(keyFile,false);
    generator=new MessageManager(userName);
    if(key->getState()==-1)
        status=-1;
    else
        status=0;
}
string ClientAuth::generateFirstHandshake(string chatRoomNumber){
    string packet=string(HANDSHAKE)+"$"+userName+"$"+chatRoomNumber+"$"+string(JOIN);
    packet=(*key)<<(packet+AuthKey::sha512(packet));
    if(packet!=FAIL)
        status=1;
    return packet;
}
string ClientAuth::generateSecondHandshake(string reply){
    string message=(*key)>>reply;
    if(message==FAIL)
        return "";
    vector<string> variable = splite(message,'$');
    if(variable[0]!=HANDSHAKE||ACCEPTED!=variable[1]){
        return "";
    }
    status=2;
    srand(time(NULL));
    for(int i=0;i<20;i++){
        sessionSecret_Client+=charSet[rand()%82];
    }
    sessionSecret_Client=key->sha512(sessionSecret_Client);
    string packet=string(HANDSHAKE)+"$"+"GETALLMESS$"+sessionSecret_Client;
    packet=(*key)<<packet;
    return packet;
}
void ClientAuth::finishHandshake(string reply){
    string message=(*key)>>reply;
    if(message==FAIL)
        return ;
    vector<string> variable = splite(message,'$');
    if(variable[0]==HANDSHAKE){
        sessionSecret_Server=variable[1];
    }
}
string ClientAuth::encodePackage(string packet){
    string chacksum=key->sha512(packet+sessionSecret_Client);
    //sessionSecret=chacksum;
    string result=string(MESSAGE)+"$"+(1+"")+"$"+packet+"$"+chacksum;
    return (*key)<<(packet);
}
vector<Message>* ClientAuth::decodePackage(string packet){
    string message=(*key)>>packet;
    if(message==FAIL)
        return nullptr;
    vector<string> variable = splite(message,'$');
    vector<Message> result=generator->divideMessage(variable);
    return &result;
}
vector<string> ClientAuth::splite(string in,char key){
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
ClientAuth::~ClientAuth(){
    //dtor
    delete generator;
}
