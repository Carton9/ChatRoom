#ifndef CLIENTSTARTER_H
#define CLIENTSTARTER_H
#include "ClientTUI.h"
#include "ClientAuth.h"
#include "CppSocket.h"
using namespace std;
class ClientStarter
{
    public:
        ClientStarter();
        void run();
        virtual ~ClientStarter();
        int dependCheck(string file);
        vector<string> splite(string in,char key);
    protected:

    private:
    ClientTUI* ctui=nullptr;
    ClientAuth* clientAuth=nullptr;
    CppSocket* socket=nullptr;
    char* serverIP;
    int serverPort;
};

#endif // CLIENTSTARTER_H
