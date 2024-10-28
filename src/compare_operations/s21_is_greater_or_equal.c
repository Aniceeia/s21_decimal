#include "s21_compare.h"

int s21_is_greater_or_equal(s21_decimal decimal1, s21_decimal decimal2) {
  int answer = S21_FALSE;
  int pre_answer = s21_is_compare(decimal1, decimal2);
  if (pre_answer == S21_TRUE) answer = S21_FALSE;
  if (pre_answer != S21_TRUE) answer = S21_TRUE;
  if (pre_answer == S21_ERROR) answer = S21_FALSE;
  return answer;
}