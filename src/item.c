#include <stdlib.h>

#include "item.h"

Item_t* item_new()
{
	Item_t *it;

	it = malloc(sizeof(Item_t));

	return it;
}

int item_open(Item_t* it, char *name)
{
	return 0;
}

void item_print_buffer(Item_t* it)
{
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

