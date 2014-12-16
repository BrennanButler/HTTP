#include "Socket.h"


char response[] = "HTTP/1.x 200 OK\n"
"Content-Type: text/html; charset=UTF-8\nContent-Length: 100\nConnection: close\n\n"
"<doctype !html><html><head><title>faggot</title></head>"
"<body><h1>You're a faggot.</h1></body></html>\n";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
SOCKET sock;
#else
int sock;
#endif

int SetupSocket()
{
	//char recvbuffer[512];
	struct sockaddr_in serverinfo, cli_addr;
	socklen_t sin_len = sizeof(cli_addr);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SOCKET client;
	WSADATA data;
	if(FAILED(WSAStartup(MAKEWORD(2, 2), &data)))
	{
		printf("WSAStartup failed << Windows\n");
		return 0;
	}

#else
	int client;
#endif

	memset(&serverinfo, 0, sizeof(serverinfo));

	serverinfo.sin_family = 2;
	serverinfo.sin_addr.s_addr = htonl(INADDR_ANY);
	serverinfo.sin_port = htons(80);

	if ((sock = socket(2, 1, 0)) == -1)
	{
		WSACleanup();
		return 0;
	}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	if ((bind(sock, (SOCKADDR *)&serverinfo, sizeof(serverinfo))) == -1)
	{
		WSACleanup();
#else
	if ((bind(sock, (struct sockaddr *)&serverinfo, sizeof(struct sockaddr_un))) == -1)
	{
#endif
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
		client = accept(sock, (struct sockaddr *)&cli_addr, &sin_len);

		if (client == -1)
			continue;

		//recv(sock, recvbuffer, 512, 0);

		//printf("%s", recvbuffer);

		send(client, response, strlen(response), 0);
		
		closesocket(client);
	}
	return 1;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

int SocketListen(SOCKET socket, int numConnections)
{
	if ((listen(socket, numConnections)) == -1)
	{
		WSACleanup();
		closesocket(socket);
		return 0;
	}
	return 1;
}


#else

int SocketListen(int socket, int numConnections)
{
	if ((listen(socket, numConnections)) == -1)
	{
		closesocket(socket);
		return 0;
	}
	return 1;
}

#endif

void ShutdownSocket()
{
	closesocket(sock);
	printf("Closed socket");
}
