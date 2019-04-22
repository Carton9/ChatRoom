#include "ServerStarter.h"
#define R_EN 4
#define W_EN 6
#define RW_EN 9
#define RWX_EN 16


ServerStarter::ServerStarter(){

    if(dependCheck(string(serverFolder)+"/chartRooms")==RW_EN){
        core=new ChartCore();
    }
    if(dependCheck(string(serverFolder)+"/user")==RW_EN){
        vector<string> files=getFiles(string(serverFolder)+"/user");
        if (files.size()>0){
            if (dependCheck(string(serverFolder)+"/user/"+files[0]+"/PUBKEY")>=R_EN){
                authHead=new ServerAuth(nullptr,files[0],core);
                for (int i = 1; i < files.size(); i++){
                    if (dependCheck(string(serverFolder)+"/user/"+files[i]+"/PUBKEY")>=R_EN)
                        (*authHead)<< new ServerAuth(nullptr,files[i],core); 
                }
            }
        }
    }
    cout << "Please Key in IP address with formate [x.x.x.x:pppp]" << endl;
    cout << "ex: 127.0.0.1:8080" << endl;
    string IP;
    cin >> IP;
    cin.get();
    vector<string> ipresult=splite(IP,':');
    if(ipresult.size()>1){
        try{
            int port=std::stoi(ipresult[1]);
            char* ip=(char*)ipresult[0].c_str();
            socket=new CppSocket(UDP,ip,port);
        }
        catch(...){
        }
    }
    
    
}
void ServerStarter::run(){
    while (core!=nullptr&&socket!=nullptr&&core->manageUser(string("Server"))==3){
        TransData data;
        socket->recevieData(4096,&data);
        string reply=authHead->answerCall(string(data.dataBuff));
        std::strcpy (data.dataBuff, reply.c_str());
        data.length=reply.size();
        socket->sendData(&data);
    }
}
ServerStarter::~ServerStarter()
{
    //dtor
}
int dependCheck(string file){
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
vector<string> ServerStarter::getFiles(string searchFolder){
	vector<string> files;
    DIR *dir;
	struct dirent *ptr;
	if ((dir=opendir(searchFolder.c_str())) == NULL){
        return files;
    }

	while ((ptr=readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
		        continue;
		else if(ptr->d_type == DT_DIR)
			files.push_back(ptr->d_name);
	}
	closedir(dir);
    return files;
}
vector<string> ServerStarter::splite(string in,char key){
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