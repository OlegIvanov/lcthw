#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i = 0;
	char *c = NULL;

	while(i < 100) {
		usleep(3000);
		
		if(i == 2) {
			*c = 100;
		}

		i++;
	}

	return 0;
}
