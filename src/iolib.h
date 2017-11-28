#ifndef IOLIB_H
#define IOLIB_H

enum IOState_e
{
	IO_CLOSED = 0,
	IO_OPEN,
	IO_FREED
};

enum IOType_e
{
	IO_FILEFORMAT = 0,
};

typedef struct IO_s IO_t;
typedef struct IOLib_s IOLib_t;

struct IO_s
{
	enum	IOState_e state;
	char*	buffer;
	int	buffer_sz;
	void*	rsrc;
	enum 	IOType_e type;

	IOLib_t *lib;
};

struct IOLib_s
{
	int  (*io_open)(IO_t *io, void *ptr, char *filename);
	int  (*io_read)(IO_t *io, void *ptr);
	int  (*io_write)(IO_t *io, void *ptr);
	int  (*io_close)(IO_t *io, void *ptr);
};

IO_t* io_new();
IOLib_t* iolib_new();

void io_free_buffer(IO_t *io);
void io_free(IO_t *io);

#endif // IOLIB_H
