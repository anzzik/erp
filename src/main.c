#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "iolib.h"
#include "item.h"
#include "list.h"
#include "database.h"

typedef struct Test_s Test_t;

struct Test_s
{
	int i;
	float j;
	char *k;

	List_t *ls;
};

int main(int argc, char** argv)
{
	Database_t *db;

	log_add(LL_DEBUG, "debug.log");

	db = db_new(IO_FILEFORMAT);

	db_open_io(db, "item.csv");
	db_load_io(db);

	db_process_io_buffer(db, db->io->buffer, db->io->buffer_sz);

	log_quit();

	return 0;
}

