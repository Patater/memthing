#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define USAGE "(r or w) hex-address (hex-value if write)"

#define GENERIC_WORD_WINDOW_SIZE 0x4
#define PAGE_SIZE 4096

int main(int argc, char *argv[])
{
	int status;
	int fd;

	if (argc <= 2) {
		printf("Usage: %s %s\n", argv[0], USAGE);
		return 0;
	}

	/* Read or write? */
	bool read_not_write = true;
	switch (argv[1][0]) {
	case 'r':
		read_not_write = true;
		break;
	case 'w':
		read_not_write = false;
		if (argc > 3)
			break;
	default:
		printf("Usage: %s %s\n", argv[0], USAGE);
		return 0;
	}

	/* Which address? */
	uintptr_t address;
	status = sscanf(argv[2], "%zx", &address);
	if (status != 1) {
		printf("Usage: %s %s\n", argv[0], USAGE);
		return 1;
	}
	/* Compute the base address for the page. */
	uintptr_t base = address / PAGE_SIZE * PAGE_SIZE;
	/* Compute the offset within the page. */
	uintptr_t offset = address % PAGE_SIZE;
	printf("address: 0x%08zx\n", address);
	printf("base: 0x%08zx\n", base);
	printf("offset: 0x%08zx\n", offset);


	/* Value (if write)? */
	uint32_t value;
	if (!read_not_write) {
		status = sscanf(argv[3], "%x", &value);
		if (status != 1) {
			printf("Usage: %s %s\n", argv[0], USAGE);
			return 1;
		}
		printf("value: 0x%08zx\n", address);
	}

	if ((fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0 ) {
		printf("Error opening file. \n");
		close(fd);
		return 1;
	}

	uint32_t *memory;
	memory = mmap(0, PAGE_SIZE,
		      PROT_READ | (read_not_write ? 0 : PROT_WRITE),
		      MAP_PRIVATE, fd, base);
	printf("private memory: %p\n", memory);
	if (memory == (uint32_t *)~0)
	{
		printf("Error mapping memory\n");
		return 1;
	}

	printf("value: 0x%08x\n", memory[offset / sizeof(*memory)]);
	if (!read_not_write)
	{
		memory[offset / sizeof(*memory)] = value;
		printf("wrote: 0x%08x\n", value);
	}

	return 0;
}
