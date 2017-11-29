#ifndef DATABASE_H
#define DATABASE_H

#include "list.h"
#include "iolib.h"
#include "file.h"

typedef struct Database_s Database_t;
typedef struct DBTable_s DBTable_t;
typedef struct DBDataDef_s DBDataDef_t;

#define max_table_name_sz 30

enum DBDataType_e
{
	DB_INT = 0,
	DB_FIXED,
	DB_STRING,
	DB_BOOLEAN,
	DB_TYPECOUNT
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
	int cols;
	List_t *ddef_head;
	List_t *data_head;

	List_t ls;
};

struct DBDataDef_s
{
	enum DBDataType_e type;
	int sz;
	char *name;

	List_t ls;
};

struct Data_s
{
	DBDataDef_t *ddef;
	void *data;

	List_t ls;
};

Database_t* db_new();

int  db_open_io(Database_t *db, char *name, enum IOType_e io_type);
int  db_load_io(Database_t *db);
int  db_save_io(Database_t *db);
int  db_close_io(Database_t *db);
void db_show_tables(Database_t *db);
void 	    db_add_table(Database_t *db, DBTable_t *dbts);
DBTable_t*  db_get_table(Database_t *db, char *tablename);

int	    db_csv_load_defs(Database_t *db, char *csvfile);
int	  __db_csv_import_datadef_cb(List_t* l, void* ptr);

DBTable_t*  db_table_new(Database_t *db, char *name);
DBTable_t*  db_table_list_entry(List_t *l);
int 	    db_table_show_cb(List_t *l, void *ptr);
void	    db_table_add_datadef(DBTable_t *dbt, DBDataDef_t *dbd);
int	    db_table_csv_fill(Database_t *db, char *tablename, char *csvfile);
int	  __db_table_csv_import_data_cb(List_t* l, void* ptr);

DBDataDef_t*   db_datadef_new(char *name, int data_sz);

#endif

