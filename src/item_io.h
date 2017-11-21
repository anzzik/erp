#ifndef ITEM_IO_H
#define ITEM_IO_H

#include "iolib.h"

void item_set_io(IO_t* io, enum ItemIOType_e io_t);

int item_file_open(IO_t* io, char *filename);
void item_file_print(IO_t* io);
int item_file_load(IO_t* io);
int item_file_save(IO_t* io);
void item_file_parse_buffer(IO_t* io);
void item_file_parse_line(char *line);
void item_file_close(IO_t* io);

#endif // ITEM_IO_H
