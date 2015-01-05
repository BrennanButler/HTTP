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
"Content-Type: text/html; charset=UTF-8\nContent-Length: 100\nConnection: close\n\n";

const char DEF_WWW[74] = "C:/Users/Brennan/Documents/Visual Studio 2013/Projects/FatHTTP/FatHTTP/www";

#define	DEF_BUFSIZE	512

int SetupSocket()
{
	SOCKET client;
	WSADATA data;
	char recvbuffer[DEF_BUFSIZE];
	struct sockaddr_in serverinfo;
	int bytesReceived = 0;

	// Windows only
	// Startup the winsocks with version 2.2
	//	
	//	TODO: Make code cross compatible.
	if (WSAStartup(MAKEWORD(2, 2), &data) != NO_ERROR)
	{
		printf("WSAStartup failed << Windows\n");
		return 0;
	}

	// Null the struct.
	memset(&serverinfo, 0, sizeof(serverinfo));

	serverinfo.sin_family = 2;
	serverinfo.sin_addr.s_addr = htonl(INADDR_ANY);
	serverinfo.sin_port = htons(80);

	// Create the socket ready for use.
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("There was an error creating the socket. [%d]\n", WSAGetLastError());
		WSACleanup();

		return 0;
	}

	// Bind the socket to port 80 ready for listening.
	if ((bind(sock, (SOCKADDR *)&serverinfo, sizeof(serverinfo))) == SOCKET_ERROR)
	{
		
		printf("There was an error binding the socket. [%d]\n", WSAGetLastError());
		WSACleanup();

		closesocket(sock);

		return 0;
	}

	// Start listening for connection on port 80
	if (!SocketListen(sock, 5))
	{
		printf("Error on listening\n");
		return 0;
	}

	while (1)
	{
		client = accept(sock, NULL, NULL);

		if (client == INVALID_SOCKET)
			printf("There was an error creating the client socket [%d]\n", WSAGetLastError());

		bytesReceived = recv(client, recvbuffer, DEF_BUFSIZE, 0);

		if (bytesReceived == -1)
			continue;

		printf("Bytes recieved [%d]\n\n", bytesReceived);

		recvbuffer[bytesReceived] = 0;

#if defined(_DEBUG)
		printf("%s\n", recvbuffer);
#endif
		//ParseRequest(&recvbuffer);

		printf("%s", recvbuffer);

		char method[10];
		char requested[512 / 2];
		char proto_version[20];
		int s;
		if (s = sscanf(&recvbuffer, "%s %s %s", &method, &requested, &proto_version) != 3)
		{
			printf("sscanf probrem %d\n", s);
		}

		printf("%s %s%s %s\n", method, DEF_WWW, requested, proto_version);

		char requested_file[512];

		strcpy(&requested_file, DEF_WWW);
		strcat(&requested_file, requested);

		FILE *file = fopen(requested_file, "r");
		if (file == NULL)
		{
			send(client, "HTTP /1.x 200 OK\n"
				"Content-Type: text/html;\ncharset=UTF-8\nContent-Length: 100\nConnection: close\n\n<html><head><title>404</title></head><body>file not found</body></html>\n", 168, 0);
		}
		else
		{
			send(client, "HTTP /1.x 200 OK\n"
				"Content-Type: text/html;\ncharset=UTF-8\nContent-Length: 100\nConnection: close\n\n<html><head><title>404</title></head><body>file not found</body></html>\n", 168, 0);
			fclose(file);
		}
		
		//send(client, response, strlen(response), 0);
		ConstructHeader("HTTP", "1.1", "404", "text/html", "Connection: close", "<div></div>");
		closesocket(client);
	}

	return 1;
}

int ConstructHeader(const char *method, const char *version, const char *resp_code, const char *content_type, const char *extra_fields, const char *content)
{
	printf("%s %s %s %s %s", method, version, resp_code, content_type, extra_fields);
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