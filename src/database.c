#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "database_file_io.h"
#include "log.h"

Database_t* db_new(enum IOType_e io_type)
{
	Database_t *db;

	db = malloc(sizeof(Database_t));

	db->io = io_new();
	db->io->type = io_type;

	db->iolib = iolib_new();

	db->table_head = list_new();

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

	return db;
}

int db_open_io(Database_t *db, char *name)
{
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
	list_traverse(db->table_head, db_table_show_cb);
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

void db_table_add(Database_t *db, DBTable_t *dbts)
{
	list_add(db->table_head, (List_t*)&dbts->ls);
}

DBTable_t* db_table_list_entry(List_t *l)
{
       	return list_get_entry(l, DBTable_t, ls);
}

void db_table_show_cb(List_t *l)
{
	DBTable_t *dbts;

       	dbts = db_table_list_entry(l);

	printf("table name: %s\n", dbts->name);
}

