#ifndef ITEM_IO_H
#define ITEM_IO_H

#include "iolib.h"

void item_set_io(IO_t* io, enum ItemIOType_e io_t);

int item_file_open();
int item_file_load();
int item_file_save();
void item_file_close();

#endif // ITEM_IO_H
