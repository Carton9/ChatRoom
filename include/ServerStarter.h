#ifndef SERVERSTARTER_H
#define SERVERSTARTER_H
#include <string>
#include "Secret.h"
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include "ProcessCore.h"
#include "ChartCore.h"
#include "ServerAuth.h"
#include "CppSocket.h"
#include <CppSocketTerm.h>
using namespace std;

class ServerStarter
{
    public:
        ServerStarter();
        void run();
        virtual ~ServerStarter();

    protected:

    private:
        ChartCore* core=nullptr;
        ServerAuth* authHead=nullptr;
        CppSocket* socket=nullptr;
        static int dependCheck(string file);
        static vector<string> getFiles(string searchFolder);
        vector<string> splite(string in,char key);
};

#endif // SERVERSTARTER_H
