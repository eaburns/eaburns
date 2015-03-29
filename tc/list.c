#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

List *
list_add(List * head, void *data)
{
  List *p,
  *j;

  p = malloc(sizeof(List));
  if (p == NULL) {
    fprintf(stderr, "list_add: malloc out of memory.\n");
    exit(1);
  }

  p->data = data;
  p->next = NULL;

  if (head == NULL)
    return p;

  for (j = head; j->next != NULL; j = j->next);
  assert(j != NULL);
  assert(j->next == NULL);

  j->next = p;

  return head;
}

void
list_free(List * head)
{
  List *p,
  *j;

  p = head;
  while (p != NULL) {
    j = p;
    p = p->next;
    free(j);
  }
}

List *append_list(List *front, List *back)
{
	List *p;

	if(front == NULL)
		return back;

	for(p = front; p->next != NULL; p = p->next);
	assert(p->next == NULL);

	p->next = back;

	return front;
}
