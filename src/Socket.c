/*
	Copyright 2014 Brennan Butler

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
	

	Socket.c -
		
		Handles setting up the sockets and recieving and sending of data.
*/

#include "Socket.h"

char response[] = "HTTP/1.x 200 OK\n"
"Content-Type: text/html; charset=UTF-8\nContent-Length: 100\nConnection: close\n\n"
"<doctype !html><html><head><title>Test</title></head>"
"<body><h1>This is a test.</h1></body></html>\n";

int SetupSocket()
{
	SOCKET client;
	WSADATA data;
	char recvbuffer[512];
	struct sockaddr_in serverinfo;
	int bytesReceived = 0;

	if (WSAStartup(MAKEWORD(2, 2), &data) != NO_ERROR)
	{
		printf("WSAStartup failed << Windows\n");
		return 0;
	}

	memset(&serverinfo, 0, sizeof(serverinfo));

	serverinfo.sin_family = 2;
	serverinfo.sin_addr.s_addr = htonl(INADDR_ANY);
	serverinfo.sin_port = htons(80);


	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Error sock()");
		printf("\n%d last error \n", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	if ((bind(sock, (SOCKADDR *)&serverinfo, sizeof(serverinfo))) == SOCKET_ERROR)
	{
		printf("\n%d last error \n", WSAGetLastError());
		WSACleanup();
		printf("Error binding");
		closesocket(sock);
		return 0;
	}

	if (!SocketListen(sock, 5))
	{
		printf("Error on listening\n");
		return 0;
	}

	while (1)
	{
		client = accept(sock, NULL, NULL);

		if (client == INVALID_SOCKET)
			printf("\n%d last error \n", WSAGetLastError());

		bytesReceived = recv(client, recvbuffer, 512, 0);

		if (bytesReceived == -1)
			printf("\n%d last error \n", WSAGetLastError());

		printf("%d\n\n", bytesReceived);

		recvbuffer[bytesReceived] = 0;

		printf("%s\n", recvbuffer);

		send(client, response, strlen(response), 0);

		closesocket(client);
	}

	return 1;
}

int SocketListen(SOCKET socket, int numConnections)
{
	if ((listen(socket, numConnections)) == SOCKET_ERROR)
	{
		printf("\n%d last error \n", WSAGetLastError());
		WSACleanup();
		closesocket(sock);
		return 0;
	}
	return 1;
}

void ShutdownSocket()
{

}