#ifndef DATABASE_FILE_IO
#define DATABASE_FILE_IO

#include "iolib.h"

int db_open_file(IO_t *io, void *db, char *name);
int db_load_file(IO_t *io, void *db);
int db_save_file(IO_t *io, void *db);
int db_close_file(IO_t *io, void *db);

#endif
