#include "ChartCore.h"
#define USER_it std::vector<User>::iterator
#define CHARTROOM_it map<string, ChartRoom*>::iterator
ChartCore::ChartCore(){
    string path=string(serverFolder)+"/chartRooms";
    vector<string> chartRoomFiles=getFiles(path);
    for(string file:chartRoomFiles){
        ifstream is(path+"/"+file,ios::in);
        if(is.is_open()){
            ChartRoom* roomFile=new ChartRoom();
            roomFile->name=file;
            string line;
            string checksum;
            string text;
            while (getline(is, line)){
                text+=line;
                vector<string> data=splite(line,' ');
                if(data.size()>1&&data[0]=="admin"){
                    roomFile->admin=data[1];
                }
                if(data.size()>1&&data[0]=="user"){
                    for(int i=1;i<data.size();i++){
                        roomFile->users.push_back(User(data[i]));
                    }
                }
                if(data.size()>1&&data[0]=="config"){
                    roomFile->isPublic=(data[1]=="y");
                }
                if(data.size()>1&&data[0]=="checksum"){
                    checksum=data[1];
                }
            }
            if(checksum==AuthKey::sha512(text)){
                host[file]=roomFile;
                vector<User>* userList=new vector<User>();
                onlineUser[file]=userList;
            }
	    }
        is.close();
    }
}

ChartCore::~ChartCore(){
    string path=string(serverFolder)+"/chartRooms";
    vector<string> roomIndex;
    for (CHARTROOM_it i=host.begin(); i!=host.end(); i++){
        string text="";
        ofstream os;
        os.open(path+"/"+(*i).first,ios::out|ios::trunc);
        ChartRoom* room=(*i).second;
        os << "admin " << room->admin << endl;
        text+="admin "+room->admin;
        text+="user ";
        for(int i=0;i<room->users.size()-1;i++){
            os << room->users[i].getName() << " ";
            text+=room->users[i].getName()+" ";
        }
        os << room->users[room->users.size()-1].getName() << endl;
        text+=room->users[room->users.size()-1].getName();
        os << "config " << ((room->isPublic)?"y":"n") <<endl;
        os << "checksum " << AuthKey::sha512(text);
        os.close();
        roomIndex.push_back((*i).first);
    }
    for(string index:roomIndex){
        delete host[index];
        delete onlineUser[index];
        close=true;
    }
}
void ChartCore::addToDialogue(string room,Message message){
    for(USER_it i=onlineUser[room]->begin();i!=onlineUser[room]->end();i++){
        (*i).addMessageToDialogue(message);
    }
}
void ChartCore::serverSpeaker(string room,string data){
    for(USER_it i=onlineUser[room]->begin();i!=onlineUser[room]->end();i++){
        (*i).addMessageToDialogue(mg.buildMessage(data));
    }
}
void ChartCore::serverSpeakToOneUser(string room,string name,string data){
     for(USER_it i=onlineUser[room]->begin();i!=onlineUser[room]->end();i++){
        if((*i)==name)
            (*i).addMessageToDialogue(mg.buildMessage(data));
    }
}
bool ChartCore::tryJoinRoom(string room,string username){
    ChartRoom* roomProfile=host[room];
    if(roomProfile=nullptr)
        return false;
    if(find(roomProfile->users.begin(), roomProfile->users.end(),username) != roomProfile->users.end()) {
        serverSpeaker(room,"new user "+username+" comming");
        onlineUser[room]->push_back(User(username));
        serverSpeakToOneUser(room,username,"Welcome");
        return true;
    }
    if(roomProfile->isPublic){
        serverSpeaker(room,"new user "+username+" comming");
        roomProfile->users.push_back(User(username));
        onlineUser[room]->push_back(User(username));
        serverSpeakToOneUser(room,username,"Welcome");
        return true;
    }else{
        return false;
    }
}
void ChartCore::addMessage(string room,string username,Message message){
    ChartRoom* roomProfile=host[room];
    if(roomProfile=nullptr)
        return ;
    if (roomProfile->admin==username) {
        message.data=executeCommend(roomProfile,message.data);
    }else{
        addToDialogue(room,message);
    }
}
vector<Message>* ChartCore::getMessage(string room,string username){
    USER_it it=find(onlineUser[room]->begin(),onlineUser[room]->end(),username);
    if(it != onlineUser[room]->end()) {
        return (*it).getDialogue();
    }
    return nullptr;
}
int ChartCore::manageUser(string username){
    if(close)
        return 0;
    if(find(removedUser.begin(),removedUser.end(),username)!= removedUser.end())
        return 0;
    else
        return 3;
}
string ChartCore::executeCommend(ChartRoom* room,string commend){
    vector<string> commends=splite(commend,'-');
    string commend_p=commends[0];
    vector<User>* userlist=&(room->users);
    vector<User>* actveUserList=onlineUser[room->name];
    if (commend_p==ADDUSER) {
        if (commends.size()>1) {
           room->users.push_back(User(commends[1]));
        }
    }
    else if (commend_p==REMOVEUSER) {
        if (commends.size()>1) {
           userlist->erase(find(userlist->begin(),userlist->end(),commends[1]));
           actveUserList->erase(find(actveUserList->begin(),actveUserList->end(),commends[1]));
           removedUser.push_back(commends[1]);
        }
    }
    else if (commend_p==CLOSEROOM) {
        if (commends.size()>1) {
            for (CHARTROOM_it i=host.begin(); i!=host.end(); i++){
                if((*i).first==commends[1]){
                    for (User user:((*i).second)->users)
                    {
                       removedUser.push_back(user.getName());
                    }
                    
                    
                }
            }
        }
    }
    else if (commend_p==GETUSERLIST) {
        string result="User List\n";
        for(USER_it i=onlineUser[room->name]->begin();i!=onlineUser[room->name]->end();i++){
            result+=(*i).getName()+"\n";
        }
        serverSpeaker(room->name,result);
    }
    else if (commend_p==CREATECHARTROOM) {
        if (commends.size()>3) {
            ChartRoom* room=new ChartRoom();
            room->name=commends[1];
            room->admin=commends[2];
            room->isPublic=(commends[3]=="1");
            host[commends[1]]=room;
        }
    }
}
vector<string> ChartCore::splite(string in,char key){
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
vector<string> ChartCore::getFiles(string searchFolder){
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
		else if(ptr->d_type == 8)
			files.push_back(ptr->d_name);
		else if(ptr->d_type == 10)
			continue;
		else if(ptr->d_type == 4)    ///dir
		{
			files.push_back(ptr->d_name);
		}
	}
	closedir(dir);
    return files;
}
