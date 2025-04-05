#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) { return NULL; }

void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node * node = malloc(sizeof(struct list_node));
  node->value = value;

  node->next = list->head;
  list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
  struct list_node * last = malloc(sizeof(struct list_node));
  last -> value = value;
  last -> next = NULL; 

  struct list_node *current = list -> head;

  if (list -> head == NULL)
  {
    list -> head = last;
    return;
  }

  while(current ->next != NULL)
  {
    current = current->next;
  }

  current->next = last;
}

size_t remove_from_head(struct linked_list *list) {
  
  if(list == NULL || list->head == NULL)
  {
    return 0;
  }

  struct list_node * old = list ->head;
  size_t value = old -> value;

  list -> head = old ->next;

  free(old);
  
  return value;
}

size_t remove_from_tail(struct linked_list *list) {
  
  if(list == NULL || list->head == NULL)
  {
    return 0;
  }
  
  struct list_node *current = list -> head;

  if(current->next== NULL)
  {
    size_t val = current ->value;
    free(current);
    list ->head = NULL;
    return val;
  }

  while(current -> next -> next != NULL)
  {
    current = current->next;
  }
  size_t val = current -> next -> value;
  free(current->next);

  current->next = NULL;

  return val;
  }

void free_list(struct linked_list list) {
  struct list_node * current = list.head;

  while(current !=NULL)
  {
    struct list_node *next = current -> next;
    free(current);
    current = next;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
