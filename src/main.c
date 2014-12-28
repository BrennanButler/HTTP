#include "Socket.h"

int main()
{
<<<<<<< HEAD
	printf("%d", SetupSocket());
	getchar();
=======
	if (!SetupSocket())
	{
		printf("failed setting up socket");
		return 0;
	}
>>>>>>> origin/master
	return 0;
}
