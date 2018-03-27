#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const char *commands[] = {"date", "ps", "df"};

int main(int argc, char **argv)
{
	char buf[100];

	while(1){
		puts("Please enter one of the following commands:\ndate\nps\ndf\nquit");
		fflush(stdout);
	
		int len = read(1, buf, 0x100);

		if(len)
			buf[len - 1] = '\0';
		else
			break;

		if(!strcmp(buf, "quit"))
			break;

		for(int i = 0; i < 3; i++){
			if(!strcmp(commands[i], buf)){
				system(buf);
				fflush(stdout);
				}
		}
	}
	return 0;
}
