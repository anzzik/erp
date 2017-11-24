#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "log.h"

List_t* list_new()
{
	List_t *l;

	l = malloc(sizeof(List_t));
	
	l->next = 0;
	l->prev = 0;

	return l;
}

void list_add(List_t* head, List_t *l)
{
	if (head->next != 0)
		head->next->prev = l;

	l->next = head->next;
	l->prev = head;
	head->next = l;
}

void list_remove(List_t* head, List_t *l)
{
	if (l->next != 0)
		l->next->prev = l->prev;

	l->prev->next = l->next;
}

int list_traverse(List_t *head, void *ptr, int (*f_cb)(List_t*, void*))
{
	List_t *l;
	int r;

	for (l = head->next; l != 0; l = l->next)
	{
		r = f_cb(l, ptr);
		if (r != 0)
		{
			lprintf(LL_ERROR, "list_traverse failed with error code %d", r);

			return -1;
		}
	}

	return 0;
}

List_t* list_get_first(List_t *head)
{
	return head->next;
}

