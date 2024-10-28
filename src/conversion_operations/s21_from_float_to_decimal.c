#include "s21_convert.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  const int LIMIT = MY_INT - 1;
  int count_int = 0, sign = S21_POSITIVE, flag_zero = 0;
  int message = s21_handling_edge_cases_converter(src, dst, &flag_zero);

  s21_big_decimal big_dst = {0};
  if (message == CONVERTER_OK) s21_decimal_set_zeroes(dst);
  if (src < 0.0 && message == CONVERTER_OK && flag_zero == 0) {
    sign = S21_NEGATIVE;
    src = fabs(src);
  }
  if (message == CONVERTER_OK && flag_zero == 0) {
    int scale = 0, exp = 0;
    double temp = 0;
    exp = s21_get_exponent_float(src);
    temp = (double)src;
    if (exp < 95) {
      for (; !((int)temp); temp *= 10, scale++) {
      }
      s21_count_int_in_float(temp, &count_int);
      if (count_int < LIMIT) {
        for (int i = count_int; i < MY_INT; i++, count_int++) {
          temp *= 10.0;
          scale++;
        }
        if ((int)temp % 10 >= 5) temp += 10;
        temp /= 10;
        scale--;
        s21_from_int_to_decimal((int)temp, dst);
      } else {
        int scale_div = 0;
        int finish_value = s21_get_first_digits(temp, &scale_div);
        s21_from_int_to_decimal(finish_value, dst);
        s21_decimal_to_big_decimal(*dst, &big_dst);
        while (scale_div > 0) {
          s21_mul_on_ten_big_decimal(&big_dst);
          scale_div--;
        }
        s21_big_decimal_to_decimal(big_dst, dst);
      }
    }
    s21_decimal_set_sign(dst, sign);
    if (0 <= scale && scale <= 28)
      s21_decimal_set_scale(dst, scale);
    else
      message = CONVERTER_ERROR;
  }
  return message;
}
