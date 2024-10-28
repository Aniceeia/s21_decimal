#include "s21_convert.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int message = CONVERTER_OK;
  if (dst == NULL || s21_is_decimal_incorrect(src) == 1) {
    message = CONVERTER_ERROR;
  } else {
    *dst = 0;
    s21_decimal trunc_decimal;
    s21_decimal_set_zeroes(&trunc_decimal);
    s21_truncate(src, &trunc_decimal);
    if (trunc_decimal.bits[1] == 0 && trunc_decimal.bits[2] == 0 &&
        !(trunc_decimal.bits[0] == 2147483648 &&
          s21_decimal_sign(src) == S21_POSITIVE)) {
      *dst = trunc_decimal.bits[0];
      if (s21_decimal_sign(src) == S21_NEGATIVE) {
        *dst = -*dst;
      }
    } else {
      message = CONVERTER_ERROR;
    }
  }

  return message;
}
