#include "s21_math_operations.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int message = S21_OK;
  int sign_1 = s21_decimal_sign(value_1);
  int sign_2 = s21_decimal_sign(value_2);

  if (s21_is_decimal_incorrect(value_1) || s21_is_decimal_incorrect(value_2)) {
    message = S21_NaN;
    *result = s21_decimal_set_inf();
  } else if (s21_decimals_overflow(value_1) && s21_decimals_overflow(value_2) &&
             (sign_1 == S21_NEGATIVE && sign_2 == S21_POSITIVE)) {
    message = S21_MINUS_INF;
    *result = s21_decimal_set_inf();

  } else if ((s21_decimals_overflow(value_1) ||
              s21_decimals_overflow(value_2)) &&
             (sign_1 == S21_NEGATIVE && sign_2 == S21_POSITIVE) &&
             (s21_scale_s21_decimal(value_1) == 28 &&
              s21_scale_s21_decimal(value_2) == 28)) {
    message = S21_MINUS_INF;
    *result = s21_decimal_set_inf();

  } else if (s21_decimals_overflow(value_1) && s21_decimals_overflow(value_2) &&
             (sign_1 == S21_POSITIVE && sign_2 == S21_NEGATIVE)) {
    message = S21_PLUS_INF;
    *result = s21_decimal_set_inf();
  } else {
    if (sign_1 == S21_POSITIVE && sign_2 == S21_POSITIVE) {
      s21_sub_logic(value_1, value_2, result);

    } else if (sign_1 == S21_POSITIVE && sign_2 == S21_NEGATIVE) {
      message = s21_add(value_1, s21_dabs(value_2), result);

    } else if (sign_1 == S21_NEGATIVE && sign_2 == S21_POSITIVE) {
      message = s21_add(s21_dabs(value_1), value_2, result);
      s21_negate(*result, result);

    } else if (sign_1 == S21_NEGATIVE && sign_2 == S21_NEGATIVE) {
      s21_sub_logic(s21_dabs(value_1), s21_dabs(value_2), result);
      s21_negate(*result, result);
    }
  }

  return message;
}

void s21_sub_logic(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result) {
  s21_big_decimal big_val_1 = {0}, big_val_2 = {0}, big_result = {0};

  int scale1 = s21_scale_s21_decimal(value_1);
  int scale2 = s21_scale_s21_decimal(value_2);
  int rescale = 0;

  s21_decimal_to_big_decimal(value_1, &big_val_1);
  s21_decimal_to_big_decimal(value_2, &big_val_2);

  if (scale1 > scale2) {
    s21_decimal_set_scale(&value_2, scale1);
  } else {
    s21_decimal_set_scale(&value_1, scale2);
  }
  rescale = s21_scale_s21_decimal(value_1);
  s21_normalize_big_decimal(&big_val_1, &big_val_2);
  if (s21_is_greater(s21_dabs(value_2), s21_dabs(value_1))) {
    s21_big_decimal_swap(&big_val_1, &big_val_2);
    s21_negate(*result, result);
  }
  s21_sub_big_mantiss(big_val_1, big_val_2, &big_result);

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
    s21_decimal_set_scale(result, rescale);
  }
}