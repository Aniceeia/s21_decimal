#include "s21_is_set_bit.h"

// приравниваем число младшей степени к степени старшего числа беря во
// внимание только разницу степеней
void s21_normalize_big_decimal(s21_big_decimal* value_1,
                               s21_big_decimal* value_2) {
  int scale1 = s21_scale_s21_big_decimal(*value_1);
  int scale2 = s21_scale_s21_big_decimal(*value_2);
  int resize = 0;

  if (scale1 >= scale2) resize = scale1 - scale2;
  if (scale1 < scale2) resize = scale2 - scale1;

  for (int i = 0; i < resize; i++) {
    if (scale1 >= scale2) {
      s21_mul_on_ten_big_decimal(value_2);
    }
    if (scale1 < scale2) {
      s21_mul_on_ten_big_decimal(value_1);
    }
  }
}

// умножение на 10
void s21_mul_on_ten_big_decimal(s21_big_decimal* big_decimal) {
  s21_big_decimal result = {0}, big_decimal_copy = {0};

  big_decimal_copy = *big_decimal;

  // Обновляем big_decimal и big_decimal_copy, используя новые значения,
  // возвращаемые из s21_left_shift_s21_big_decimal
  s21_big_decimal shifted_big_decimal =
      s21_left_shift_s21_big_decimal(big_decimal_copy, 3);
  s21_big_decimal shifted_big_decimal_copy =
      s21_left_shift_s21_big_decimal(big_decimal_copy, 1);

  // Теперь используем shifted_big_decimal и shifted_big_decimal_copy для
  // сложения
  s21_add_big_mantiss(shifted_big_decimal, shifted_big_decimal_copy, &result);

  // Обновляем исходный big_decimal результатом
  *big_decimal = result;
}
