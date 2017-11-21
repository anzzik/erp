#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "item_io.h"
#include "file.h"

void item_set_io(IO_t* io, enum ItemIOType_e io_type)
{
	switch (io_type)
	{
		case IOTYPE_FILEFORMAT:
			io->lib->io_open  = item_file_open;
			io->lib->io_load  = item_file_load;
			io->lib->io_save  = item_file_save;
			io->lib->io_close = item_file_close;

			io->rsrc = file_new();
			io->type = IOTYPE_FILEFORMAT;

			break;
	}
}

int item_file_open(IO_t* io, char *filename)
{
	size_t f_sz;
	int r;

	if (io->state == IO_OPEN)
	{
		lprintf(LL_ERROR, "IO already opened with filename %s", file_get_name(io->rsrc));

		return -1;
	}

	r = file_open(io->rsrc, filename, "r+");
	if (r < 0)
	{
		lprintf(LL_ERROR, "file_open failed with filename %s", filename);

		return -1;
	}

	f_sz = file_get_size(io->rsrc);

	io->buffer = malloc(f_sz + 1);
	memset(io->buffer, '\0', f_sz + 1);

	r = file_read(io->rsrc, io->buffer, f_sz);
	if (r != f_sz)
	{
		lprintf(LL_ERROR, "file_read returned different value than file size: %d <> %d", r, f_sz);

		return -1;
	}

	lprintf(LL_DEBUG, "Opening file IO: %s", filename);

	io->state = IO_OPEN;

	return 0;
}

void item_file_print(IO_t* io)
{
	lprintf(LL_DEBUG, "Buffer:\n%s", io->buffer);
}

int item_file_load(IO_t* io)
{
	if (io->state != IO_OPEN)
	{
		lprintf(LL_ERROR, "IO not opened on load");

		return -1;
	}

	return 0;
}

int item_file_save(IO_t* io)
{
	if (io->state != IO_OPEN)
	{
		lprintf(LL_ERROR, "IO not opened on save");

		return -1;
	}

	return 0;
}

void item_file_parse_buffer(IO_t* io)
{
	char id[255];
	char name[255];
	char property[255];

	memset(id, '\0', 255);
	memset(name, '\0', 255);
	memset(property, '\0', 255);

	sscanf(io->buffer, "%s%s%s\n", id, name, property);

	lprintf(LL_DEBUG, "parse: %s ; %s ; %s", id, name, property);
}

void item_file_close(IO_t* io)
{
	if (io->state == IO_CLOSED)
	{
		lprintf(LL_ERROR, "IO already closed");

		return;
	}

	lprintf(LL_DEBUG, "Closing file IO: %s", file_get_name(io->rsrc));

	file_close(io->rsrc);

	io->state = IO_CLOSED;
}



