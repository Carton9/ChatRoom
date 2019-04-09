#include "Auth.h"
using namespace std;
string ACCEPTED="YWNjZXB0ZWQ=";
Auth::Auth()
{
    //ctor
}
void Auth::init(string username){
    string keyFile=profileFolder+"/user/"+userName+"/KEY";
    AuthKey* key=new AuthKey(keyFile,false);

    status=0;
}
string Auth::generateFirstHandshake(string chatRoomNumber){
    string packet=HANDSHAKE+"$"+userName+"$"+chatRoomNumber+"$"+JOIN;
    packet=(*key)<<(packet+AuthKey::sha512(packet));
    if(packet!=FAIL)
        status=1;
    return packet;
}
string Auth::generateSecondHandshake(string reply){
    string message=(*key)>>reply;
    if(message==FAIL)
        return FAIL;
    vector<string> variable = splite(message,'$');
    if(ACCEPTED!=variable[1]){
        return FAIL;
    }
    status=2;
    srand(time(NULL));
    for(int i=0;i<20;i++){
        sessionSecret+=charSet[rand()%82];
    }
    sessionSecret=key->sha512(sessionSecret);
    string packet=HANDSHAKE+"$"+"GETALLMESS$"+sessionSecret;
    packet=(*key)<<packet;
    return packet;
}
void Auth::finishHandshake(string reply){
    string message=(*key)>>reply;
    if(message==FAIL)
        return ;
    vector<string> variable = splite(message,'$');
    if(variable[0]==HANDSHAKE){
        sessionSecret2=variable[1];
    }
}
string Auth::encodePackage(string packet){
    string chacksum=key->sha512(packet+sessionSecret);
    //sessionSecret=chacksum;
    string result=MESSAGE+"$"+(1+"")+"$"+packet+"$"+chacksum;
    return (*key)<<(packet);
}
vector<message> Auth::decodePackage(string packet){
    string message=(*key)>>packet;
    if(message==FAIL)
        return FAIL;
    vector<string> variable = splite(message,'$');
    if(variable.size()<2)
        return FAIL;
    if(variable[0]==MESSAGE){
        int messageCount=stoi(variable[1]);
        int pointer=1;
        for(int i=0;i<messageCount;i++){

        }
    }

}
vector<string> Auth::splite(string in,char key){
    vector<string> result;
    string section="";
    for(int i=0;i<in.size();i++){
        if(in[i]==key){
            result.push_back(section);
        }else{
            section+=in[i];
        }
    }
    return result;
}
Auth::~Auth(){
    //dtor
}
