#include <stdio.h>

#include "log.h"
#include "item.h"

int main(int argc, char** argv)
{
	log_add(LL_DEBUG, "debug.log");

	Item_t* item = item_new(IOTYPE_FILEFORMAT);

	item_open(item, "testfile.txt");
	item_open(item, "testfile2.txt");
//	item_load(item, "JJ");

	log_quit();

	return 0;
}
