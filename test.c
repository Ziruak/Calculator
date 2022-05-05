#include "calculator.h"
#include "stack.h"
#include <check.h>

START_TEST(stack_test) {
  stack s = {NULL};
  ck_assert(s_empty(&s));
  op test_op = {NUMBER,2.3,0};
  s_push(&s,test_op);
  ck_assert(!s_empty(&s));
  test_op = s_top(&s);
  
  ck_assert(test_op.type == NUMBER && test_op.priority == 0);
  ck_assert(s_pop(&s));
  ck_assert(s_empty(&s));
  ck_assert(!s_pop(&s));
  s_clear(&s);
}
END_TEST

int main(void) {
  Suite *calculator = suite_create("calculator");

  TCase *tc_stack_tests = tcase_create("stack_tests");

  SRunner *sr = srunner_create(calculator);

  suite_add_tcase(calculator, tc_stack_tests);
  tcase_add_test(tc_stack_tests, stack_test);


  srunner_run_all(sr, CK_ENV);
  srunner_ntests_failed(sr);
  srunner_free(sr);

  return 0;
}