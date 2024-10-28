#include "s21_math_operations.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int message = S21_OK;
  if (!result) {
    message = S21_NaN;
  }
  int sign_1 = s21_decimal_sign(value_1);
  int sign_2 = s21_decimal_sign(value_2);
  s21_decimal check = {{5, 0, 0, 0x10000}};

  if (s21_is_decimal_incorrect(value_1) || s21_is_decimal_incorrect(value_2)) {
    message = S21_NaN;
  } else if (((s21_decimals_overflow(value_1) &&
               s21_scale_s21_decimal(value_1) == 0) ||
              (s21_decimals_overflow(value_2) &&
               s21_scale_s21_decimal(value_2) == 0)) &&
             (sign_1 == S21_POSITIVE && sign_2 == S21_POSITIVE)) {
    if (s21_is_less(value_1, check) || s21_is_less(value_2, check)) {
      message = S21_OK;
      *result = s21_decimal_set_inf();
    } else {
      message = S21_PLUS_INF;
      *result = s21_decimal_set_inf();
    }
  } else if (((s21_decimals_overflow(value_1) &&
               s21_scale_s21_decimal(value_1) == 0) ||
              (s21_decimals_overflow(value_2) &&
               s21_scale_s21_decimal(value_2) == 0)) &&
             (sign_1 == S21_NEGATIVE && sign_2 == S21_NEGATIVE)) {
    if (s21_is_less(s21_dabs(value_1), check) ||
        s21_is_less(s21_dabs(value_2), check)) {
      message = S21_OK;
      *result = s21_decimal_set_inf();
      s21_negate(*result, result);
    } else {
      message = S21_MINUS_INF;
      s21_decimal_set_zeroes(result);
      s21_decimal_set_sign(result, S21_POSITIVE);
    }
  } else {
    if (sign_1 == S21_POSITIVE && sign_2 == S21_POSITIVE) {
      message = s21_add_logic(s21_dabs(value_1), s21_dabs(value_2), result);

    } else if (sign_1 == S21_POSITIVE && sign_2 == S21_NEGATIVE) {
      message = s21_sub(s21_dabs(value_1), s21_dabs(value_2), result);
      s21_negate(value_2, &value_2);
      if (s21_is_greater(value_2, value_1)) {
        s21_negate(*result, result);
      }
    } else if (sign_1 == S21_NEGATIVE && sign_2 == S21_POSITIVE) {
      message = s21_sub(s21_dabs(value_1), value_2, result);
      s21_negate(value_1, &value_1);
      if (s21_is_greater(value_1, value_2)) {
        s21_negate(*result, result);
      }
    } else if (sign_1 == S21_NEGATIVE && sign_2 == S21_NEGATIVE) {
      message = s21_sub(value_1, s21_dabs(value_2), result);
      if (message == S21_PLUS_INF) {
        message = S21_MINUS_INF;
        s21_decimal_set_zeroes(result);
        s21_decimal_set_sign(result, S21_POSITIVE);
      }
    }
  }
  return message;
}
// логика сложения, знак ставим за пределами
int s21_add_logic(s21_decimal value_1, s21_decimal value_2,
                  s21_decimal *result) {
  s21_big_decimal big_value_1 = {0};
  s21_big_decimal big_value_2 = {0};
  s21_big_decimal big_result = {0};

  int scale1 = s21_scale_s21_decimal(value_1);
  int scale2 = s21_scale_s21_decimal(value_2);
  int rescale = 0;
  int message = 0;
  s21_decimal_to_big_decimal(value_1, &big_value_1);
  s21_decimal_to_big_decimal(value_2, &big_value_2);
  s21_normalize_big_decimal(&big_value_1, &big_value_2);
  if (scale1 > scale2) {
    s21_decimal_set_scale(&value_2, scale1);
  } else {
    s21_decimal_set_scale(&value_1, scale2);
  }
  rescale = s21_scale_s21_decimal(value_1);
  s21_add_big_mantiss(big_value_1, big_value_2, &big_result);
  if (s21_is_big_decimal_fits_in_decimal(&big_result)) {
    s21_big_decimal_to_decimal(big_result, result);
    s21_decimal_set_scale(result, rescale);
  } else {
    while (rescale > 0 && !s21_is_big_decimal_fits_in_decimal(&big_result)) {
      s21_bank_round(big_result, &big_result);
      rescale--;
    }
  }
  if (rescale <= 28 && s21_is_big_decimal_fits_in_decimal(&big_result)) {
    s21_big_decimal_to_decimal(big_result, result);
    s21_decimal_set_scale(result, scale1);
  }
  if (!s21_is_big_decimal_fits_in_decimal(&big_result)) {
    message = S21_PLUS_INF;
    *result = s21_decimal_set_inf();
  }
  return message;
}