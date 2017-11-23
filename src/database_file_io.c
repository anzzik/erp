#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "file.h"
#include "database_file_io.h"

int db_open_file(IO_t *io, void *db, char *name)
{
	int r;

	if (io->state == IO_OPEN)
	{
		lprintf(LL_ERROR, "IO already opened with filename %s", file_get_name(io->rsrc));

		return -1;
	}

	r = file_open(io->rsrc, name, "r+");
	if (r < 0)
	{
		lprintf(LL_ERROR, "file_open failed with filename %s", name);

		return -1;
	}

	return 0;
} 

int db_load_file(IO_t *io, void *db)
{
	int r;
	int f_sz;

	f_sz = file_get_size(io->rsrc);

	io->buffer = malloc(f_sz + 1);
	memset(io->buffer, '\0', f_sz + 1);

	r = file_read(io->rsrc, io->buffer, f_sz);
	if (r != f_sz)
	{
		lprintf(LL_ERROR, "file_read returned different value than file size: %d <> %d", r, f_sz);

		return -1;
	}

	io->buffer_sz = f_sz;
	io->state = IO_OPEN;

	return 0;
}

int db_save_file(IO_t *io, void *db)
{
	return 0;
} 

int db_close_file(IO_t *io, void *db)
{
	return 0;
}
