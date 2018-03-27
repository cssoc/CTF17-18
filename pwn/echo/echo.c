#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
	char inp[0x100];

	puts("ECHO SERVICE");
	puts("q to quit");
	fflush(stdout);

	unsigned char len;
	do {
	len = read(1, inp, 0xff);
	inp[len] = '\0';
	
	printf(inp);
	fflush(stdout);

	} while(len && strcmp(inp, "q"));
	
	exit(0);
}
