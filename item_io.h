#ifndef ITEM_IO_H
#define ITEM_IO_H

#include "iolib.h"

void item_set_iolib(IOLib_t* io_lib, enum ItemIOType_e io_t);

void item_file_open();
void item_file_load();
void item_file_save();

#endif // ITEM_IO_H
