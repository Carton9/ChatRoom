#include "ClientTUI.h"
#define DEL 127
#define RETURN 10
#define SPACE 32
#define TAB 9
#define COLOR "\033[41;31m"
#define CLEAR "\033[0m"

ClientTUI::ClientTUI(string username)
{
    mmer=new MessageManager(username);
    clientMmer=new MessageManager("System");
    this->username=username;

    struct winsize size;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
    x=size.ws_col;
    y=size.ws_row;
    alive=true;
    this->printFrame();
    windowListen=new thread(windowListener,this);
    keyListen=new thread(keyListener,this);

}
void ClientTUI::printFrame(){
    // print out side
    //print row
    pLuck.lock();
    system("clear");
    setCursor(0,0);
    for (int i = 0; i < x; i++){
        printBlock();
    }
    printf(CLEAR);
    setCursor(0,y-2);
    for (int i = 0; i < x; i++){
        printBlock();
    }
    printf(CLEAR);
    setCursor(0,y);
    for (int i = 0; i < x; i++){
        printBlock();
    }
    //print col
    for (int i = 0; i < y+1; i++){
        setCursor(0,i);
        printBlock();
        setCursor(x,i);
        printBlock();
    }
    int messegeLength=x-2;
    int messegeArea=y-4;
    bool extraLine=(messegeArea%2);
    int messegeSize=(messegeArea/2);
    vector<Message>* printout;
    if (messegeBuffer.size()>messegeSize){
        printout=new vector<Message>(messegeBuffer.end()-messegeSize,messegeBuffer.end());
    }else{
         printout=new vector<Message>(messegeBuffer);
    }
    for (int i = 0; i < printout->size(); i++){

        setCursor(2,(i*2)+2);
        if((*printout)[i].name.size()+(*printout)[i].time.size()<messegeLength-3)
            cout << (*printout)[i].name << " @ " << (*printout)[i].time << " $";
        else
            cout << (*printout)[i].name;
        setCursor(2,(i*2)+3);
        if((*printout)[i].data.size()<messegeLength)
            cout << "     " << (*printout)[i].data;
        else
            cout << "     "  << "$ Messege is too long $";
    }
    setCursor(2,y-1);
    cout << username << "$  ";
    for (size_t i = 0; i < inputbuffer.size(); i++)
    {
        if(i<x-1)
            cout << (char)inputbuffer[i];
    }
    cout.flush();
    pLuck.unlock();
}
void ClientTUI::inputAnalyze(){
    string commed="";
    string data="";

    if(inputbuffer.size()==0)
        return;
    if ((char)inputbuffer[0]=='$'){
        bool isCommend=true;
        for (int i = 1; i < inputbuffer.size(); i++){
            if (inputbuffer[i]==SPACE){
                isCommend=false;
            }else if (isCommend){
                commed+=(char)inputbuffer[i];
            }else{
                data+=(char)inputbuffer[i];
            }
        }
        string formatedCommend="";

        if (commed=="ADDUSER"){
            formatedCommend=ADDUSER;
            formatedCommend+="-";
            formatedCommend+=data;
            messegeBuffer.push_back(clientMmer->buildMessage("Commend <ADDUSER> Detected."));
        }
        else if (commed=="REMOVEUSER"){
            formatedCommend=REMOVEUSER;
            formatedCommend+="-";
            formatedCommend+=data;
            messegeBuffer.push_back(clientMmer->buildMessage("Commend <REMOVEUSER> Detected."));
        }
        else if (commed=="GETUSERLIST"){
            formatedCommend=GETUSERLIST;
            formatedCommend+="-";
            formatedCommend+=data;
            messegeBuffer.push_back(clientMmer->buildMessage("Commend <GETUSERLIST> Detected."));
        }
        else if (commed=="CREATECHARTROOM"){
            formatedCommend=CREATECHARTROOM;
            formatedCommend+="-";
            formatedCommend+=data;
            messegeBuffer.push_back(clientMmer->buildMessage("Commend <CREATECHARTROOM> Detected."));
        }
        else if (commed=="DELETEROOM"){
            formatedCommend=DELETEROOM;
            formatedCommend+="-";
            formatedCommend+=data;
            messegeBuffer.push_back(clientMmer->buildMessage("Commend <DELETEROOM> Detected."));
        }
        else if (commed=="GOOGLE"){

            string google="";
            google+="\033[34;40mG";
            google+="\033[31;40mO";
            google+="\033[33;40mO";
            google+="\033[34;40mG";
            google+="\033[32;40mL";
            google+="\033[31;40mE";
            google+="\033[0m";
            formatedCommend=google;
            messegeBuffer.push_back(clientMmer->buildMessage(google));
        }
        else if (commed=="HELP"){
            formatedCommend=DELETEROOM;
            formatedCommend+="-";
            formatedCommend+=data;
            messegeBuffer.push_back(clientMmer->buildMessage("ADDUSER: add user to room"));
            messegeBuffer.push_back(clientMmer->buildMessage("REMOVEUSER: remove user from room"));
            messegeBuffer.push_back(clientMmer->buildMessage("GETUSERLIST: get user list"));
            messegeBuffer.push_back(clientMmer->buildMessage("CREATECHARTROOM: create new room"));
            messegeBuffer.push_back(clientMmer->buildMessage("DELETEROOM: remove room"));
        }
        else {
            messegeBuffer.push_back(clientMmer->buildMessage(commed+": command not found"));
        }
        sendOutBuffer.push_back(mmer->buildMessage(formatedCommend));
    }else{

        for (int i = 0; i < inputbuffer.size(); i++){
            data+=(char)inputbuffer[i];
        }
        Message ms1=mmer->buildMessage(data);
        Message ms2=mmer->buildMessage(data);
        sendOutBuffer.push_back(ms1);
        messegeBuffer.push_back(ms2);
    }
    inputbuffer.clear();
}
void ClientTUI::printBlock(){
    printf(COLOR);
    cout << " ";
    printf(CLEAR);
}
void ClientTUI::keyListener(ClientTUI* tui){
    tui->printFrame();
    while(tui->isAlive()){
        int key=scanKeyboard();
        tui->lock.lock();
        if(key==DEL&&tui->inputbuffer.size()>0){
            tui->inputbuffer.pop_back();
        }else if (key==RETURN&&tui->inputbuffer.size()>0){
            tui->inputAnalyze();

        }
        else if(tui->inputbuffer.size()>=tui->x-1){
            tui->inputAnalyze();
        }
        else{
            if(key!=RETURN&&key!=DEL)
                tui->inputbuffer.push_back(key);
        }
        tui->lock.unlock();
        tui->printFrame();
    }
}
void  ClientTUI::windowListener(ClientTUI* tui){

    int tmpX=0;
    int tmpY=0;
    while(tui->isAlive()){
        struct winsize w;
        ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
        if(tmpX!=w.ws_col||tmpY!=w.ws_row){
            tmpX=w.ws_col;
            tmpY=w.ws_row;
            tui->lock.lock();
            tui->x=w.ws_col;
            tui->y=w.ws_row;
            tui->printFrame();
            tui->lock.unlock();
        }

    }


}
ClientTUI::~ClientTUI(){
    //dtor
}
int ClientTUI::scanKeyboard(){
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);

    in = getchar();

    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}
