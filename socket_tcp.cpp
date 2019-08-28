#include <winsock2.h>
#include <iostream>
#include <stdint.h>
#include <thread>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
class socket_basic
{
public:
    SOCKET sclient;
    WORD sockVersion;
    char *revData;
    char *sendData;
    socket_basic();
    virtual void setup_port_ip(uint16_t port,const char *ip)=0;
    virtual void get_message()=0;
    virtual void send_message()=0;
    virtual void close_socket()=0;
};
class socket_server:public socket_basic
{
    SOCKET slisten;
    sockaddr_in remoteAddr;
public:
    socket_server();
    void setup_port_ip(uint16_t port,const char *ip);
    void get_message();
    void send_message();
    void close_socket();
};
class socket_client:public socket_basic
{
public:
    socket_client();
    void setup_port_ip(uint16_t port,const char *ip);
    void get_message();
    void send_message();
    void close_socket();
};
class thread_socket
{
public:
    void Th_getmessage_server(socket_server &posi);
    void Th_getmessage_client(socket_client &posi);
    void Th_sendmessage_server(socket_server &posi);
    void Th_sendmessage_client(socket_client &posi);
};
int main()
{
    socket_client test;
    thread_socket tem;
    test.setup_port_ip(8888,"127.0.0.1");
    tem.Th_sendmessage_client(test);
    test.get_message();
    test.close_socket();
}
socket_basic::socket_basic()
{
    revData=new char[255];
    sendData=new char[255];
    sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;  
    WSAStartup(sockVersion, &wsaData);
}
socket_server::socket_server()
{
    slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}
void socket_server::setup_port_ip(uint16_t port,const char *ip)
{
    sockaddr_in sin;  
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(port);  
    sin.sin_addr.S_un.S_addr=inet_addr(ip);
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)  
    {  
        cout<<"绑定IP和端口失败";
        return;
    }  
    //开始监听  
    if(listen(slisten, 5) == SOCKET_ERROR)  
    {  
        cout<<"监听失败";  
        return;  
    }
    int nAddrlen = sizeof(remoteAddr);
    cout<<"等待连接..."<<endl;
    sclient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);  
    if(sclient == INVALID_SOCKET)  
    {  
        cout<<"信息接收失败"<<endl;
        return;
    }  
    cout<<"接收到一个连接"<<inet_ntoa(remoteAddr.sin_addr)<<endl;
}
void socket_server::get_message()
{  
    //接收数据  
    while(1)
    {
    int ret = recv(sclient, revData, 255, 0);         
    if(ret > 0)  
    {  
        revData[ret] = 0x00;  
        cout<<revData; 
    }
    }
}
void socket_server::send_message()
{
    while(1)
    {
    cin>>sendData;
    send(sclient, sendData,255, 0); 
    }
}
void socket_server::close_socket()
{
    closesocket(sclient);
    closesocket(slisten);  
    WSACleanup(); 
}
socket_client::socket_client()
{
    sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}
void socket_client::setup_port_ip(uint16_t port,const char *ip)
{
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);//接收方端口号
    serAddr.sin_addr.S_un.S_addr = inet_addr(ip);//接收端ip地址
    if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        cout<<"服务器连接失败"<<endl;
        closesocket(sclient);
        return;
    }
}
void socket_client::get_message()
{
    while(1)
    {
    int ret = recv(sclient, revData, 255, 0);
    if(ret>0){
        revData[ret] = 0x00;
        cout<<revData;
    } 
    }
}
void socket_client::send_message()
{
    while(1)
    {
    cin>>sendData;
    send(sclient, sendData, strlen(sendData), 0);
    }
}
void socket_client::close_socket()
{
    closesocket(sclient);
    WSACleanup();
}
void thread_socket::Th_getmessage_server(socket_server &posi)
{
    thread tem(bind(&socket_server::get_message,&posi));
    tem.detach();
}
void thread_socket::Th_getmessage_client(socket_client &posi)
{
    thread tem(bind(&socket_client::get_message,&posi));
    tem.detach();
}
void thread_socket::Th_sendmessage_server(socket_server &posi)
{
    thread tem(bind(&socket_server::send_message,&posi));
    tem.detach();
}
void thread_socket::Th_sendmessage_client(socket_client &posi)
{
    thread tem(bind(&socket_client::send_message,&posi));
    tem.detach();
}
