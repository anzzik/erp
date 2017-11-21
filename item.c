#include <stdlib.h>

#include "item.h"
#include "item_io.h"

Item_t* item_new(enum ItemIOType_e io_type)
{
	Item_t *it;

	it = malloc(sizeof(Item_t));
	it->io = io_new();

	item_set_io(it->io, io_type);

	return it;
}

void item_open(Item_t* it, char *name)
{
	it->io->lib->io_open(it->io, name);
}

void item_load(Item_t* it, char* id)
{
	it->io->lib->io_load(it->io);
}

void item_save(Item_t* it)
{
	it->io->lib->io_save(it->io);
}

void item_Close(Item_t* it)
{
	it->io->lib->io_close(it->io);
}

void item_free(Item_t* it)
{
	free(it);
}

