#ifndef SECRET_H
#define SECRET_H
#include <string>
#define JOIN "Sk9JTg=="
#define HANDSHAKE "aGFuZHNoYWtlCg=="
#define MESSAGE "bWVzc2FnZQoK=="
#define charSet "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#+^&-=\\][';/.,<>?:*"
#define secret "SXQgaXMgdmVyeSBzZWNyZXQ="
#define profileFolder "/etc/SCR_config"
#define serverFolder "/etc/SCRS_config"
#define ACCEPTED "YWNjZXB0ZWQ="
#define REJECT "YWNjZXB0ZWQ="
struct Message{
    std::string name="";
    std::string time="";
    std::string data="";
};
#endif
