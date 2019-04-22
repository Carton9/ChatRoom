#include "ClientTUI.h"
#define DEL 127
#define RETURN 10
#define SPACE 32
#define TAB 9
#define COLOR "\033[41;31m"
#define CLEAR "\033[0m"

ClientTUI::ClientTUI()
{
    struct winsize size;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
    x=size.ws_col;
    y=size.ws_row;
    alive=true;
    windowListen=new thread(windowListener,this);
    keyListen=new thread(keyListener,this);
    
}
void ClientTUI::printFrame(){
    setCursor(0,0);
    printf(COLOR);
    for(int i=0;i<x;i++){
        cout << " ";
    }
    printf(CLEAR);
    cout << endl;
    int pointer=0;
    int messegeArea=y-4;
    bool extraLine=(messegeArea%2);
    int messegeSize=(messegeArea/2);
    int messegeLength=x-2;
    int lineIndex=1;
    for (int i = 0; i < messegeSize; i++){
        printBlock();
        int index=messegeBuffer.size()-(messegeSize-i);
        if(messegeBuffer[index].name.size()+messegeBuffer[index].time.size()<messegeLength-3)
            cout << messegeBuffer[index].name << " @ " << messegeBuffer[index].time << " $";
        else
            cout << messegeBuffer[index].name;
        setCursor(x,lineIndex);
        printBlock();
        cout << endl;
        printBlock();
        lineIndex++;
        if(messegeBuffer[index].data.size()<messegeLength)
            cout << messegeBuffer[index].data << endl;
        else
            cout << "$ Messege is too long $" << endl;
        setCursor(x,lineIndex);
        printBlock();
        cout << endl;
        lineIndex++;
    }
    if(extraLine){
        setCursor(x,lineIndex);
        printBlock();
        cout << endl;
        lineIndex++;
    }
    printf(COLOR);
    for(int i=0;i<x;i++){
        cout << " ";
    }
    printf(CLEAR);
    cout << endl;
    printBlock();
    setCursor(x,y-1);
    printBlock();
    cout << endl;
    printf(COLOR);
    for(int i=0;i<x;i++){
        cout << " ";
    }
    printf(CLEAR);
    setCursor(1,y-1);
    for (size_t i = 0; i < inputbuffer.size(); i++)
    {
        if(i<x-1)
            cout << (char)inputbuffer[i];
    }
    
    
}
void ClientTUI::printBlock(){
    printf(COLOR);
    cout << " ";
    printf(CLEAR);
}
void ClientTUI::keyListener(ClientTUI* tui){
    while(tui->isAlive()){
        int key=scanKeyboard();
        tui->lock.lock();
        if(key==DEL&&tui->inputbuffer.size()>1){
            tui->inputbuffer.pop_back();
        }else if (key==RETURN&&tui->inputbuffer.size()>1){
            tui->inputAnalyze();
        }
        else if(tui->inputbuffer.size()>=tui->x-1){
            tui->inputAnalyze();
        }
        else{
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
