#include "s21_math_operations.h"

void s21_multiplication(s21_big_decimal big_val_mul,
                        s21_big_decimal *big_val_sum,
                        s21_big_decimal big_val_copy,
                        s21_big_decimal big_val_orig, int *pos, int *counter) {
  (*pos)--;
  while (*pos >= 0) {
    big_val_copy = *big_val_sum;
    big_val_mul = big_val_orig;
    big_val_mul = s21_left_shift_s21_big_decimal(big_val_mul, counter[*pos]);
    s21_add_big_mantiss(big_val_mul, big_val_copy, big_val_sum);
    (*pos)--;
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int pos = 1, message = 0, pre_scale_1 = 0, pre_scale_2 = 0;

  s21_big_decimal big_val_copy = {0}, big_val_mul = {0}, big_val_sum = {0},
                  big_val_orig = {0}, big_val_round = {0};

  pre_scale_1 = s21_scale_s21_decimal(value_1);
  pre_scale_2 = s21_scale_s21_decimal(value_2);
  message =
      s21_mul_dec_correct(value_1, value_2, message, pre_scale_1, pre_scale_2);
  if ((message != S21_MINUS_INF) && (message != S21_NaN)) {
    int counter[MAX_MANTISS_BITS], sign = 0;
    sign = s21_mul_sign_check(value_1, value_2, sign);
    int fsign = s21_if_zero_clear(&value_1, &value_2);
    if (fsign) sign = S21_POSITIVE;

    s21_recording_units(value_2, &pos, counter);
    int scale = pre_scale_1 + pre_scale_2;
    s21_decimal_to_big_decimal(value_1, &big_val_orig);
    s21_multiplication(big_val_mul, &big_val_sum, big_val_copy, big_val_orig,
                       &pos, counter);

    message = s21_mul_round(big_val_sum, &big_val_round, &scale);
    s21_big_decimal_to_decimal(big_val_round, result);
    s21_decimal_set_scale(result, scale);

    if (sign == S21_NEGATIVE) s21_decimal_set_sign(result, 1);
    if (sign == S21_POSITIVE) s21_decimal_set_sign(result, 0);
    if (message == S21_PLUS_INF && sign == S21_NEGATIVE)
      message = S21_MINUS_INF;
  }
  return message;
}
