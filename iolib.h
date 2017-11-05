#ifndef IOLIB_H
#define IOLIB_H

enum ItemIOType_e
{
	FILEFORMAT = 0,
};

typedef struct IOLib_s IOLib_t;

struct IOLib_s
{
	void (*io_open)();
	void (*io_load)();
	void (*io_save)();
};

#endif // IOLIB_H
