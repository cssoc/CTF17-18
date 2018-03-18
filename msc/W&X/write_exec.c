#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>

char check_len(char *);

	unsigned int sizes[] = {0x1c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x14, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1a};

int main(int argc, char **argv)
{
	puts("GO ON... TELL ME THE FLAG");

	char buf[0x100];
	fgets(buf, 0x100, stdin);
	int len = strlen(buf) - 1;
	buf[len] = '\0';

	int i = 0;	

	char xor = buf[0];

	char *ptr = (char *)check_len;
	char (*fun)(char *) = check_len;

	unsigned long int boundary = (unsigned long int) check_len;
	boundary = boundary & ~0xfff;

	mprotect((void *)boundary , 0x100, PROT_WRITE | PROT_READ | PROT_EXEC);

	while(1) {
		for(unsigned int ctr = 0; ctr < sizes[i]; ctr++) {
			ptr[ctr] = ptr[ctr] ^ xor;
		}
		
		if( ptr[0] == 0x55)
			xor = fun(buf);
		else{
			puts("NOT ACCEPTED");
			exit(1);
		}
		ptr += sizes[i++];
		fun = (char(*)(char *))ptr;
	}
}
