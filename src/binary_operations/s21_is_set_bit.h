#ifndef _IS_SET_BIT_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MY_INT 8
#define MAX_BLOCK_BITS 32
#define MAX_BLOCK_NUMBER 96
#define MAX_MANTISS_BITS 192
#define MAX_INT 0xffffffff
#define MAX_FOR_32BIT 4294967296
#define RIGHT_BORDER 79228162514264337593543950335.0
#define LEFT_BORDER 1e-28
#define S21_EQUAL -1
typedef struct s21_decimal {
  unsigned bits[4];
} s21_decimal;

typedef struct s21_big_decimal {
  unsigned bits[7];
} s21_big_decimal;

typedef union s21_decimal_bit3 {
  int number;
  struct {
    unsigned zero_2 : 16;
    unsigned scale : 8;
    unsigned zero_1 : 7;
    unsigned sign : 1;
  } part;
} s21_scale_sign;

typedef union {
  float f;
  unsigned i;
} FloatUnion;
union ULLToBinary {
  unsigned long long num;
  unsigned char bytes[8];
};

typedef enum { CONVERTER_OK, CONVERTER_ERROR } conversion_error;

typedef enum other_message { S21_CALC_OK, S21_CALC_ERR } other_message;

typedef enum compare_res { S21_FALSE, S21_TRUE, S21_ERROR } compare_res;

typedef enum sign_message { S21_POSITIVE, S21_NEGATIVE } sign_message;
typedef enum arith_message {
  S21_OK,
  S21_PLUS_INF,
  S21_MINUS_INF,
  S21_NaN
} arith_message;

#define S21_IS_SET_BIT(TypeName)                                    \
  int s21_is_set_bit_##TypeName(TypeName *decimal, int index) {     \
    int blockIndex = index / MAX_BLOCK_BITS;                        \
    int bitIndexInBlock = index % MAX_BLOCK_BITS;                   \
    return !!(decimal->bits[blockIndex] & (1U << bitIndexInBlock)); \
  }
#define S21_SCALE(TypeName)                                           \
  int s21_scale_##TypeName(TypeName decimal) {                        \
    s21_scale_sign pointer;                                           \
    pointer.number =                                                  \
        decimal.bits[(int)(sizeof(TypeName) / sizeof(unsigned) - 1)]; \
    return pointer.part.scale;                                        \
  }
#define S21_DECIMAL_SET(Name)                                   \
  void s21_decimal_set_##Name(s21_decimal *decimal, int Name) { \
    s21_scale_sign pointer;                                     \
    pointer.number = decimal->bits[3];                          \
    pointer.part.Name = Name;                                   \
    decimal->bits[3] = pointer.number;                          \
  }
#define S21_DECIMAL_BIT3(Name)                       \
  int s21_decimal_bit3_##Name(s21_decimal decimal) { \
    s21_scale_sign val;                              \
    val.number = decimal.bits[3];                    \
    return val.part.Name;                            \
  }

#define S21_DIVIDE_BY_TEN(TypeName)                                          \
  int s21_divide_by_ten_##TypeName(TypeName *buffer, int scale) {            \
    int remainder = 0;                                                       \
    for (int i = 0; i < scale; i++) {                                        \
      int first_index_not_zero = s21_first_not_null_bit_##TypeName(*buffer); \
      unsigned long long current_value = buffer->bits[first_index_not_zero]; \
      for (int j = first_index_not_zero; j >= 0; j--) {                      \
        if (j == 0) {                                                        \
          buffer->bits[j] = current_value / 10;                              \
          remainder = current_value % 10;                                    \
        } else {                                                             \
          remainder = current_value % 10;                                    \
          buffer->bits[j] = current_value / 10;                              \
          current_value = remainder * MAX_FOR_32BIT + buffer->bits[j - 1];   \
        }                                                                    \
      }                                                                      \
    }                                                                        \
    buffer->bits[(int)(sizeof(TypeName) / sizeof(unsigned) - 1)] = 0;        \
    return remainder;                                                        \
  }

#define S21_FIRST_NOT_NULL_BIT(TypeName)                                 \
  int s21_first_not_null_bit_##TypeName(TypeName decimal) {              \
    int bit_num = -1;                                                    \
    for (int i = (int)(sizeof(TypeName) / sizeof(unsigned) - 2); i >= 0; \
         i--) {                                                          \
      if (decimal.bits[i] != 0) {                                        \
        bit_num = i;                                                     \
        break;                                                           \
      }                                                                  \
    }                                                                    \
    return bit_num;                                                      \
  }

#define S21_LEFT_SHIFT(TypeName)                                           \
  TypeName s21_left_shift_##TypeName(TypeName val, int shift_val) {        \
    int full_blocks_shift = shift_val / MAX_BLOCK_BITS;                    \
    int remaining_bits_shift = shift_val % MAX_BLOCK_BITS;                 \
    if (full_blocks_shift > 0) {                                           \
      for (int i = (int)(sizeof(TypeName) / sizeof(unsigned) - 2); i >= 0; \
           --i) {                                                          \
        if (i >= full_blocks_shift) {                                      \
          val.bits[i] = val.bits[i - full_blocks_shift];                   \
        } else {                                                           \
          val.bits[i] = 0;                                                 \
        }                                                                  \
      }                                                                    \
    }                                                                      \
    if (remaining_bits_shift > 0) {                                        \
      unsigned memory = 0;                                                 \
      for (int i = 0; i < (int)(sizeof(TypeName) / sizeof(unsigned) - 1);  \
           ++i) {                                                          \
        unsigned temp = val.bits[i];                                       \
        val.bits[i] <<= remaining_bits_shift;                              \
        val.bits[i] |= memory;                                             \
        memory = temp >> (MAX_BLOCK_BITS - remaining_bits_shift);          \
      }                                                                    \
    }                                                                      \
    return val;                                                            \
  }

// dec and big_dec
int s21_is_set_bit_s21_big_decimal(s21_big_decimal *decimal, int index);
int s21_is_set_bit_s21_decimal(s21_decimal *decimal, int index);
int s21_scale_s21_decimal(s21_decimal decimal);
int s21_scale_s21_big_decimal(s21_big_decimal decimal);
void s21_decimal_set_sign(s21_decimal *decimal, int sign);
void s21_decimal_set_scale(s21_decimal *decimal, int scale);
int s21_decimal_bit3_zero_1(s21_decimal decimal);
int s21_decimal_bit3_zero_2(s21_decimal decimal);
int s21_divide_by_ten_s21_big_decimal(s21_big_decimal *buffer, int scale);
int s21_divide_by_ten_s21_decimal(s21_decimal *buffer, int scale);
s21_decimal s21_left_shift_s21_decimal(s21_decimal val, int shift_val);
s21_big_decimal s21_left_shift_s21_big_decimal(s21_big_decimal val,
                                               int shift_val);
void s21_decimal_to_big_decimal(s21_decimal decimal,
                                s21_big_decimal *big_decimal);
void s21_big_decimal_to_decimal(s21_big_decimal big_decimal,
                                s21_decimal *decimal);
// dec
int s21_decimal_sign(s21_decimal decimal);
int s21_sign_check(s21_decimal decimal1, s21_decimal decimal2);
int s21_mul_sign_check(s21_decimal value_1, s21_decimal value_2, int sign);
unsigned s21_init_bit(unsigned val, int index);
unsigned s21_reset_bit(unsigned val, int index);
void s21_decimal_set_zeroes(s21_decimal *decimal);
void s21_recording_units(s21_decimal value, int *pos, int *counter);
int s21_if_zero_clear(s21_decimal *value_1, s21_decimal *value_2);
int s21_mul_dec_correct(s21_decimal value_1, s21_decimal value_2, int message,
                        int pre_scale_1, int pre_scale_2);
s21_decimal s21_dabs(s21_decimal decimal);
s21_decimal s21_zero_clean(s21_decimal decimal);
s21_decimal s21_decimal_set_inf(void);
// big_dec
void s21_big_decimal_bit_init(s21_big_decimal *big_decimal, int index, int bit);
void s21_add_big_mantiss(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result);
void s21_sub_big_mantiss(s21_big_decimal big_decimal1,
                         s21_big_decimal big_decimal2, s21_big_decimal *result);
void s21_mul_on_ten_big_decimal(s21_big_decimal *big_decimal);
void s21_normalize_big_decimal(s21_big_decimal *value_1,
                               s21_big_decimal *value_2);
void s21_big_decimal_swap(s21_big_decimal *decimal1, s21_big_decimal *decimal2);
int s21_mul_round(s21_big_decimal value, s21_big_decimal *result, int *scale);
void s21_bank_round(s21_big_decimal value, s21_big_decimal *result);
// correct check
int s21_is_decimal_mantiss_zero(s21_decimal decimal);
int s21_decimals_overflow(s21_decimal decimal);
int s21_is_decimal_incorrect(s21_decimal decimal);
int s21_is_big_decimal_fits_in_decimal(s21_big_decimal *big_decimal);
// converter
int s21_get_exponent_float(float f);
void s21_count_int_in_float(float src, int *count);
int s21_plus_or_tvar_drozhachaya(float num);
int s21_get_first_digits(float number, int *second_mno);
int s21_hanging_zero(float tmp, s21_decimal *dst);
int s21_handling_edge_cases_converter(float src, s21_decimal *dst,
                                      int *flag_hang_zero);
// s21_div
s21_decimal s21_decimal_init_bit(s21_decimal decimal, int index);

long double s21_binary_to_double(const char *binary, int *scale_frac);
s21_decimal s21_set_bit_string(s21_decimal value, const char *str, int start,
                               int end);

s21_decimal s21_decimal_right_shift(s21_decimal val, int shift_val);

// new
s21_big_decimal s21_big_decimal_init_bit(s21_big_decimal decimal, int index);

s21_big_decimal s21_big_decimal_right_shift(s21_big_decimal val, int shift_val);
int get_bit(unsigned value, int bit_index);
int s21_big_is_greater_or_equal(s21_big_decimal a, s21_big_decimal b);
int s21_big_is_not_equal(s21_big_decimal a, s21_big_decimal b);

#endif  //_IS_SET_BIT_H_