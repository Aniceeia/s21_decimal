#include "s21_compare.h"

int s21_is_compare(s21_decimal decimal1, s21_decimal decimal2) {
  s21_big_decimal big_decimal1 = {0}, big_decimal2 = {0};
  int answer = S21_EQUAL, nigate = 0, message = 0;
  int mantis1[MAX_MANTISS_BITS];
  int mantis_count1 = 1;
  int mantis2[MAX_MANTISS_BITS];
  int mantis_count2 = 1;
  message += s21_is_decimal_incorrect(decimal1);
  message += s21_is_decimal_incorrect(decimal2);

  decimal1 = s21_zero_clean(decimal1);
  decimal2 = s21_zero_clean(decimal2);

  if (s21_sign_check(decimal1, decimal2) == 0) answer = S21_FALSE;
  if (s21_sign_check(decimal1, decimal2) == 1) answer = S21_TRUE;
  if (s21_sign_check(decimal1, decimal2) == 2) nigate = 1;
  s21_decimal_to_big_decimal(decimal1, &big_decimal1);
  s21_decimal_to_big_decimal(decimal2, &big_decimal2);
  s21_normalize_big_decimal(&big_decimal1, &big_decimal2);
  for (int i = 0; i < MAX_MANTISS_BITS; i++) {
    s21_write_to_arr(mantis1, &mantis_count1, big_decimal1, i);
    s21_write_to_arr(mantis2, &mantis_count2, big_decimal2, i);
  }
  answer = s21_compare_mantiss(mantis1, &mantis_count1, mantis2, &mantis_count2,
                               answer, nigate);

  if (message != 0) answer = S21_ERROR;
  return answer;
}

int s21_compare_mantiss(int* mantis1, int* mantis_count1, int* mantis2,
                        int* mantis_count2, int answer, int nigate) {
  (*mantis_count1)--;
  (*mantis_count2)--;
  while (answer == S21_EQUAL &&
         ((*mantis_count2 != 0) || (*mantis_count1 != 0))) {
    if (nigate == 0) {
      if (mantis1[*mantis_count1] > mantis2[*mantis_count2]) answer = S21_FALSE;
      if (mantis1[*mantis_count1] < mantis2[*mantis_count2]) answer = S21_TRUE;
    } else if (nigate == 1) {
      if (mantis1[*mantis_count1] > mantis2[*mantis_count2]) answer = S21_TRUE;
      if (mantis1[*mantis_count1] < mantis2[*mantis_count2]) answer = S21_FALSE;
    }
    (*mantis_count1)--;
    (*mantis_count2)--;
  }

  return answer;
}
// записывааем число в массив чтобы в последующем читать массив задом на
// перед тоесть с старших битов к младшим
void s21_write_to_arr(int* mantis, int* mantis_count,
                      s21_big_decimal big_decimal, int i) {
  mantis[(*mantis_count)++] = s21_is_set_bit_s21_big_decimal(&big_decimal, i);
}
