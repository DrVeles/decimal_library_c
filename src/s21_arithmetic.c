#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (is_decimal_zero(&value_1)) clear_decimal(&value_1);
  if (is_decimal_zero(&value_2)) clear_decimal(&value_2);
  clear_decimal(result);
  int sign_res = 0, temp_scale = 0;
  s21_arifmetic_error error = OK;
  if (get_sign(value_1) && get_sign(value_2)) sign_res = S21_NEG;
  if (get_sign(value_1) != get_sign(value_2)) {
    set_bit_to_index(&value_1, BITS_SIGN, 0);
    set_bit_to_index(&value_2, BITS_SIGN, 0);
    error = (get_sign(value_1) ? s21_sub(value_2, value_1, result)
                               : s21_sub(value_1, value_2, result));
  } else if ((value_1.bits[0] == UINT_MAX && value_1.bits[1] == UINT_MAX &&
              value_1.bits[2] == UINT_MAX) &&
             (value_2.bits[0] == UINT_MAX && value_2.bits[1] == UINT_MAX &&
              value_2.bits[2] == UINT_MAX)) {
    error = INF_OR_BIGGERINF;
  } else {
    s21_decimal_extended v1 = {0}, v2 = {0}, r = {0};
    copy_to_ext_decimal(&v1, value_1);
    copy_to_ext_decimal(&v2, value_2);
    (get_pow(value_1) - get_pow(value_2)) > 0
        ? set_pow(&value_2, get_pow(value_1))
        : set_pow(&value_1, get_pow(value_2));
    temp_scale = get_pow(value_1);
    pow_equate_ext(&v1, &v2);
    add_extended(v1, v2, &r);
    prover_ovf(&r, &temp_scale, result);
    if (temp_scale <= 28 && is_decimal_zero_ext(&r)) {
      copy_ext_to_decimal(r, result);
      set_pow(result, get_pow(value_1));
    } else {
      error = INF_OR_BIGGERINF;
    }
  }
  if (sign_res == S21_NEG) set_sign(result, 1);
  if (error == INF_OR_BIGGERINF && get_sign(*result)) error = UNINF_OR_LESSINF;
  return error;
}

void add_extended(s21_decimal_extended value_1, s21_decimal_extended value_2,
                  s21_decimal_extended *result) {
  int tmp_res = 0, overflow_bit = 0, i = 0;
  for (; i < MAX_MANTISS_BITS; i++) {
    tmp_res = get_bit_ext(value_1, i) + get_bit_ext(value_2, i) + overflow_bit;
    overflow_bit = tmp_res / 2;
    tmp_res %= 2;
    set_bit_to_index_ext(result, i, tmp_res);
  }
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (is_decimal_zero(&value_1)) clear_decimal(&value_1);
  if (is_decimal_zero(&value_2)) clear_decimal(&value_2);
  s21_arifmetic_error error = OK;
  int sign_res = 0, temp_scale = 0;
  clear_decimal(result);
  if (get_sign(value_1) == S21_NEG && get_sign(value_2) == S21_NEG) {
    s21_decimal tmp_value_1 = value_1;
    value_1 = value_2;
    value_2 = tmp_value_1;
    set_bit_to_index(&value_1, BITS_SIGN, 0);
    set_bit_to_index(&value_2, BITS_SIGN, 0);
  } else if (get_sign(value_1) != get_sign(value_2)) {
    sign_res = (get_sign(value_1) ? 1 : 0);
    set_bit_to_index(&value_1, BITS_SIGN, 0);
    set_bit_to_index(&value_2, BITS_SIGN, 0);
    error = s21_add(value_1, value_2, result);
  } else {
    s21_decimal_extended v1 = {0}, v2 = {0}, r = {0};
    copy_to_ext_decimal(&v1, value_1);
    copy_to_ext_decimal(&v2, value_2);
    (get_pow(value_1) - get_pow(value_2)) > 0
        ? set_pow(&value_2, get_pow(value_1))
        : set_pow(&value_1, get_pow(value_2));
    temp_scale = get_pow(value_1);
    pow_equate_ext(&v1, &v2);
    if (is_greater_ext(v2, v1)) {
      s21_decimal_extended tmp_value_2 = v1;
      v1 = v2;
      v2 = tmp_value_2;
      set_sign(result, 1);
    }
    sub_extended(v1, v2, &r);
    prover_ovf(&r, &temp_scale, result);
    if (temp_scale <= 28) {
      copy_ext_to_decimal(r, result);
      set_pow(result, get_pow(value_1));
    } else {
      error = INF_OR_BIGGERINF;
    }
  }
  if (sign_res == 1) set_sign(result, 1);
  if (error == INF_OR_BIGGERINF && get_sign(*result)) error = UNINF_OR_LESSINF;
  return error;
}

void sub_extended(s21_decimal_extended value_1, s21_decimal_extended value_2,
                  s21_decimal_extended *result) {
  int overflow_bit = 0, tmp_res = 0, i = 0;
  for (; i < MAX_MANTISS_BITS; i++) {
    tmp_res = get_bit_ext(value_1, i) - get_bit_ext(value_2, i) - overflow_bit;
    overflow_bit = tmp_res < 0;
    tmp_res = abs(tmp_res);
    set_bit_to_index_ext(result, i, tmp_res % 2);
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (is_decimal_zero(&value_1)) clear_decimal(&value_1);
  if (is_decimal_zero(&value_2)) clear_decimal(&value_2);
  s21_arifmetic_error error = OK;
  int temp_scale = 0, sign_res = 0;
  s21_decimal_extended v1 = {0}, v2 = {0}, r = {0};
  copy_to_ext_decimal(&v1, value_1);
  copy_to_ext_decimal(&v2, value_2);
  if (get_sign(value_1) != get_sign(value_2)) sign_res = S21_NEG;
  temp_scale = get_pow(value_1) + get_pow(value_2);
  mul_extended(v1, v2, &r);
  if (is_decimal_zero_ext(&r) && temp_scale <= 28) {
    copy_ext_to_decimal(r, result);
    set_pow(result, temp_scale);
  } else if (!is_decimal_zero_ext(&r) || temp_scale > 28) {
    while (!is_decimal_zero_ext(&r) && temp_scale != 0) {
      div_on_ten_ext(&r);
      temp_scale--;
    }
  }
  if (is_decimal_zero_ext(&r)) {
    copy_ext_to_decimal(r, result);
    set_pow(result, temp_scale);
  } else {
    error = INF_OR_BIGGERINF;
  }
  if (error == INF_OR_BIGGERINF && sign_res) error = UNINF_OR_LESSINF;
  return error;
}

void mul_extended(s21_decimal_extended value_1, s21_decimal_extended value_2,
                  s21_decimal_extended *result) {
  int count = 0, i = 0;
  for (; i < MAX_MANTISS_BITS; i++) {
    if (get_bit_ext(value_2, i)) {
      shift_left_ext(&value_1, i - count);
      add_extended(value_1, *result, result);
      count = i;
    }
  }
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_arifmetic_error error = OK;
  if (!is_decimal_zero(&value_2)) {
    int scale = 0, result_scale = 0, temp_scale = 0, i = 0;
    s21_decimal_extended v1 = {0}, v2 = {0}, r = {0};
    copy_to_ext_decimal(&v1, value_1);
    copy_to_ext_decimal(&v2, value_2);
    if (get_sign(value_1) != get_sign(value_2)) {
      set_sign(result, 1);
    }
    scale = div_ext(v1, v2, &r);
    set_pow(&value_1, get_pow(value_1) + scale);
    temp_scale = get_pow(value_1) - get_pow(value_2);
    if (temp_scale > 0) {
      result_scale = 0;
    } else if (temp_scale < 0) {
      for (i = 0; i <= abs(temp_scale); i++) {
        increase_scale_ext(&r);
      }
      result_scale = 0;
    }
    if (result_scale >= 0) {
      copy_ext_to_decimal(r, result);
      set_pow(result, result_scale);
    } else {
      error = INF_OR_BIGGERINF;
    }
  } else {
    error = s21_NAN;
  }
  if (error == INF_OR_BIGGERINF && get_sign(*result)) error = UNINF_OR_LESSINF;
  return error;
}

int div_ext(s21_decimal_extended value_1, s21_decimal_extended value_2,
            s21_decimal_extended *result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0, scale = 0, diff = 0, tmp_scale = 0;
  s21_decimal_extended tmp = {0};
  high_old_bit_ext(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;
  for (int i = 0; i < MANTISS_BITS && is_decimal_zero_ext(&value_1);) {
    if (i > 0) {
      shift_ext_left(&value_2, 1);
      increase_scale_ext(result);
      increase_scale_ext(&value_1);
      tmp_scale++;
    }
    tmp_scale += scale;
    b_1 = b_2 = 0;
    high_old_bit_ext(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    if (diff < 0) diff = 0;
    for (; diff >= 0 && is_decimal_zero_ext(&value_1);) {
      if (is_greater_ext(value_2, value_1)) {
        set_bit_to_index_ext(&tmp, 0, 0);
      } else {
        sub_extended(value_1, value_2, &value_1);
        set_bit_to_index_ext(&tmp, 0, 1);
      }
      i++;
      diff--;
      if (diff >= 0) {
        shift_ext_right(&value_2, 1);
        shift_ext_left(&tmp, 1);
      }
    }
    if (diff >= 0) shift_ext_left(&tmp, diff + 1);
    shift_ext_right(&tmp, 1);
    add_extended(*result, tmp, result);
    clear_decimal_ext(&tmp);
  }
  return tmp_scale;
}