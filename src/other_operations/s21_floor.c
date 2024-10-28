#include "s21_other_operations.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int message = S21_CALC_OK;
  int scale = 0;
  scale = s21_scale_s21_decimal(value);
  if (scale == 0 || s21_is_decimal_incorrect(value)) {
    *result = value;
    message = S21_CALC_ERR;
  }
  if (scale != 0) {
    s21_decimal buffer = {0};
    buffer = value;
    int check_dot_number = s21_divide_by_ten_s21_decimal(&buffer, scale);
    *result = buffer;
    int sign = s21_decimal_sign(value);
    if (sign) {
      if (check_dot_number) {
        s21_decimal plus_one = {{1}};
        s21_add(buffer, plus_one, result);
      }
      s21_decimal_set_sign(result, 1);
    }
  }
  return message;
}
