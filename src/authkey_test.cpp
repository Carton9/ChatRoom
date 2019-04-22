#include "AuthKey.h"
#include <string.h>

#include <iostream>
#include <sys/time.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <vector>

using namespace std;
int main(){
    const string charSet="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#+^&-=\\][';/.,<>?:*";
    AuthKey key("./pubkey.pem",true);
    AuthKey keyp("./privkey.pem",false);
    string word="";
    string word2="";
    cout << rand()<<endl;
    srand(rand());
    int size=60;
    for(int i=0;i<size;i++){
       for(int j=0;j<size;j++)
            word+=charSet[rand()%82];
        cout << word.size() << " " << (key<<word).size() << " " << (word==(keyp>>(key<<word)));
    }
    
    return 0;
}
