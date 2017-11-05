#include "item_io.h"

void item_set_iolib(IOLib_t* io_lib, enum ItemIOType_e io_t)
{
	switch (io_t)
	{
	case FILEFORMAT:
		io_lib->io_open = item_file_open;
		io_lib->io_load = item_file_load;
		io_lib->io_save = item_file_save;

		break;
    }
}

void item_file_open()
{

}

void item_file_load()
{

}

void item_file_save()
{

}
