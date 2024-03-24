#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int scale = get_pow(value), error = 0;
  if (scale != 0) {
    int sign_op = get_sign(value);
    s21_decimal buf = {0};
    buf = value;
    div_on_ten(&buf, scale);
    *result = buf;
    if (sign_op) {
      set_sign(result, 1);
    }
  } else {
    *result = value;
  }
  return error;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int scale = get_pow(value), error = 0;
  if (scale != 0) {
    int sign_op = get_sign(value);
    s21_decimal buf = {0};
    buf = value;
    int num_after_dot = div_on_ten(&buf, scale) % 10;
    if (num_after_dot >= 5) plus_one(&buf);
    *result = buf;
    if (sign_op) {
      set_sign(result, 1);
    }
  } else {
    *result = value;
  }
  return error;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int scale = 0, error = 0;
  scale = get_pow(value);
  if (scale != 0) {
    int sign_op = get_sign(value);
    s21_decimal buf = {0};
    buf = value;
    int have_after_dot = div_on_ten(&buf, scale);
    *result = buf;
    if (sign_op) {
      if (have_after_dot >= 10) {
        plus_one(result);
      }
      set_sign(result, 1);
    }
  } else {
    *result = value;
  }
  return error;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  *result = value;
  get_sign(value) ? set_sign(result, 0) : set_sign(result, 1);
  return error;
}