#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#else

#include <sys/socket.h>     
#include <sys/types.h>      
#include <sys/un.h>
#include <arpa/inet.h>        

#define close closesocket

#endif

int SetupSocket();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
int SocketListen(SOCKET socket, int numConnections);
#else
int SocketListen(int socket, int numConnections);
#endif

void ShutdownSocket();


#endif