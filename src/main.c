#include <stdio.h>

#include "log.h"
#include "item.h"

int main(int argc, char** argv)
{
	int r;

	log_add(LL_DEBUG, "debug.log");

	Item_t* item = item_new(IOTYPE_FILEFORMAT);

	r = item_open(item, "item.dat");
	if (r < 0)
	{
		lprintf(LL_ERROR, "item_open failed");

		return -1;
	}
	
	item_print_buffer(item);
	item_close(item);

	log_quit();

	return 0;
}
