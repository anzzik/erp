#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "csv.h"
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

int test(List_t* l, void* ptr)
{
	Item_t *it;

	it = list_get_entry(l, Item_t, ls);

	lprintf(LL_DEBUG, "Item:");
	lprintf(LL_DEBUG, "Id: %s", it->id);
	lprintf(LL_DEBUG, "Name: %s", it->name);
	lprintf(LL_DEBUG, "Property: %s", it->property);

	return 0;
}

int main(int argc, char** argv)
{
	Database_t *db;
	List_t *csv_head;
	List_t *item_head;

	csv_head = list_new();
	item_head = list_new();

	log_add(LL_DEBUG, "debug.log");

	db = db_new();

	db_open_io(db, "table_def.csv", IO_FILEFORMAT);
	db_load_io(db);

	csv_parse(db->io->buffer, db->io->buffer_sz, csv_head);

	list_traverse(csv_head, db, __db_import_csvdata_cb);

	db_close_io(db);


	list_empty(csv_head);


	db_open_io(db, "item.csv", IO_FILEFORMAT);
	db_load_io(db);

	csv_parse(db->io->buffer, db->io->buffer_sz, csv_head);

	db_close_io(db);

	list_traverse(csv_head, item_head, __item_import_csvdata_cb);
	list_traverse(item_head, 0, test);

	log_quit();

	return 0;
}


