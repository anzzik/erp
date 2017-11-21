#include <stdlib.h>

#include "iolib.h"

IO_t* io_new()
{
	IO_t *io;

	io = malloc(sizeof(IO_t));
	io->lib = malloc(sizeof(IOLib_t));

	io->state = IO_CLOSED;

	return io;
}

