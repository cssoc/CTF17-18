#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	char inp_buf[512];
	char *buf = malloc(64);
	FILE *file = fopen("flag.txt", "r");

	fgets(buf, 64, file);
	fclose(file);
	puts("IN ORDER TO OBTAIN THE FLAG\nYOU FIRST NEED TO KNOW THE FLAG");
	fflush(stdout);

	while(1) {
		puts("WHAT IS THE FLAG?????");
		fflush(stdout);
		fgets(inp_buf, sizeof(inp_buf), stdin);
		puts("THE FLAG IS SUPPOSED TO BE:");
		printf(inp_buf);
		puts("??????");
		fflush(stdout);
		if(strcmp(inp_buf, buf)) {
			puts("YOU ARE WRONG");
			fflush(stdout);
		} else {
			puts("GREAT THAT WAS THE FLAG\nI COULD GIVE YOU THE FLAG NOW BUT THERE'S NO POINT IN DOING THAT SINCE YOU ALREADY KNOW IT");
			fflush(stdout);
			break;
		}
	}

}
