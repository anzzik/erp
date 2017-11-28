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

int db_process_io_buffer(Database_t *db, char *buffer, int n)
{
	char line[2048];
	int i;
	int read;

	read = 0;

	while (1)
	{
		memset(line, '\0', 2048);
		sscanf(buffer + read, "%s\n%n", line, &i);

		read += i;

		__db_process_io_entry(db, line, i);

		if (read == n)
			break;
	}

	return 0;
}

int __db_process_io_entry(Database_t *db, char *entry, int n)
{
	int i;
	int j;
	int esc;
	int quote;
	char *field;

	field = malloc(n * sizeof(char));

	memset(field, '\0', n);

	i = 0;
	j = 0;
	esc = 0;
	quote = 0;

	while (1)
	{
		if (esc == 0 && entry[i] == '\\')
		{
			esc = 1;
			i++;

			continue;
		}

		if (esc == 1)
		{
			field[j] = entry[i];
			esc = 0;
		}

		if (entry[i] == '"')
		{
			if (quote == 0)
			{
				quote = 1;

				i++;

				continue;
			}
			else
			{
				quote = 0;

				i++;

				continue;
			}
		}

		if (entry[i] == ';' || entry[i] == '\0')
		{
			field[j] = '\0';
			lprintf(LL_DEBUG, "Parsed field: %s", field);

			j = 0;
			memset(field, '\0', 256);

			if (entry[i] == '\0')
			{
				break;
			}

			i++;

			continue;
		}

		field[j++] = entry[i++];
	}

	return 0;
}

void db_show_tables(Database_t *db)
{
	list_traverse(db->table_head, 0, db_table_show_cb);
}

DBTable_t* db_table_new(Database_t *db, char* name)
{
	DBTable_t *dbts;
	int len;

	dbts = malloc(sizeof(DBTable_t));

	dbts->data_head = list_new();

	len = strlen(name);
	if (len > max_table_name_sz - 1)
	{
		lprintf(LL_ERROR, "Error creating new db table, name too long");

		return 0;
	}

	memset(dbts->name, '\0', max_table_name_sz);
	strncpy(dbts->name, name, len);

	return dbts;
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
	list_add(dbt->data_head, &dbd->ls);
}

int db_table_show_cb(List_t *l, void *ptr)
{
	DBTable_t *dbts;

       	dbts = db_table_list_entry(l);

	printf("table name: %s\n", dbts->name);

	return 0;
}

int __db_import_csvdata_cb(List_t* l, void* ptr)
{
	CSVLine_t *csvl;
	Database_t *db;
	DBTable_t *dbt;
	DBDataDef_t *dbd;

	db = ptr;
	csvl = list_get_entry(l, CSVLine_t, ls);
	
	/* fields:tablename, dataname, type, length */
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

	dbd = db_datadef_new(csvl->fields[1], atoi(csvl->fields[3]));

	db_table_add_datadef(dbt, dbd);

	return 0;
}

