#include "s21_convert.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int message = CONVERTER_OK;
  if (dst == NULL || s21_is_decimal_incorrect(src) == 1) {
    message = CONVERTER_ERROR;
  } else {
    double tmp = 0;

    for (int i = 0; i < MAX_BLOCK_NUMBER; i++) {
      if (s21_is_set_bit_s21_decimal(&src, i)) {
        tmp = tmp + pow(2, i);
      }
    }
    int scale = s21_scale_s21_decimal(src);
    while (scale != 0) {
      tmp = tmp / 10;
      scale--;
    }
    if (s21_decimal_sign(src) == S21_NEGATIVE) {
      tmp = tmp * (-1);
    }
    *dst = tmp;
  }
  return message;
}
