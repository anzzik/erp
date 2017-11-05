#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file.h"

File_t* file_new()
{
	File_t* f = malloc(sizeof(File_t));

	return f;
}

void file_open(File_t* f, char* filename, char* mode)
{
	f->fp = fopen(filename, mode);
}

int file_read(File_t* f, char* buffer, int n)
{
	int r;

	r = read(fileno(f->fp), buffer, n);
	if (r < 0)
	{
		return -1;
	}

	if (r != n)
	{

	}

	return n;
}

int file_get_size(File_t* f)
{
    int sz;
    fseek(f->fp, 0L, SEEK_END);
    sz = ftell(f->fp);
    fseek(f->fp, 0L, SEEK_SET);

    return sz;
}

void file_rewind(File_t* f)
{
    rewind(f->fp);
}

void file_save(File_t* f)
{

}

void file_free(File_t* f)
{
    free(f);
}
