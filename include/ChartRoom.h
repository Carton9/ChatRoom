#ifndef CHARTROOM_H
#define CHARTROOM_H
#include <vector>
#include <string>
#include "User.h"
using namespace std;

class ChartRoom
{
    public:
        ChartRoom();
        string name;
        string admin;
        vector<User> users;
        bool isPublic;
        string checksum;
        virtual ~ChartRoom();

    protected:

    private:
};

#endif // CHARTROOM_H
