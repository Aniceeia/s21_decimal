#include "s21_is_set_bit.h"

// val = 0001, index = 1 -> return 0011
unsigned s21_init_bit(unsigned val, int index) { return val | (1U << index); }
// val = 0001, index = 0 -> return 0000
unsigned s21_reset_bit(unsigned val, int index) { return val & ~(1U << index); }
// устанавливает биты по интексу
void s21_big_decimal_bit_init(s21_big_decimal *big_decimal, int index,
                              int bit) {
  int blockIndex = index / MAX_BLOCK_BITS;
  int bitIndexInBlock = index % MAX_BLOCK_BITS;

  if (bit) {
    big_decimal->bits[blockIndex] =
        s21_init_bit(big_decimal->bits[blockIndex], bitIndexInBlock);
  } else {
    big_decimal->bits[blockIndex] =
        s21_reset_bit(big_decimal->bits[blockIndex], bitIndexInBlock);
  }
}
// обработка помещается ли малый децимал в большой
int s21_is_big_decimal_fits_in_decimal(s21_big_decimal *big_decimal) {
  return (big_decimal->bits[3] == 0 && big_decimal->bits[4] == 0 &&
          big_decimal->bits[5] == 0);
}
// проверяет находятся ли в мантиссе нули
int s21_is_decimal_mantiss_zero(s21_decimal decimal) {
  return (decimal.bits[0] == 0 && decimal.bits[1] == 0 && decimal.bits[2] == 0);
}

S21_SCALE(s21_decimal)
S21_SCALE(s21_big_decimal)
S21_IS_SET_BIT(s21_decimal)
S21_IS_SET_BIT(s21_big_decimal)
S21_DECIMAL_BIT3(zero_1)
S21_DECIMAL_BIT3(zero_2)
S21_DECIMAL_SET(sign)
S21_DECIMAL_SET(scale)
S21_FIRST_NOT_NULL_BIT(s21_decimal)
S21_FIRST_NOT_NULL_BIT(s21_big_decimal)
S21_DIVIDE_BY_TEN(s21_decimal)
S21_DIVIDE_BY_TEN(s21_big_decimal)
S21_LEFT_SHIFT(s21_decimal)
S21_LEFT_SHIFT(s21_big_decimal)