#include "s21_other_operations.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int message = S21_CALC_ERR;

  if (result != NULL && !s21_is_decimal_incorrect(value)) {
    int sign = s21_decimal_sign(value);
    int scale = s21_scale_s21_decimal(value);
    s21_divide_by_ten_s21_decimal(&value, scale);

    *result = value;
    s21_decimal_set_sign(result, sign);
    message = S21_CALC_OK;
  }
  return message;
}
