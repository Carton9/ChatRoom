#ifndef CHARTCORE_H
#define CHARTCORE_H
#include "ChartRoom.h"
#include "ProcessCore.h"
#include <algorithm>
#include "MessageManager.h"
#include "User.h"
#include "ChartRoomCommend.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "AuthKey.h"
using namespace std;

class ChartCore : public ProcessCore{
    public:
        ChartCore();
        virtual ~ChartCore();
        bool tryJoinRoom(string room,string username);
        void addMessage(string room,string username,Message message);
        vector<Message>* getMessage(string room,string username);
        int manageUser(string username);
    protected:

    private:
        bool close;
        string executeCommend(ChartRoom* room, string commend);
        void serverSpeaker(string room,string data);
        void serverSpeakToOneUser(string room,string name,string data);
        vector<string> splite(string in,char key);
        map<string,ChartRoom*> host;
        map<string,vector<User>*> onlineUser;
        vector<string> removedUser;
        MessageManager mg=MessageManager("Server");
        static vector<string> getFiles(string searchFolder);
        void addToDialogue(string room,Message message);
};

#endif // CHARTCORE_H
