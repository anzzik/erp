#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct List_s List_t;
struct List_s
{
	List_t *next;
	List_t *prev;
};

#define CONTAINER(ptr, type, member) \
	(type*)((char*)ptr - offsetof(type, member))

#define list_get_entry(ptr, type, member) \
	CONTAINER(ptr, type, member)

List_t* list_new();

void list_init(List_t* head);
void list_add(List_t* head, List_t *l);
void list_remove(List_t* head, List_t *l);
void list_empty(List_t *head);
int list_traverse(List_t *head, void *ptr, int (*f_cb)(List_t*, void*));
List_t* list_get_first(List_t *head);

#endif

