#include "s21_decimal.h"

int s21_is_less(s21_decimal decim_1, s21_decimal decim_2) {
  int flag = 0;
  s21_decimal_bit3 bit3_1, bit3_2;
  if (is_decimal_zero(&decim_1)) {
    clear_decimal(&decim_1);
  }
  if (is_decimal_zero(&decim_2)) {
    clear_decimal(&decim_2);
  }
  bit3_1.bit = decim_1.bits[3];
  bit3_2.bit = decim_2.bits[3];
  if ((bit3_1.parts.sign_bit) < (bit3_2.parts.sign_bit)) {
    flag = 0;
  } else if ((bit3_1.parts.sign_bit) > (bit3_2.parts.sign_bit)) {
    flag = 1;
  } else {
    int for_flag = -1;
    s21_decimal_extended decimal_1_ext, decimal_2_ext;
    copy_to_ext_decimal(&decimal_1_ext, decim_1);
    copy_to_ext_decimal(&decimal_2_ext, decim_2);
    pow_equate_ext(&decimal_1_ext, &decimal_2_ext);
    for (int i = MAX_MANTISS_BITS - 1; i >= 0 && for_flag == -1; i--) {
      int temp_bit_1 = get_bit_ext(decimal_1_ext, i),
          temp_bit_2 = get_bit_ext(decimal_2_ext, i);
      if (temp_bit_1 < temp_bit_2) {
        for_flag = flag = 1;
      } else if (temp_bit_1 > temp_bit_2) {
        for_flag = flag = 0;
      }
    }
    if (bit3_1.parts.sign_bit && bit3_2.parts.sign_bit) {
      flag = !flag;
    }
  }
  return flag;
}

int s21_is_equal(s21_decimal decim_1, s21_decimal decim_2) {
  int flag = 1;
  s21_decimal_bit3 bit3_1, bit3_2;
  if (is_decimal_zero(&decim_1)) {
    clear_decimal(&decim_1);
  }
  if (is_decimal_zero(&decim_2)) {
    clear_decimal(&decim_2);
  }
  bit3_1.bit = decim_1.bits[3];
  bit3_2.bit = decim_2.bits[3];
  if (((bit3_1.parts.sign_bit) & 1u) != ((bit3_2.parts.sign_bit) & 1u)) {
    flag = 0;
  } else {
    int for_flag = -1;
    s21_decimal_extended decimal_1_ext, decimal_2_ext;
    copy_to_ext_decimal(&decimal_1_ext, decim_1);
    copy_to_ext_decimal(&decimal_2_ext, decim_2);
    pow_equate_ext(&decimal_1_ext, &decimal_2_ext);
    for (int i = MAX_MANTISS_BITS - 1; i >= 0 && for_flag == -1; i--) {
      int temp_bit_1 = get_bit_ext(decimal_1_ext, i),
          temp_bit_2 = get_bit_ext(decimal_2_ext, i);
      if (temp_bit_1 != temp_bit_2) for_flag = flag = 0;
    }
  }
  return flag;
}

int s21_is_less_or_equal(s21_decimal decim_1, s21_decimal decim_2) {
  return (s21_is_less(decim_1, decim_2)) || (s21_is_equal(decim_1, decim_2));
}

int s21_is_greater(s21_decimal decim_1, s21_decimal decim_2) {
  return (!(s21_is_less(decim_1, decim_2))) &&
         !(s21_is_equal(decim_1, decim_2));
}

int s21_is_greater_or_equal(s21_decimal decim_1, s21_decimal decim_2) {
  return (!(s21_is_less(decim_1, decim_2))) || (s21_is_equal(decim_1, decim_2));
}

int s21_is_not_equal(s21_decimal decim_1, s21_decimal decim_2) {
  return (s21_is_less(decim_1, decim_2) || s21_is_greater(decim_1, decim_2));
}
