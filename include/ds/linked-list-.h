#ifndef X_LINKED_LIST_H
#define X_LINKED_LIST_H

#include <stddef.h>

/**
 * @brief
 */
struct linked_list_node
{
  void  *data;                     /* A pointer to the node data. */
  size_t size;                     /* A definition of the size of the data at the data-pointer. */
  struct linked_list_node *prev;   /* A pointer to the previous sibling node in the list. */
  struct linked_list_node *next;   /* A pointer to the next sibling node in the list. */
};

/**
 * @brief
 */
typedef struct linked_list_node linked_list_node_t;

/**
 * @brief
 */
struct linked_list
{
  linked_list_node_t *head;   /* A pointer to the head of the list. */
  linked_list_node_t *tail;   /* A pointer to the tail of the list. */
};

/**
 * @brief
 */
typedef struct linked_list linked_list_t;

/**
 * @brief
 */
linked_list_node_t *linked_list_node_new(void *data, const size_t size);

/**
 * @brief
 */
void linked_list_node_destroy(linked_list_node_t *node);

/**
 * @brief
 */
linked_list_t *linked_list_new(void);

/**
 * @brief
 */
void linked_list_destroy(linked_list_t *list);

/**
 * @brief
 */
void linked_list_push(linked_list_t *list, void *data, const size_t size);

/**
 * @brief
 */
void *linked_list_pop(linked_list_t *list);

/**
 * @brief
 */
typedef void (*linked_list_callback_t)(const void *, const size_t);

/**
 * @brief
 */
void linked_list_foreach(linked_list_t *list, linked_list_callback_t fn);

/**
 * @brief
 */
void linked_list_reverse(linked_list_t *list, linked_list_callback_t fn);

#endif/*X_LINKED_LIST_H*/
