#include <stdio.h>
#include <stdlib.h>

#include "list.h"

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

void list_traverse(List_t *head, void (*f_cb)(List_t*))
{
	List_t *it;

	for (it = head->next; it != 0; it = it->next)
	{
		f_cb(it);
	}
}

List_t* list_get_first(List_t *head)
{
	return head->next;
}

