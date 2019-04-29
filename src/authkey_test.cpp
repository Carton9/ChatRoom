#include "AuthKey.h"
#include <string.h>

#include <iostream>
#include <sys/time.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <vector>
#include "ClientTUI.h"
#include "CppSocket.h"

using namespace std;
vector<string> splite(string in,char key){
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
TransData* formateData(char* ip,int port,string info){
    TransData* td;
    td=(TransData*)malloc(sizeof(TransData));
    memset(td,0,sizeof(td));
    td->address.sin_family=AF_INET;
    td->address.sin_addr.s_addr=inet_addr(ip);
    td->address.sin_port = htons(port);
    td->dataBuff=(char*)info.data();
    td->length=info.size();
    return td;
}
string unpackData(TransData* td){
    return string(td->dataBuff,td->length);
}
void listener(ClientTUI* tui,AuthKey* decodeKey,CppSocket* cps){
    while (true){
        TransData* td2=new TransData();
        Error error=cps->recevieData(4096,td2);
        if(NOERROR==error){
            string data=(*decodeKey)>>(unpackData(td2));
            if (1==1){
                Message ms=MessageManager::unpackMessage(data);
                if(ms.name!=""){
                    tui->addMessage(ms);
                    tui->printFrame();
                }
            }
        }else{
            string result="";
            if(error== TIMEOUT)
                result="tf";
            else if (error== SOCKET_ERROR){
                result="se";
            }

            tui->systemMessage(result);
        }

    }
}
void producer(ClientTUI* tui,AuthKey* encodeKey,CppSocket* cps,char* ip,int port){
    while (true){
        Message ms=tui->getMessage();
        string data=(*encodeKey)<<(MessageManager::formateMessage(&ms));
        cps->sendData(formateData(ip,port,data));

    }
}
void frameUpdate(ClientTUI* tui){
    while(true){
        //tui->printFrame();
        sleep(1);
    }
}
int main(){
    /*const string charSet="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#+^&-=\\][';/.,<>?:*";
    AuthKey key("./pubkey.pem",true);
    AuthKey keyp("./privkey.pem",false);
    string word="";
    string word2="";
    cout << rand()<<endl;
    srand(rand());
    int size=60;
    for(int i=0;i<size;i++){
       for(int j=0;j<size;j++)
            word+=charSet[rand()%82];
        cout << word.size() << " " << (key<<word).size() << " " << (word==(keyp>>(key<<word)));
    }*/
    /*
    ClientTUI* tui=new ClientTUI("test");
    while (true)
    {

    }*/
    /*string selex;
    cin >>selex;*/
    string clientIP;
    string serverIP;
    string keySelection;
    string username;
    /*
    if(selex=="1"){
        serverIP="127.0.0.1:9090";
        clientIP="127.0.0.1:9091";
        keySelection="1";
    }else{
        serverIP="127.0.0.1:9091";
        clientIP="127.0.0.1:9090";
        keySelection="2";
    }
    username=selex+" debug";
    */
    /**/
    cout << "Please Key in Client IP address with formate [x.x.x.x:pppp]" << endl;
    cout << "ex: 127.0.0.1:8080" << endl;
    cin >> clientIP;
    cin.get();
    cout << "Please Key in Server IP address with formate [x.x.x.x:pppp]" << endl;
    cout << "ex: 127.0.0.1:8080" << endl;
    cin >> serverIP;
    cin.get();
    cout << "Please Key in the keypair you want to use. (1/2)" << endl;
    cin >> keySelection;
    cin.get();
    cout << "Please Key in the username" << endl;
    cin >> username;
    cin.get();

    AuthKey* encodeKey;
    AuthKey* decodeKey;
    CppSocket* socket;
    vector<string> clientIPresult=splite(clientIP,':');
    if(clientIPresult.size()>1){
        try{
            int port=std::stoi(clientIPresult[1]);
            char* ip=(char*)clientIPresult[0].c_str();
            socket=new CppSocket(UDP,ip,port);
        }
        catch(...){
            cout << "client address incorrect" << endl;
            return 0;
        }
    }else{
        cout << "client address incorrect" << endl;
        return 0;
    }

    int targetPort=0;
    char* targetIP;

    vector<string> serverIPresult=splite(serverIP,':');
    if(clientIPresult.size()>1){
        try{
            targetPort=std::stoi(serverIPresult[1]);
            targetIP=(char*)serverIPresult[0].c_str();
        }
        catch(...){
            cout << "server address incorrect" << endl;
            return 0;
        }
    }else{
        cout << "server address incorrect" << endl;
        return 0;
    }


    if (keySelection=="1") {
        decodeKey=new AuthKey("./pubkey.pem",true);
        encodeKey=new AuthKey("./privkey.pem",false);
    }else if (keySelection=="2") {
        decodeKey=new AuthKey("./pubkey2.pem",true);
        encodeKey=new AuthKey("./privkey2.pem",false);
    }else {
        cout << "incorrect key" << endl;
        return 0;
    }
    cout << "begin";
    //string username="c";
    ClientTUI* tui=new ClientTUI(username);
    thread ls(listener,tui,decodeKey,socket);
    thread ps(producer,tui,encodeKey,socket,targetIP,targetPort);
    thread us(frameUpdate,tui);
    while (true);
    return 0;
}
