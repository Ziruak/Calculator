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

START_TEST(calculate_invalid_test) {
  const size_t test_size = 4;
  char *test_str[] = {"asdv", "sqrt()", "3eee20", "sin)0("};
  

  for (size_t i = 0; i < test_size; ++i) {
    double res = calculate(test_str[i],0);
    char error_str[64];
    sprintf(error_str, "Error in test %lu: result is %lf, should be nan\n", i,res);
    ck_assert_msg(res!=res,error_str); 
    
  }
}
END_TEST

START_TEST(calculate_valid_test) {
  const size_t test_size = 7;
  char *test_str[] = {"2+3", "-2", "sqrt(2)", "sin(123)^2+cos(123)^2", "-2+3*(3^2/6)", "1-1","+ln(120)modlog(100)"};
  double res[] = {5.,-2.,sqrt(2),pow(sin(123),2)+pow(cos(123),2),-2+3*(pow(3,2)/6),1.-1.,fmod(+log(120.),log10(100))};

  for (size_t i = 0; i < test_size; ++i) {
    ck_assert_double_eq_tol(calculate(test_str[i],0),res[i],1e-9);    
  }
}
END_TEST

START_TEST(calculate_x_test) {
  const size_t test_size = 5;
  const size_t x_size = 5;
  char *test_str[] = {"x", "-2*x + 3*x", "tan(x)", "asin(x^2) + sqrt(x-3)", "atan(-x)*acos(-x)"};
  double res;
  double xvals[] = {-1.,0,123.,3.14, 1e-12};
  

  for (size_t i = 0; i < test_size; ++i) {
    for (size_t j = 0; j < x_size; ++j) {
      switch (i)
      {
      case 0:
        res = xvals[j];
        break;
      case 1:
        res = -2*xvals[j]+3*xvals[j];
        break;
      case 2:
        res = tan(xvals[j]);
        break;
      case 3:
        res = asin(pow(xvals[j],2))+sqrt(xvals[j]-3);
        break;
      case 4:
        res = atan(-xvals[j])*acos(-xvals[j]);
        break;
      }
      if (res==res){
        ck_assert_double_eq_tol(calculate(test_str[i],xvals[j]),res,1e-9); 
      }else {
        ck_assert_double_nan(calculate(test_str[i],xvals[j]));
      }   
    }
  }
}
END_TEST

int main(void) {
  Suite *calculator = suite_create("calculator");

  TCase *tc_stack_tests = tcase_create("stack_tests"),
  *tc_calculate_tests = tcase_create("calculator_tests");

  SRunner *sr = srunner_create(calculator);

  suite_add_tcase(calculator, tc_stack_tests);
  tcase_add_test(tc_stack_tests, stack_test);

  suite_add_tcase(calculator, tc_calculate_tests);
  tcase_add_test(tc_calculate_tests, calculate_invalid_test);
  tcase_add_test(tc_calculate_tests, calculate_valid_test);
  tcase_add_test(tc_calculate_tests,calculate_x_test);


  srunner_run_all(sr, CK_ENV);
  srunner_ntests_failed(sr);
  srunner_free(sr);

  return 0;
}