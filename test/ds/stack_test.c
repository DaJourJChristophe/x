#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

#include "common.h"
#include "ds/stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void stack_new_test(void unused **state)
{
  int data[DEFAULT_STACKSIZE];
  stack_t *stack = stack_new(data, DEFAULT_STACKSIZE, sizeof(int));

  assert_non_null(stack);
  assert_non_null(stack->data);
  assert_ptr_equal((void *)data, stack->data);
  assert_int_equal(stack->ofs, sizeof(int));
  assert_int_equal(stack->cap, DEFAULT_STACKSIZE);
  assert_int_equal(stack->top, 0);

  stack_destroy(stack);
}

static void stack_push_test(void unused **state)
{
  size_t const n = 1;
  int data[n];
  int x = 1;

  stack_t *stack = stack_new(data, n, sizeof(int));

  assert_true(stack_push(stack, &x));
  assert_int_equal(stack->top, (uint64_t)1);

  assert_false(stack_push(stack, &x));

  stack_destroy(stack);
}

static void stack_pop_test(void unused **state)
{
  int data[1];
  int x = 1;

  stack_t *stack = stack_new(data, 1, sizeof(int));

  stack_push(stack, &x);
  assert_int_equal(stack->top, (uint64_t)1);

  assert_non_null(stack_pop(stack));
  assert_int_equal(stack->top, (uint64_t)0);

  assert_null(stack_pop(stack));

  stack_destroy(stack);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(stack_new_test),
    cmocka_unit_test(stack_push_test),
    cmocka_unit_test(stack_pop_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
