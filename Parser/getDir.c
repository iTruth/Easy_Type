#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc != 2){
		printf("²ÎÊı´íÎó\n");
		return -1;
	}
	char dir[256] = {0};
	strcpy(dir, argv[1]);
	*strrchr(dir, '\\') = '\0';
	printf("%s", dir);
}
