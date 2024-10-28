#include "s21_other_operations.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int message = S21_CALC_OK;
  s21_decimal zero = {0};
  int scale = s21_scale_s21_decimal(value);
  int sign = s21_decimal_sign(value);
  if (s21_is_decimal_incorrect(value)) {
    *result = value;
    message = S21_CALC_ERR;
  }
  if (scale == 0) {
    *result = value;
  }
  if (scale != 0 && message != S21_CALC_ERR) {
    s21_decimal buffer = value;
    s21_decimal buffer1 = value;
    int frac = s21_divide_by_ten_s21_decimal(&buffer, scale);
    int integ = s21_divide_by_ten_s21_decimal(&buffer1, scale + 1);
    int i = 0;
    char str[54] = {0};
    while (i < scale) {
      int check = 0;
      s21_decimal dec = value;
      check = s21_divide_by_ten_s21_decimal(&dec, scale - i);
      str[i] = '0' + check;
      i++;
    }
    if (((frac > 4) && ((frac % 2 && integ % 2) || !(frac % 2 || integ % 2))) ||
        (strlen(str) > 2 && frac > 4)) {
      s21_decimal one = {{1}};
      if ((s21_is_equal(buffer, zero) && frac <= 5 &&
           (str[1] < 1 || str[1] == '0') && (str[2] < 1 || str[2] == '0'))) {
        *result = zero;
        sign = S21_POSITIVE;
      } else {
        s21_add(buffer, one, result);
      }
    } else {
      *result = buffer;
    }
    if (sign) {
      s21_decimal_set_sign(result, 1);
    }
  }
  return message;
}
