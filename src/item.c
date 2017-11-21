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

int item_open(Item_t* it, char *name)
{
	return it->io->lib->io_open(it->io, name);
}

void item_print_buffer(Item_t* it)
{
//	item_file_print(it->io);
	item_file_parse_buffer(it->io);
}

void item_load(Item_t* it, char* id)
{
	it->io->lib->io_load(it->io);
}

void item_save(Item_t* it)
{
	it->io->lib->io_save(it->io);
}

void item_close(Item_t* it)
{
	it->io->lib->io_close(it->io);
}

void item_free(Item_t* it)
{
	free(it);
}

