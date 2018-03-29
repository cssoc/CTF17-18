#include <stdio.h>
#include <string.h>

void flag(void)
{
	char buf[1024];

	FILE *file = fopen("flag.txt", "r");
	fgets(buf, sizeof(buf), file);
	fputs(buf, stdout);
	fflush(stdout);
}

int main(int argc, char **argv)
{	
	char buf[1024];
	puts("This is a simple echo server.");
	fflush(stdout);
	gets(buf);
	puts(buf);
}

