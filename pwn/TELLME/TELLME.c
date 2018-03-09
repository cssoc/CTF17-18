#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	puts("JUST TELL ME WHAT TO DO!!!!111111");
	fflush(stdout);

	void (*data)(void) = mmap(NULL, 4096, PROT_EXEC | PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	read(1, (void *)data, 4096);
	data();

	return 0;
}
