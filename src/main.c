#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "csv.h"
#include "iolib.h"
#include "item.h"
#include "list.h"
#include "database.h"

int main(int argc, char** argv)
{
	Database_t *db;

	log_add(LL_DEBUG, "debug.log");

	db = db_new();

	db_csv_load_defs(db, "table_def.csv");
	db_table_csv_fill(db, "item", "item.csv");

	log_quit();

	return 0;
}


