#ifndef IOLIB_H
#define IOLIB_H

enum IOState_e
{
	IO_CLOSED = 0,
	IO_OPEN
};

enum ItemIOType_e
{
	IOTYPE_FILEFORMAT = 0,
};

typedef struct IOLib_s IOLib_t;
struct IOLib_s
{
	int  (*io_open)();
	int (*io_load)();
	int (*io_save)();
	void (*io_close)();
};

typedef struct IO_s IO_t;
struct IO_s
{
	enum	IOState_e state;
	char*	buffer;
	void*	rsrc;
	enum ItemIOType_e type;

	IOLib_t *lib;
};

IO_t* io_new();

#endif // IOLIB_H
