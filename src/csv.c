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
		csv_parse_line(line, i, csvl);

		list_add(head, &csvl->ls);

		if (read == n)
			break;
	}

	free(fields);

	return 0;
}

int csv_parse_line(char *line, int n, CSVLine_t *csvl)
{
	int i;
	int j;
	int k;
	int esc;
	int quote;
	char *f;

	f = malloc(n * sizeof(char));

	memset(f, '\0', n);

	i = 0;
	j = 0;
	k = 0;
	esc = 0;
	quote = 0;

	while (1)
	{
		if (esc == 0 && line[i] == '\\')
		{
			esc = 1;
			i++;

			continue;
		}

		if (esc == 1)
		{
			f[j] = line[i];
			esc = 0;
		}

		if (line[i] == '"')
		{
			if (quote == 0)
			{
				quote = 1;

				i++;

				continue;
			}
			else
			{
				quote = 0;

				i++;

				continue;
			}
		}

		if (line[i] == ';' || line[i] == '\0')
		{
			f[j] = '\0';
			lprintf(LL_DEBUG, "Parsed field: %s", f);

			csvl->fields[k] = malloc(strlen(f) + 1);
			memset(csvl->fields[k], '\0', strlen(f) + 1);

			strncpy(csvl->fields[k], f, strlen(f));
			k++;

			j = 0;
			memset(f, '\0', strlen(f) + 1);

			if (line[i] == '\0')
			{
				break;
			}

			i++;

			continue;
		}

		f[j++] = line[i++];
	}

	free(f);

	return 0;
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

