#include "s21_other_operations.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int message = S21_CALC_ERR;
  if (result != NULL && !s21_is_decimal_incorrect(value)) {
    *result = value;
    if (s21_decimal_sign(value)) {
      s21_decimal_set_sign(result, 0);
    } else {
      s21_decimal_set_sign(result, 1);
    }
    message = S21_CALC_OK;
  }
  return message;
}
