#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "csv.h"

CSVLine_t* csv_new_line(int fc)
{
	CSVLine_t *csvl;

	csvl = malloc(sizeof(CSVLine_t));

	csvl->fc = fc;
	csvl->fields = malloc(fc * sizeof(char*));

	return csvl;
}

int csv_parse(char *buffer, int n, List_t *head)
{
	char line[2048];
	char **fields;
	CSVLine_t *csvl;
	int i;
	int r;
	int read;
	int fc;

	read = 0;
	fc = 0;

	while (1)
	{
		memset(line, '\0', 2048);
		sscanf(buffer + read, "%s\n%n", line, &i);

		read += i;

		if (!fc)
		{
			fc = csv_get_field_count(line, i, ';');
			fields = malloc(fc * sizeof(char*));
		}

		csvl = csv_new_line(fc);
		r = csv_parse_line(line, i, csvl);
		if (r != fc)
		{
			lprintf(LL_ERROR, "Field count (%d) doesnt match in string %s", fc, line);

			return -1;
		}

		list_add(head, &csvl->ls);

		if (read == n)
			break;
	}

	free(fields);

	return 0;
}

int csv_parse_line(char *line, int n, CSVLine_t *csvl)
{
	int line_c;
	int word_c;
	int field_c;
	int esc;
	int quote;
	char *f;

	line_c	= 0;
	word_c	= 0;
	field_c = 0;
	esc	= 0;
	quote	= 0;

	f = calloc(1, n * sizeof(char));

	while (1)
	{
		if (esc == 0 && line[line_c] == '\\')
		{
			esc = 1;
			line_c++;

			continue;
		}

		if (esc == 1)
		{
			f[word_c] = line[line_c];
			esc = 0;
		}

		if (line[line_c] == '"')
		{
			if (quote == 0)
			{
				quote = 1;
				line_c++;

				continue;
			}
			else
			{
				quote = 0;
				line_c++;

				continue;
			}
		}

		if (quote == 0 && (line[line_c] == ';' || line[line_c] == '\0'))
		{
			f[word_c] = '\0';

			csvl->fields[field_c] = calloc(1, strlen(f) + 1);

			strncpy(csvl->fields[field_c], f, strlen(f));
			field_c++;

			word_c = 0;
			memset(f, '\0', strlen(f) + 1);

			if (line[line_c] == '\0')
				break;

			line_c++;

			continue;
		}

		if (line[line_c] == '\0')
			break;

		f[word_c++] = line[line_c++];
	}

	free(f);

	return field_c;
}

int csv_get_field_count(char *line, int n, char delim)
{
	int i;
	int fc;

	fc = 1;

	for (i = 0; i < n; i++)
	{
		if (line[i] == delim)
		{
			fc++;
		}
	}

	return fc;
}

