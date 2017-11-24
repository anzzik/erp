#include <stdlib.h>
#include <string.h>

#include "item.h"
#include "log.h"
#include "csv.h"

Item_t* item_new()
{
	Item_t *it;

	it = calloc(1, sizeof(Item_t));

	return it;
}

int item_open(Item_t* it, char *name)
{
	return 0;
}

void item_print_buffer(Item_t* it)
{
}

int __item_import_csvdata_cb(List_t* ptr, void* list_head)
{
	CSVLine_t *csvl;
	Item_t *it;
	List_t *item_head;

	item_head = list_head;
	csvl = list_get_entry(ptr, CSVLine_t, ls);

	if (csvl->fc != ITEM_DATACOUNT)
	{
		lprintf(LL_ERROR, "CSV line has wrong number of fields");

		return -1;
	}

	it = item_new();

	it->id = calloc(1, strlen(csvl->fields[0]) + 1);
	memcpy(it->id, csvl->fields[0], strlen(csvl->fields[0]));

	it->name = calloc(1, strlen(csvl->fields[1]) + 1);
	memcpy(it->name, csvl->fields[1], strlen(csvl->fields[1]));

	it->property = calloc(1, strlen(csvl->fields[2]) + 1);
	memcpy(it->property, csvl->fields[2], strlen(csvl->fields[2]));

	list_add(item_head, &it->ls);

	return 0;
}

void item_load(Item_t* it)
{
}

void item_save(Item_t* it)
{
}

void item_close(Item_t* it)
{
}

void item_free(Item_t* it)
{
}

