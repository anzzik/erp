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

void test(List_t* l)
{
	CSVLine_t *csvl;
	int i;

	csvl = list_get_entry(l, CSVLine_t, ls);

	printf("Fields: ");

	for (i = 0; i < csvl->fc; i++)
	{
		printf("%s, ", csvl->fields[i]);
	}

	printf("\n");
}

int main(int argc, char** argv)
{
	Database_t *db;
	List_t *csv_head;

	csv_head = list_new();

	log_add(LL_DEBUG, "debug.log");

	db = db_new(IO_FILEFORMAT);

	db_open_io(db, "item.csv");
	db_load_io(db);

	csv_parse(db->io->buffer, db->io->buffer_sz, csv_head);

	list_traverse(csv_head, test);

	log_quit();

	return 0;
}


