#ifndef ITEM_H
#define ITEM_H

#include "iolib.h"

enum ItemType_e
{
	PRODUCT = 0
};

struct Item_s
{
	char id[30];
	enum ItemType_e type;
	enum ItemIOType_e io_t;
	IOLib_t *io;
};

typedef struct Item_s Item_t;

#endif // ITEM_H
