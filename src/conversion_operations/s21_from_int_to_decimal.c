#include "s21_convert.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int message = CONVERTER_OK;
  if (dst == NULL) {
    message = CONVERTER_ERROR;
  } else {
    s21_decimal_set_zeroes(dst);
    dst->bits[0] = abs(src);
    if (src < 0) {
      s21_decimal_set_sign(dst, S21_NEGATIVE);
    }
  }

  return message;
}
