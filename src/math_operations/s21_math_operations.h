#ifndef MATH_OP_H
#define MATH_OP_H

#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_add_logic(s21_decimal value_1, s21_decimal value_2,
                  s21_decimal *result);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void s21_sub_logic(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void s21_multiplication(s21_big_decimal big_val_mul,
                        s21_big_decimal *big_val_sum,
                        s21_big_decimal big_val_copy,
                        s21_big_decimal big_val_orig, int *pos, int *counter);

// div
s21_big_decimal s21_get_result_in_big_decimal(char *str, int scale,
                                              int *message, int *scale_finish,
                                              int last_bit);
s21_decimal s21_get_result_in_string(s21_decimal dividend, s21_decimal divisor,
                                     char *str, int *last_bit);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// new
s21_big_decimal mul_str(char *str, int scale_frac);

#endif /*math_operaionts.h*/