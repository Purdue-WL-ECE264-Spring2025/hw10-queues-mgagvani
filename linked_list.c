#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) { 
  struct list_node* new_node = malloc(sizeof(struct list_node));
  new_node->value = value;
  new_node->next = NULL;
  return new_node;
 }

void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node* head = list->head;
  struct list_node* new_head = new_node(value);
  new_head->next = head; // reassign
  list->head = new_head; // now make this the new head;
}

void insert_at_tail(struct linked_list *list, size_t value) {
  // create a new list if its empty
  if (list->head == NULL) {
    list->head = new_node(value);
    return;
  }

  // find the end
  struct list_node* node = list->head;
  while (node->next != NULL) {
    node = node->next;
  }
  // now we have the last node.
  node->next = new_node(value);
}

size_t remove_from_head(struct linked_list *list) { 
  if (list->head == NULL) {
    return 0; // empty list
  }
  
  struct list_node* old_head = list->head;
  size_t old_value = old_head->value;
  list->head = old_head->next;
  free(old_head);
  return old_value;
}

size_t remove_from_tail(struct linked_list *list) { 
  if(list->head == NULL) {
    return 0; // there wasn't anything there to begin with
  }

  // Only one node in the list
  if(list->head->next == NULL) {
    size_t value = list->head->value;
    free(list->head);
    list->head = NULL;
    return value;
  }

  struct list_node* node = list->head;
  struct list_node* prev_node = NULL;
  while(node->next != NULL) {
    prev_node = node;
    node = node->next;
  }
  size_t to_ret = node->value;
  free(node);
  prev_node->next = NULL;
  return to_ret;
}

void free_list(struct linked_list list) {
  struct list_node* curr = list.head;
  if (curr == NULL) {
    return; // Empty list
  }
  
  while (curr != NULL) {
    struct list_node* next = curr->next;
    free(curr);
    curr = next;
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
