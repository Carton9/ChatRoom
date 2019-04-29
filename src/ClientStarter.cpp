#include "ClientStarter.h"
#define R_EN 4
#define W_EN 6
#define RW_EN 9
#define RWX_EN 16

ClientStarter::ClientStarter(){
    if(dependCheck(string(profileFolder)+"/user/")!=RW_EN)return;
    cout << "Please Key in username" << endl;
    string username;
    cin >> username;
    cin.get();
    cout << "Please Key in Server IP address with formate [x.x.x.x:pppp]" << endl;
    cout << "ex: 127.0.0.1:8080" << endl;
    string serverIP;
    cin >> serverIP;
    cin.get();
    cout << "Please Key in Client IP address with formate [x.x.x.x:pppp]" << endl;
    cout << "ex: 127.0.0.1:8080" << endl;
    string clientIP;
    cin >> clientIP;
    cin.get();
    cout << "Please Key in chat room number" << endl;
    string c_room;
    cin >> c_room;
    cin.get();
    clientAuth=new ClientAuth(username);
    if(clientAuth==nullptr||clientAuth->getState()==-1){
        cout << "username incorrect" << endl;
        return;
    }
    vector<string> clientIPresult=splite(clientIP,':');
    if(clientIPresult.size()>1){
        try{
            int port=std::stoi(clientIPresult[1]);
            char* ip=(char*)clientIPresult[0].c_str();
            socket=new CppSocket(UDP,ip,port);
        }
        catch(...){
            cout << "client incorrect" << endl;
        }
    }else{
        cout << "client incorrect" << endl;
    }
    if(socket==nullptr)
        return;
    vector<string> serverIPresult=splite(serverIP,':');
    if(clientIPresult.size()>1){
        try{
            serverPort=std::stoi(serverIPresult[1]);
            serverIP=(char*)serverIPresult[0].c_str();
        }
        catch(...){
            cout << "server incorrect" << endl;
            return;
        }
    }else{
        cout << "server incorrect" << endl;
        return;
    }
    ctui=new ClientTUI(username);
}

ClientStarter::~ClientStarter(){
    //dtor
}
int ClientStarter::dependCheck(string file){
    int result=0;
    if(access(file.c_str(), F_OK)==0){
        result+=1;
    }

    if(access(file.c_str(), W_OK)==0){
        result+=3;
    }
    if(access(file.c_str(), R_OK)==0){
        result+=5;
    }
    if(access(file.c_str(), X_OK)==0){
        result+=7;
    }
    return result;
}
vector<string> ClientStarter::splite(string in,char key){
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