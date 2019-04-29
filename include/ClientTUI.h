#ifndef CLIENTTUI_H
#define CLIENTTUI_H

#include <string>
#include <termio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Secret.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <deque>
#include "MessageManager.h"
#include "ChartRoomCommend.h"


using namespace std;


class ClientTUI
{
    public:
        ClientTUI(string username);
        static void keyListener(ClientTUI* tui);
        static void windowListener(ClientTUI* tui);
        void inputAnalyze();
        void printFrame();
        void printBlock();
        void setCursor(int x,int y){printf("\033[%d;%dH\033[0m",y,x);}
        bool isAlive(){return alive;}
        void systemMessage(string data){
            messegeBuffer.push_back(clientMmer->buildMessage(data));
            printFrame();
        }
        Message getMessage(){
            Message ms;
            while(sendOutBuffer.size()==0);
            if (1==1){
                lock.lock();
                ms=sendOutBuffer[0];
                sendOutBuffer.pop_front();
                lock.unlock();
            }
            return ms;
        }
        void addMessage(Message ms){
            lock.lock();
            messegeBuffer.push_back(ms);
            lock.unlock();
        }
        virtual ~ClientTUI();

    protected:

    private:
    string username;
    int x=0;
    int y=0;
    static int scanKeyboard();
    thread* keyListen;
    thread* windowListen;
    bool alive;
    vector<int> inputbuffer;
    vector<Message> messegeBuffer;
    deque<Message> sendOutBuffer;
    MessageManager* mmer;
    MessageManager* clientMmer;
    mutex lock;
    mutex pLuck;
};

#endif // CLIENTTUI_H
