
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef void (*func_t)(void);

int main(int argc, char *argv[])
{
	int fd = open(argv[1], O_RDONLY);
	func_t payload = mmap(NULL, 0x1000, PROT_READ|PROT_EXEC, MAP_PRIVATE, fd, 0);
	payload();
}

