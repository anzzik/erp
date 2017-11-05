#include <stdlib.h>

#include "item.h"
#include "item_io.h"

Item_t* item_new(enum ItemIOType_e io_t)
{
	Item_t *it = malloc(sizeof(Item_t));

	item_set_iolib(it->io, io_t);
	it->io_t = io_t;

	return it;
}

void item_load(Item_t* it)
{
	it->io->io_load();
}

void item_save(Item_t* it)
{
	it->io->io_save();
}

void item_free(Item_t* it)
{
	free(it);
}
