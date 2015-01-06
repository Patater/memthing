#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/io.h>
#include <unistd.h>

#define USAGE "(r or w) hex-port (hex-value if write)"

int main(int argc, char *argv[])
{
	int status;

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

	/* Which port? */
	uint16_t port;
	status = sscanf(argv[2], "%x", &port);
	if (status != 1) {
		printf("Usage: %s %s\n", argv[0], USAGE);
		return 1;
	}
	printf("io port: 0x%04x\n", port);

	/* Enable permissions to the port. */
	status = ioperm(port, 1, 1);
	if (status != 0)
	{
		printf("No permissions\n");
		return 1;
	}

	/* Value (if write)? */
	uint32_t value;
	if (!read_not_write) {
		status = sscanf(argv[3], "%x", &value);
		if (status != 1) {
			printf("Usage: %s %s\n", argv[0], USAGE);
			return 1;
		}
		printf("value: 0x%08x\n", value);
	}

	if (read_not_write) {
		uint8_t value = inb(port);
		printf("value: 0x%02x\n", value);
	} else {
		outb(value, port);
		printf("wrote: 0x%02x\n", value);
	}

	return 0;
}
