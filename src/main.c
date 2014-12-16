#include "Socket.h"

int main()
{
	if (!SetupSocket())
	{
		printf("failed setting up socket");
		return 0;
	}
	return 0;
}
