#ifndef ITEM_H
#define ITEM_H

#include "iolib.h"

enum ItemType_e
{
	PRODUCT = 0
};

typedef struct Item_s Item_t;
struct Item_s
{
	char id[30];
	char name[200];
	char property[200];

	enum ItemType_e type;
	IO_t *io;
};

Item_t* item_new(enum ItemIOType_e io_type);

int item_open(Item_t* it, char *name);
void item_print_buffer(Item_t* it);
void item_load(Item_t* it, char* id);
void item_save(Item_t* it);
void item_close(Item_t* it);
void item_free(Item_t* it);

#endif // ITEM_H
