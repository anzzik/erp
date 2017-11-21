#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "log.h"
#include "file.h"

File_t* file_new()
{
	File_t* f = malloc(sizeof(File_t));

	f->size = 0;
	f->filename = 0;

	return f;
}

int file_open(File_t* f, char* filename, char* mode)
{
	f->fp = fopen(filename, mode);
	if (f->fp == NULL)
	{
		lprintf(LL_ERROR, "Error in fopen, filename: %s", filename);

		return -1;
	}

	f->filename = malloc((strlen(filename) + 1) + 1);
	memset(f->filename, '\0', (strlen(filename) + 1));
	strncpy(f->filename, filename, strlen(filename));

	fseek(f->fp, 0, SEEK_END);
	f->size = ftell(f->fp);
	rewind(f->fp);

	return 0;
}

int file_read(File_t* f, char* buffer, int n)
{
	int r;

	memset(buffer, '\0', n + 1);

	r = fread(buffer, 1, f->size, f->fp);
	if (r != f->size)
	{
		lprintf(LL_ERROR, "Error in fread, filename: %s", f->filename);

		return -1;
	}

	return r;
}

int file_get_size(File_t* f)
{
	return f->size;
}

void file_save(File_t* f)
{

}

void file_close(File_t* f)
{
	fclose(f->fp);
}

void file_free(File_t* f)
{
	free(f);
}
