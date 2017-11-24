#ifndef ITEM_H
#define ITEM_H

#include "iolib.h"
#include "list.h"

enum ItemType_e
{
	PRODUCT = 0
};

enum ItemData_e
{
	ITEM_ID = 0,
	ITEM_NAME,
	ITEM_PROPERTY,
	ITEM_DATACOUNT
};

typedef struct Item_s Item_t;
struct Item_s
{
	char *id;
	char *name;
	char *property;

	void *data[ITEM_DATACOUNT];

	List_t ls;
};

Item_t* item_new();

int item_open(Item_t* it, char *name);
void item_print_buffer(Item_t* it);
int __item_import_csvdata_cb(List_t* ptr, void* list_head);
void item_load(Item_t* it);
void item_save(Item_t* it);
void item_close(Item_t* it);
void item_free(Item_t* it);

#endif // ITEM_H
