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


using namespace std;


class ClientTUI
{
    public:
        ClientTUI();
        static void keyListener(ClientTUI* tui);
        static void windowListener(ClientTUI* tui);
        void inputAnalyze();
        void printFrame();
        void printBlock();
        void setCursor(int x,int y){printf("\033[%d;%dH\033[0m",y,x);}
        bool isAlive(){return alive;}
        virtual ~ClientTUI();

    protected:

    private:
    int x=0;
    int y=0;
    static int ClientTUI::scanKeyboard();
    thread* keyListen;
    thread* windowListen;
    bool alive;
    vector<int> inputbuffer;
    vector<Message> messegeBuffer;
    mutex lock;
};

#endif // CLIENTTUI_H
