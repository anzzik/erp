#include <stdlib.h>

#include "iolib.h"

IO_t* io_new()
{
	IO_t *io;

	io = malloc(sizeof(IO_t));
	io->state = IO_CLOSED;

	return io;
}

void io_free_buffer(IO_t *io)
{
	free(io->buffer);
	io->buffer_sz = 0;
	io->state = IO_FREED;
}

IOLib_t* iolib_new()
{
	IOLib_t *iol;

	iol = malloc(sizeof(IOLib_t));

	return iol;
}

void io_free(IO_t *io)
{
	io_free_buffer(io);
	free(io);
}

