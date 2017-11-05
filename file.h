#include <stdio.h>

typedef struct File_s File_t;

struct File_s
{
	FILE *fp;
	char* content;
};
