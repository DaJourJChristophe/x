#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

#include "common.h"
#include "ds/queue.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void queue_new_test(void unused **state)
{
  int data[DEFAULT_QUEUESIZE];
  queue_t *queue = queue_new(data, DEFAULT_QUEUESIZE, sizeof(int));

  assert_non_null(queue);
  assert_non_null(queue->data);
  assert_ptr_equal((void *)data, queue->data);
  assert_int_equal(queue->ofs, sizeof(int));
  assert_int_equal(queue->cap, DEFAULT_QUEUESIZE);
  assert_int_equal(queue->w, 0);
  assert_int_equal(queue->r, 0);

  queue_destroy(queue);
}

static void queue_write_test(void unused **state)
{
  size_t const n = 1;
  int data[n];
  int x = 1;

  queue_t *queue = queue_new(data, n, sizeof(int));

  assert_true(queue_write(queue, &x));
  assert_int_equal(queue->w, (uint64_t)1);

  assert_false(queue_write(queue, &x));

  queue_destroy(queue);
}

static void queue_read_test(void unused **state)
{
  int data[1];
  int x = 1;

  queue_t *queue = queue_new(data, 1, sizeof(int));

  queue_write(queue, &x);
  assert_int_equal(queue->w, (uint64_t)1);

  assert_non_null(queue_read(queue));
  assert_int_equal(queue->r, queue->w);

  assert_null(queue_read(queue));

  queue_destroy(queue);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(queue_new_test),
    cmocka_unit_test(queue_write_test),
    cmocka_unit_test(queue_read_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
