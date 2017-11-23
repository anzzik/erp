#ifndef DATABASE_H
#define DATABASE_H

#include "list.h"
#include "iolib.h"
#include "file.h"

typedef struct Database_s Database_t;
typedef struct DBTable_s DBTable_t;
typedef struct DBData_s DBData_t;

#define max_table_name_sz 30

enum DBDataType_e
{
	INT = 0,
	FIXED,
	STRING,
	BOOLEAN,
	TYPECOUNT
};

struct Database_s
{
	IO_t *io;
	IOLib_t *iolib;

	List_t *table_head;

	List_t ls;
};

struct DBTable_s
{
	char name[max_table_name_sz];
	List_t *data_head;

	List_t ls;
};

struct DBData_s
{
	enum DBDataType_e type;
	int sz;
	void *data;

	List_t ls;
};

Database_t* db_new();

int db_open_io(Database_t *db, char *name);
int db_load_io(Database_t *db);
int db_save_io(Database_t *db);
int db_close_io(Database_t *db);

int db_process_io_buffer(Database_t *db, char *buffer, int n);
int __db_process_io_entry(Database_t *db, char *entry, int n);
void db_show_tables(Database_t *db);

DBTable_t*  db_table_new(Database_t *db, char *name);
void 	    db_table_add(Database_t *db, DBTable_t *dbts);
DBTable_t*  db_table_list_entry(List_t *l);
void 	    db_table_show_cb(List_t *l);

#endif

