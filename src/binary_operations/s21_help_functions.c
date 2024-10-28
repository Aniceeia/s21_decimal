#include "s21_is_set_bit.h"

// записываем в биг децимал decimal_to_big_decimal(из чего записываем, во что)
void s21_decimal_to_big_decimal(s21_decimal decimal,
                                s21_big_decimal *big_decimal) {
  *big_decimal = (s21_big_decimal){0};
  big_decimal->bits[0] = decimal.bits[0];
  big_decimal->bits[1] = decimal.bits[1];
  big_decimal->bits[2] = decimal.bits[2];
  big_decimal->bits[6] = decimal.bits[3];
}
// из биг децимала в децимал big_decimal_to_decimal(из чего записываем, во что)
void s21_big_decimal_to_decimal(s21_big_decimal big_decimal,
                                s21_decimal *decimal) {
  *decimal = (s21_decimal){0};
  decimal->bits[0] = big_decimal.bits[0];
  decimal->bits[1] = big_decimal.bits[1];
  decimal->bits[2] = big_decimal.bits[2];
}
// зануляет все биты децимала
void s21_decimal_set_zeroes(s21_decimal *decimal) {
  *decimal = (s21_decimal){0};
}

// проверяет знак децимала 0 +, 1 -
int s21_decimal_sign(s21_decimal decimal) {
  s21_scale_sign bit_3;
  bit_3.number = decimal.bits[3];

  return bit_3.part.sign;
}
//  0 - окей, 1 - ошибку
// 0 - 15, 24 - 30 = 0, 16 - 23 - scale = [0, 28)
int s21_is_decimal_incorrect(s21_decimal decimal) {
  int return_val = 0;
  int scale = s21_scale_s21_decimal(decimal);

  if (s21_decimal_bit3_zero_1(decimal) != 0 ||
      s21_decimal_bit3_zero_2(decimal) != 0) {
    return_val = 1;
  } else {
    if (!(0 <= scale && scale <= 28)) {
      return_val = 1;
    }
  }
  return return_val;
}
// установка всех битов в return_val = {{MAX_INT, MAX_INT, MAX_INT,0}};
s21_decimal s21_decimal_set_inf(void) {
  s21_decimal return_val = {{MAX_INT, MAX_INT, MAX_INT, 0}};
  return return_val;
}
// аналог abs() для децимала
s21_decimal s21_dabs(s21_decimal decimal) {
  s21_decimal_set_sign(&decimal, 0);

  return decimal;
}
// обработка краевого случая
int s21_decimals_overflow(s21_decimal decimal) {
  int overflow = S21_FALSE;

  if ((decimal.bits[0] == MAX_INT && decimal.bits[1] == MAX_INT &&
       decimal.bits[2] == MAX_INT)) {
    overflow = S21_TRUE;
  }

  return overflow;
}
// меняем децималы местами
void s21_big_decimal_swap(s21_big_decimal *decimal1,
                          s21_big_decimal *decimal2) {
  s21_big_decimal *temp = (s21_big_decimal *)malloc(sizeof(s21_big_decimal));

  memcpy(temp->bits, decimal1->bits, sizeof(temp->bits));
  memcpy(decimal1->bits, decimal2->bits, sizeof(decimal2->bits));
  memcpy(decimal2->bits, temp->bits, sizeof(temp->bits));

  free(temp);
}
// побитовое сложение для битов с 0 по 2
void s21_add_big_mantiss(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {
  int overflow_bit = 0;

  for (int i = 0; i < MAX_MANTISS_BITS; i++) {
    int res_bit = s21_is_set_bit_s21_big_decimal(&value_1, i) +
                  s21_is_set_bit_s21_big_decimal(&value_2, i) + overflow_bit;
    overflow_bit = res_bit / 2;
    res_bit %= 2;
    s21_big_decimal_bit_init(result, i, res_bit);
  }
}
// побитовое вычитание
void s21_sub_big_mantiss(s21_big_decimal big_decimal1,
                         s21_big_decimal big_decimal2,
                         s21_big_decimal *result) {
  int overflow_bit = 0;

  for (int i = 0; i < MAX_MANTISS_BITS; i++) {
    int res_bit = s21_is_set_bit_s21_big_decimal(&big_decimal1, i) -
                  s21_is_set_bit_s21_big_decimal(&big_decimal2, i) -
                  overflow_bit;

    overflow_bit = res_bit < 0;
    res_bit = abs(res_bit);
    res_bit %= 2;
    s21_big_decimal_bit_init(result, i, res_bit);
  }
}
// compare
//  первый минуус - 1, второй минус - 0, оба минусы - 2, оба плюсы - (-1)
int s21_sign_check(s21_decimal decimal1, s21_decimal decimal2) {
  int next = -1;
  int sign1 = s21_decimal_sign(decimal1);
  int sign2 = s21_decimal_sign(decimal2);
  if (sign1 != 0 && sign2 == 0) {
    next = 1;
  } else if (sign1 == 0 && sign2 != 0) {
    next = 0;
  } else if (sign1 != 0 && sign2 != 0) {
    next = 2;
  }
  return next;
}

// проверяет мантиссу на 0 и зануляет биты скейла
s21_decimal s21_zero_clean(s21_decimal decimal) {
  if (s21_is_decimal_mantiss_zero(decimal)) {
    s21_decimal_set_zeroes(&decimal);
  }
  return decimal;
}

// проверяет одно значение и при 0 затирает оба (mull)
int s21_if_zero_clear(s21_decimal *value_1, s21_decimal *value_2) {
  int active = 0;
  *value_1 = s21_zero_clean(*value_1);
  if ((s21_is_decimal_mantiss_zero(*value_1))) {
    s21_decimal_set_zeroes(value_2);
    active = 1;
  }
  *value_2 = s21_zero_clean(*value_2);
  if ((s21_is_decimal_mantiss_zero(*value_2))) {
    s21_decimal_set_zeroes(value_1);
    active = 1;
  }
  return active;
}
//  проверка знака для умножения -+ или +- будет "-" при ++ или -- будет "+"
int s21_mul_sign_check(s21_decimal value_1, s21_decimal value_2, int sign) {
  if (s21_sign_check(value_1, value_2) == 0 ||
      s21_sign_check(value_1, value_2) == 1)
    sign = S21_NEGATIVE;
  if (s21_sign_check(value_1, value_2) == -1 ||
      s21_sign_check(value_1, value_2) == 2)
    sign = S21_POSITIVE;
  return sign;
}

int s21_mul_round(s21_big_decimal value, s21_big_decimal *result, int *scale) {
  int was = 0, fits_in = 0, message = 0, copy_scale = (*scale);
  s21_big_decimal buffer = value, one = {{1}};
  fits_in = s21_is_big_decimal_fits_in_decimal(&buffer);
  while (((*scale) > 0 && !fits_in) || ((*scale) > 28 && fits_in)) {
    s21_big_decimal answer;
    int remainder = s21_divide_by_ten_s21_big_decimal(&buffer, 1);
    (*scale)--;
    fits_in = s21_is_big_decimal_fits_in_decimal(&buffer);
    if (remainder > 5) {
      was = 1;
      answer = buffer;
      s21_add_big_mantiss(answer, one, &buffer);
    }
    if (remainder == 5) {
      if ((was == 1) ||
          (was == 0 && s21_is_set_bit_s21_big_decimal(&buffer, 0))) {
        answer = buffer;
        s21_add_big_mantiss(answer, one, &buffer);
      }
      was = 1;
    }
    if (remainder <= 4 && remainder != 0) was = 1;
  }
  while (1) {
    s21_big_decimal copy = buffer;
    if (*scale > 0 && s21_divide_by_ten_s21_big_decimal(&copy, 1) == 0) {
      s21_divide_by_ten_s21_big_decimal(&buffer, 1);
      *scale = *scale - 1;
    } else {
      break;
    }
  }
  if ((*scale) == 0 && !fits_in) message = S21_PLUS_INF;
  if ((copy_scale > 28) && (buffer.bits[0] == 0)) message = S21_MINUS_INF;
  if (message) *result = value;
  if (!message) *result = buffer;
  return message;
}
// converter
int s21_get_exponent_float(float f) {
  unsigned bits = *(unsigned *)&f;
  unsigned exponent_bits = (bits >> 23) & 0xFF;
  return (int)exponent_bits - 127;
}
// converter
void s21_count_int_in_float(float src, int *count) {
  *count = 0;
  while (src >= 1) {
    (*count)++;
    src = src / 10;
  }
}
// converter
int s21_plus_or_tvar_drozhachaya(float num) {
  char buffer[50];
  int add = 0;
  sprintf(buffer, "%.10f", num);
  int d = buffer[7] - '0';
  if (!('0' <= buffer[7] && buffer[7] <= '9')) {
    d = buffer[8] - '0';
  }
  if (d >= 5) {
    add = 1;
  }
  return add;
}
// converter
int s21_get_first_digits(float number, int *second_mno) {
  int order = log10(number);
  float scaled = number / pow(10, order - 6);
  long long first_seven = (long long)scaled;
  first_seven += s21_plus_or_tvar_drozhachaya(number);
  int c1 = 0;
  s21_count_int_in_float(number, &c1);
  *second_mno = c1 - 7;

  return (int)first_seven;
}
// converter
int s21_hanging_zero(float tmp, s21_decimal *dst) {
  int code = CONVERTER_OK;
  FloatUnion fu;
  fu.f = tmp;
  if (fu.i == 0x80000000) {
    s21_decimal_set_sign(dst, S21_NEGATIVE);
    code = CONVERTER_ERROR;
  }
  return code;
}
// converter
int s21_handling_edge_cases_converter(float tmp, s21_decimal *dst,
                                      int *flag_hang_zero) {
  int code = CONVERTER_OK;
  if (dst == NULL || isinf(tmp) || isnan(tmp)) {
    code = CONVERTER_ERROR;
  } else {
    if ((0 < fabs(tmp) && fabs(tmp) < LEFT_BORDER) ||
        fabs(tmp) >= RIGHT_BORDER) {
      code = CONVERTER_ERROR;
    }
  }
  if (code == CONVERTER_OK && tmp == 0.0) {
    if (s21_hanging_zero(tmp, dst) == CONVERTER_ERROR) {
      code = CONVERTER_ERROR;
    } else {
      *flag_hang_zero = 1;
    }
  }
  return code;
}

// записывает номера единичеек (mul)
void s21_recording_units(s21_decimal value, int *pos, int *counter) {
  int index = 0;
  for (int i = 0; i < MAX_BLOCK_NUMBER; i++) {
    if (s21_is_set_bit_s21_decimal(&value, i)) {
      counter[index++] = i;
    }
  }
  *pos = index;
}

/// s21_div
s21_decimal s21_decimal_init_bit(s21_decimal decimal, int index) {
  int blockIndex = index / MAX_BLOCK_BITS;
  int bitIndexInBlock = index % MAX_BLOCK_BITS;

  decimal.bits[blockIndex] =
      s21_init_bit(decimal.bits[blockIndex], bitIndexInBlock);
  return decimal;
}
s21_big_decimal s21_big_decimal_init_bit(s21_big_decimal decimal, int index) {
  int blockIndex = index / MAX_BLOCK_BITS;
  int bitIndexInBlock = index % MAX_BLOCK_BITS;

  decimal.bits[blockIndex] =
      s21_init_bit(decimal.bits[blockIndex], bitIndexInBlock);
  return decimal;
}

// переход к double домножение до тех пор пока целым
long double s21_binary_to_double(const char *binary, int *scale_frac) {
  long double result = 0.0;
  long double frac = 0.5;

  for (int i = 0; i < (int)strlen(binary); i++) {
    if (binary[i] == '1') {
      result += frac;
    }
    frac /= 2.0;
  }
  *scale_frac = 0;
  while (*scale_frac < 28) {
    if ((int)result == result) {
      break;
    }
    result *= 10;
    *scale_frac = *scale_frac + 1;
  }
  result = roundl(result);

  return result;
}

s21_decimal s21_set_bit_string(s21_decimal value, const char *str, int start,
                               int end) {
  for (int i = start; i < end; i++) {
    if (str[i] == '1') {
      value = s21_decimal_init_bit(value, end - 1 - i);
    }
  }
  return value;
}

s21_decimal s21_decimal_right_shift(s21_decimal val, int shift_val) {
  if (shift_val <= 0 || shift_val >= 96) {
    return val;
  }
  int full_blocks_shift = shift_val / MAX_BLOCK_BITS;
  int remaining_bits_shift = shift_val % MAX_BLOCK_BITS;

  if (full_blocks_shift > 0) {
    for (int i = 0; i < 3; ++i) {
      if (i + full_blocks_shift < 3) {
        val.bits[i] = val.bits[i + full_blocks_shift];
      } else {
        val.bits[i] = 0;
      }
    }
  }
  if (remaining_bits_shift > 0) {
    unsigned memory = 0;
    for (int i = 2; i >= 0; --i) {
      unsigned temp = val.bits[i];
      val.bits[i] >>= remaining_bits_shift;
      val.bits[i] |= memory;
      memory = temp << (MAX_BLOCK_BITS - remaining_bits_shift);
    }
  }

  return val;
}
s21_big_decimal s21_big_decimal_right_shift(s21_big_decimal val,
                                            int shift_val) {
  int full_blocks_shift = shift_val / MAX_BLOCK_BITS;
  int remaining_bits_shift = shift_val % MAX_BLOCK_BITS;

  if (full_blocks_shift > 0) {
    for (int i = 0; i < 6; ++i) {
      if (i + full_blocks_shift < 6) {
        val.bits[i] = val.bits[i + full_blocks_shift];
      } else {
        val.bits[i] = 0;
      }
    }
  }
  if (remaining_bits_shift > 0) {
    unsigned memory = 0;
    for (int i = 5; i >= 0; --i) {
      unsigned temp = val.bits[i];
      val.bits[i] >>= remaining_bits_shift;
      val.bits[i] |= memory;
      memory = temp << (MAX_BLOCK_BITS - remaining_bits_shift);
    }
  }

  return val;
}
void s21_bank_round(s21_big_decimal value, s21_big_decimal *result) {
  s21_big_decimal buffer1 = value;
  s21_big_decimal buffer2 = value;
  s21_big_decimal one = {{1}};
  int check = s21_divide_by_ten_s21_big_decimal(&buffer1, 1);
  int check_next = s21_divide_by_ten_s21_big_decimal(&buffer2, 2);
  if (check == 5 && (check_next % 2 == 1)) {
    s21_add_big_mantiss(buffer1, one, result);
  } else if (check > 5) {
    s21_add_big_mantiss(buffer1, one, result);
  } else {
    *result = buffer1;
  }
}

int s21_mul_dec_correct(s21_decimal value_1, s21_decimal value_2, int message,
                        int pre_scale_1, int pre_scale_2) {
  if (s21_decimal_bit3_zero_1(value_1) != 0 ||
      s21_decimal_bit3_zero_2(value_1) != 0 ||
      s21_decimal_bit3_zero_1(value_2) != 0 ||
      s21_decimal_bit3_zero_2(value_2) != 0) {
    message = S21_NaN;
  }
  if ((pre_scale_1 < 0) || (pre_scale_2 < 0)) {
    message = S21_NaN;
  }
  if (((pre_scale_1 > 28) || (pre_scale_2 > 28)) && (message != S21_NaN)) {
    message = S21_MINUS_INF;
  }
  return message;
}

int s21_big_is_greater_or_equal(s21_big_decimal a, s21_big_decimal b) {
  for (int i = MAX_MANTISS_BITS - 1; i >= 0; i--) {
    if (s21_is_set_bit_s21_big_decimal(&a, i) >
        s21_is_set_bit_s21_big_decimal(&b, i))
      return 0;
  }
  // Если все биты равны, возвращаем 1 (a равно b)
  return 1;
}
int s21_big_is_not_equal(s21_big_decimal a, s21_big_decimal b) {
  for (int i = 0; i < 6; i++) {
    if (a.bits[i] != b.bits[i]) {
      return 1;  // Возвращаем 1, если хотя бы одно значение отличается
    }
  }
  return 0;
}