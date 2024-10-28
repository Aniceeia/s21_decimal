#ifndef _S21_COMPARE_H_
#define _S21_COMPARE_H_
#include "../s21_decimal.h"

int s21_is_compare(s21_decimal decimal1, s21_decimal decimal2);
int s21_compare_mantiss(int* mantis1, int* manti_count1, int* mantis2,
                        int* manti_count2, int trye, int nigate);
void s21_write_to_arr(int* mantis, int* manti_count, s21_big_decimal decimal,
                      int i);

int s21_is_less(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_less_or_equal(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_greater(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_greater_or_equal(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_equal(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_not_equal(s21_decimal decimal1, s21_decimal decimal2);

#endif