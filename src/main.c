#include "Socket.h"

int main()
{
	char response =0;

	if (!SetupSocket())
	{
		printf("failed setting up socket");
		return 0;
	}
	scanf("%d", response);

	if (response == 69)
	{
		ShutdownSocket();
	}
	return 0;
}