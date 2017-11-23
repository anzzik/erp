#include <stdlib.h>

#include "iolib.h"

IO_t* io_new()
{
	IO_t *io;

	io = malloc(sizeof(IO_t));
	io->state = IO_CLOSED;

	return io;
}

IOLib_t* iolib_new()
{
	IOLib_t *iol;

	iol = malloc(sizeof(IOLib_t));

	return iol;
}

