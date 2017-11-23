#ifndef CSV_H
#define CSV_H

#include "list.h"

typedef struct CSVLine_s CSVLine_t;

struct CSVLine_s
{
	int fc;
	char **fields;

	List_t ls;
};

CSVLine_t* csv_new_line(int fc);
int csv_parse(char *buffer, int n, List_t *head);
int csv_parse_line(char *line, int n, CSVLine_t *csvl);
int csv_get_field_count(char *line, int n, char delim);

#endif
