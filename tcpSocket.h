#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

#define err(errMsg) printf("%s failed code %d \n",errMsg, WSAGetLastError())	//������
#define  PORT 8888		//[0,65536) 0-1024����


//�������
bool init_Socket();
//�ر������
bool close_Socket();
//����������socket
SOCKET createServerSocket();
//�����ͻ���socket
SOCKET createClientSocket(const char *ip);

bool readFile(const char* fileName);
bool saveFile(const char* fileName);
bool sendFile(SOCKET s, const char* fileName);
bool recvFile(SOCKET s, const char* fileName);

#endif 