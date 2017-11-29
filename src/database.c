#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "database_file_io.h"
#include "log.h"
#include "csv.h"

Database_t* db_new()
{
	Database_t *db;

	db = malloc(sizeof(Database_t));
	db->table_head = list_new();

	return db;
}

int db_open_io(Database_t *db, char *name, enum IOType_e io_type)
{
	db->io = io_new();
	db->io->type = io_type;
	db->iolib = iolib_new();

	switch (db->io->type)
	{
		case IO_FILEFORMAT:
			db->iolib->io_open  = db_open_file;
			db->iolib->io_read  = db_load_file;
			db->iolib->io_write = db_save_file;
			db->iolib->io_close = db_close_file;

			db->io->rsrc = file_new();

			break;
	};

	return db->iolib->io_open(db->io, db, name);
}

int db_load_io(Database_t *db)
{
	return db->iolib->io_read(db->io, db);
}

int db_save_io(Database_t *db)
{
	return db->iolib->io_write(db->io, db);
}

int db_close_io(Database_t *db)
{
	return db->iolib->io_close(db->io, db);
}

void db_show_tables(Database_t *db)
{
	list_traverse(db->table_head, 0, db_table_show_cb);
}

DBTable_t* db_table_new(Database_t *db, char* name)
{
	DBTable_t *dbt;
	int len;

	dbt = malloc(sizeof(DBTable_t));

	dbt->cols = 0;
	dbt->ddef_head = list_new();
	dbt->data_head = list_new();

	len = strlen(name);
	if (len > max_table_name_sz - 1)
	{
		lprintf(LL_ERROR, "Error creating new db table, name too long");

		return 0;
	}

	memset(dbt->name, '\0', max_table_name_sz);
	strncpy(dbt->name, name, len);

	return dbt;
}

DBDataDef_t* db_datadef_new(char *name, int data_sz)
{
	DBDataDef_t *dbd = malloc(sizeof(DBDataDef_t));
	dbd->name = calloc(1, strlen(name) + 1);

	strncpy(dbd->name, name, strlen(name));

	return dbd;
}

void db_add_table(Database_t *db, DBTable_t *dbts)
{
	list_add(db->table_head, &dbts->ls);
}

DBTable_t* db_get_table(Database_t *db, char *tablename)
{
	DBTable_t *dbt;
	List_t *it;
	DBTable_t *r;

	r = 0;
	it = db->table_head->next;

	while (1)
	{
		if (it == 0)
			break;

		dbt = list_get_entry(it, DBTable_t, ls);

		if (strcmp(dbt->name, tablename) == 0)
		{
			r = dbt;

			break;
		}

		it = it->next;
	}

	return r;
}

DBTable_t* db_table_list_entry(List_t *l)
{
       	return list_get_entry(l, DBTable_t, ls);
}

void db_table_add_datadef(DBTable_t *dbt, DBDataDef_t *dbd)
{
	list_add(dbt->ddef_head, &dbd->ls);

	dbt->cols++;
}

int db_table_show_cb(List_t *l, void *ptr)
{
	DBTable_t *dbts;

       	dbts = db_table_list_entry(l);

	printf("table name: %s\n", dbts->name);

	return 0;
}

int db_csv_load_defs(Database_t *db, char *csvfile)
{
	List_t *csv_head;
	int r;

	csv_head = list_new();

	db_open_io(db, csvfile, IO_FILEFORMAT);
	db_load_io(db);

	r = csv_parse(db->io->buffer, db->io->buffer_sz, csv_head);
	if (r != 0)
	{
		lprintf(LL_ERROR, "csv_parse failed");

		return -1;
	}

	list_traverse(csv_head, db, __db_csv_import_datadef_cb);

	db_close_io(db);

	return 0;
}

int db_table_csv_fill(Database_t *db, char *tablename, char *csvfile)
{
	DBTable_t *dbt;
	List_t *csv_head;
	int r;

	csv_head = list_new();
	dbt = db_get_table(db, tablename);

	db_open_io(db, csvfile, IO_FILEFORMAT);
	db_load_io(db);

	r = csv_parse(db->io->buffer, db->io->buffer_sz, csv_head);
	if (r != 0)
	{
		lprintf(LL_ERROR, "csv_parse failed");

		return -1;
	}

	db_close_io(db);

	list_traverse(csv_head, dbt, __db_table_csv_import_data_cb);

	return 0;
}

int __db_csv_import_datadef_cb(List_t* l, void* ptr)
{
	CSVLine_t *csvl;
	Database_t *db;
	DBTable_t *dbt;
	DBDataDef_t *dbd;

	db = ptr;
	csvl = list_get_entry(l, CSVLine_t, ls);
	
	if (csvl->fc != 4)
	{
		lprintf(LL_ERROR, "Wrong number of fields in the csv row (%d)", csvl->fc);

		return -1;
	}

	dbt = db_get_table(db, csvl->fields[0]);
	if (!dbt)
	{
		dbt = db_table_new(db, csvl->fields[0]);

		db_add_table(db, dbt);
	}

	/* fields: 0:tablename, 1:dataname, 2:type, 3:length */

	dbd = db_datadef_new(csvl->fields[1], atoi(csvl->fields[3]));

	db_table_add_datadef(dbt, dbd);

	return 0;
}


int __db_table_csv_import_data_cb(List_t* l, void* ptr)
{
	CSVLine_t *csvl;
	DBTable_t *dbt;
	DBDataDef_t *ddef;
	List_t *it;
	int i;

	dbt = ptr;
	csvl = list_get_entry(l, CSVLine_t, ls);

	if (csvl->fc != dbt->cols)
	{
		lprintf(LL_ERROR, "Field count doesn't match to the table definition (%d <> %d)", csvl->fc, dbt->cols);

		return -1;
	}

	it = dbt->ddef_head->next;
	for (i = 0; i < csvl->fc; i++)
	{
		if (it == 0)
		{
			lprintf(LL_DEBUG, "it is 0");
			break;
		}

		ddef = list_get_entry(it, DBDataDef_t, ls);

		lprintf(LL_DEBUG, "def: %s, data: %s", ddef->name, csvl->fields[i]);

		it = it->next;
	}

	return 0;
}

