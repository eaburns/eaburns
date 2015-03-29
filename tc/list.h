#ifndef _LIST_H_
#define _LIST_H_

/*
 * A list element data structure. 
 */
typedef struct List {
  void *data;
  struct List *next;
} List;

/*
 * Adds data to a new element in the list, returns the (potentially new
 * head of the list). 
 */
List *list_add(List * head, void *data);

/* 
 * Appends back to the end of front and returns a pointer to the
 * new head.
 */
List *append_list(List *front, List *back);

/*
 * Frees all list nodes. 
 */
void list_free(List * head);

#endif                          /* !_LIST_H_ */
