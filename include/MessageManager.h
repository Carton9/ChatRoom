#ifndef MESSAGEGENERATOR_H
#define MESSAGEGENERATOR_H
#include <time.h>
#include <string>
#include <vector>
#include "Secret.h"

using namespace std;

class MessageManager
{
    public:
        MessageManager(string name);
        string formateMessage(string data);
        static string formateMessage(Message* data);
        string linkMessage(vector<Message> ms);
        string linkMessage(vector<Message>* ms);
        static vector<Message> divideMessage(vector<string> variable);
        Message buildMessage(string data);
        static Message unpackMessage(string data);
        static vector<string> splite(string in,char key){
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
        virtual ~MessageManager();
    protected:

    private:
    string name;

};

#endif // MESSAGEGENERATOR_H
/*

*/
