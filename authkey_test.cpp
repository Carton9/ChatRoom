#include "rsa++.h"
#include <string.h>
using namespace std;
int main(){
    cout << "c";
    string e= AuthKey::aes_encrypt("123321","1234567890123456","1234567890123456");
    
    //cout << e<< endl;
    //cout << AuthKey::aes_decrypt(e,"1234567890123456","1234567890123456");
    return 0;
}