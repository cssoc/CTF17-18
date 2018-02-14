#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define flag "CSSOC{Backtr4c3d_17}"
#define flag_len 20

int main(int argc, char** argv)
{
	char input [256];
	char str [] = {-68, -84, -84, -80, -68, -124, -67, -98,
		-100, -108, -117, -115, -53, -100, -52, -101,
		-96, -50, -56, -126, -11};

	int x;
	for(x = 0; x < flag_len + 1; x++){
		str[x] = ~str[x];
	}
	
	while(1) {
		puts("Gibe password plox!!!");
		fgets(input, 256, stdin);
		if(strcmp(input, str))
			puts("Wrong input");
		else{
			puts("Correct input!");
			exit(0);
		}

	}

}
