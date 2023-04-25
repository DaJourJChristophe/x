/**
 * @file linked-list.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-24
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "error.h"
#include "linked-list.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief
 */
#define LINKED_LIST_NODE_SIZE sizeof(linked_list_node_t)

/**
 * @brief
 */
#define LINKED_LIST_SIZE      sizeof(linked_list_t)

/**
 * @brief
 */
linked_list_node_t *linked_list_node_new(void *data, const size_t size)
{
  if (data == NULL || size == 0)
  {
    throw("cannot create linked-list node because data is null or size is zero");
  }

  linked_list_node_t *node = NULL;
  node = __malloc(LINKED_LIST_NODE_SIZE);

  node->data = __malloc(size);
  node->size = size;
  node->prev = NULL;
  node->next = NULL;

  memcpy(node->data, data, size);
  return node;
}

/**
 * @brief
 */
void linked_list_node_destroy(linked_list_node_t *node)
{
  if (node == NULL)
  {
    return;
  }

  __free(node->data);
  __free(node);
}

/**
 * @brief
 */
linked_list_t *linked_list_new(void)
{
  linked_list_t *list = NULL;
  list = __calloc(1, LINKED_LIST_SIZE);
  return list;
}

/**
 * @brief
 */
void linked_list_destroy(linked_list_t *list)
{
  if (list == NULL)
  {
    return;
  }
  if (list->head == NULL)
  {
    goto done;
  }

  linked_list_node_t *node = NULL;
  linked_list_node_t *next = NULL;

  node = list->head;

  while (node != NULL)
  {
    next = node->next;
    linked_list_node_destroy(node);
    node = NULL;
    node = next;
    next = NULL;
  }
done:
  __free(list);
}

/**
 * @brief
 */
void linked_list_push(linked_list_t *list, void *data, const size_t size)
{
  if (list == NULL)
  {
    throw("cannot insert into linked-list because list pointer is null");
  }

  if (list->head == NULL)
  {
    list->head = linked_list_node_new(data, size);
    list->tail = list->head;
    return;
  }

  list->tail->next = linked_list_node_new(data, size);
  list->tail->next->prev = list->tail;
  list->tail = list->tail->next;
}

/**
 * @brief
 */
void *linked_list_pop(linked_list_t *list)
{
  if (list == NULL)
  {
    throw("cannot pop linked-list because list pointer is null");
  }

  const size_t size = list->tail->size;
  void *data = __malloc(size);
  memcpy(data, list->tail->data, size);
  list->tail = list->tail->prev;
  linked_list_node_destroy(list->tail->next);
  list->tail->next = NULL;
  return data;
}

/**
 * @brief
 */
typedef void (*linked_list_callback_t)(const void *, const size_t);

/**
 * @brief
 */
void linked_list_foreach(linked_list_t *list, linked_list_callback_t fn)
{
  if (list == NULL)
  {
    throw("cannot enumerate over the linked-list because list pointer is null");
  }

  linked_list_node_t *node = NULL;
  node = list->head;

  while (node != NULL)
  {
    fn((const void *)node->data, (const size_t)node->size);
    node = node->next;
  }
}

/**
 * @brief
 */
void linked_list_reverse(linked_list_t *list, linked_list_callback_t fn)
{
  linked_list_node_t *node = NULL;
  node = list->tail;

  while (node != NULL)
  {
    fn((const void *)node->data, (const size_t)node->size);
    node = node->prev;
  }
}

// int main(void)
// {
//   linked_list_t *list = linked_list_new();

//   linked_list_push(list, &(int){3}, sizeof(int));
//   linked_list_push(list, &(int){5}, sizeof(int));
//   linked_list_push(list, &(int){7}, sizeof(int));
//   linked_list_push(list, &(int){9}, sizeof(int));

//   linked_list_foreach(list, &forward);
//   printf("\n");

//   __free(linked_list_pop(list));

//   linked_list_reverse(list, &backward);
//   printf("\n");

//   linked_list_destroy(list);
//   return EXIT_SUCCESS;
// }
