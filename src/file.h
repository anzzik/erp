#include <stdio.h>

typedef struct File_s File_t;
struct File_s
{
	FILE *fp;
	char *filename;
	int size;
};

File_t* file_new();
int  file_open(File_t* f, char* filename, char* mode);
int  file_read(File_t* f, char* buffer, int n);
int  file_write(File_t* f, char *buffer, int n);
int  file_get_size(File_t* f);
char* file_get_name(File_t* f);
void file_close(File_t* f);
void file_free(File_t* f);

