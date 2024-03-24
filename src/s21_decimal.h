#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BITS 224
#define MAX_MANTISS_BITS 192
#define BITS_SIGN 127
#define MANTISS_BITS 96
#define BLOCK_BITS 32
#define MAX_FLOAT_TO_CONVERT 79228157791897854723898736640.0f
#define MAX_DEC powf(2.0f, 96.0f) - 1.0
#define MIN_DEC -powf(2.0f, 96.0f) + 1.0

#define MAX_32bit 4294967296

typedef struct s21_decimal {
  unsigned int bits[4];
} s21_decimal;

typedef struct s21_decimal_extended {
  unsigned int bits[7];
} s21_decimal_extended;

typedef union s21_decimal_bit3 {
  int bit;
  struct {
    unsigned int zero_bit2 : 16;
    unsigned int power_bit : 8;
    unsigned int zero_bit1 : 7;
    unsigned int sign_bit : 1;
  } parts;
} s21_decimal_bit3;

typedef enum s21_decimal_sign {
  S21_POS,
  S21_NEG,
} s21_decimal_sign;

typedef union float_cast {
  float fl;
  struct {
    unsigned int mantisa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  } parts;
  unsigned int bytes;
} float_cast;

typedef enum s21_arifmetic_error {
  OK,
  INF_OR_BIGGERINF,
  UNINF_OR_LESSINF,
  s21_NAN,
} s21_arifmetic_error;

void clear_decimal(s21_decimal *decimal);
int get_sign(s21_decimal decimal);
void set_sign(s21_decimal *decimal, int sign);
int get_pow(s21_decimal decimal);
void set_pow(s21_decimal *decimal, int pow);
int get_bit(s21_decimal decimal, int index);
void set_bit_to_index(s21_decimal *decimal, int index, int bit);
int is_decimal_correct(s21_decimal decimal);
int is_decimal_zero(s21_decimal *decimal);
void pow_equate_ext(s21_decimal_extended *value_1,
                    s21_decimal_extended *value_2);
int get_bit_ext(s21_decimal_extended decimal, int index);
void set_bit_to_index_ext(s21_decimal_extended *decimal, int index, int bit);
int get_pow_ext(s21_decimal_extended decimal);
void shift_left_ext(s21_decimal_extended *src, unsigned int steps);
void clear_decimal_ext(s21_decimal_extended *ext_decimal);
void copy_to_ext_decimal(s21_decimal_extended *ext_decimal,
                         s21_decimal decimal);
void copy_ext_to_decimal(s21_decimal_extended ext_decimal,
                         s21_decimal *decimal);
int is_decimal_zero_ext(s21_decimal_extended *decimal);
int first_bit_nzero(s21_decimal decimal);
void plus_one(s21_decimal *buf);
int div_on_ten(s21_decimal *buf, int scale);
void high_old_bit_ext(s21_decimal_extended v1, s21_decimal_extended v2,
                      int *bit_1, int *bit_2);
void add_extended(s21_decimal_extended value_1, s21_decimal_extended value_2,
                  s21_decimal_extended *result);
void sub_extended(s21_decimal_extended value_1, s21_decimal_extended value_2,
                  s21_decimal_extended *result);
void mul_extended(s21_decimal_extended value_1, s21_decimal_extended value_2,
                  s21_decimal_extended *result);
int div_ext(s21_decimal_extended value_1, s21_decimal_extended value_2,
            s21_decimal_extended *result);
int is_greater_ext(s21_decimal_extended value_1, s21_decimal_extended value_2);
void increase_scale_ext(s21_decimal_extended *decimal);
int shift_ext_left(s21_decimal_extended *decimal, int steps);
void shift_ext_right(s21_decimal_extended *decimal, int steps);
void div_on_ten_ext(s21_decimal_extended *decimal);
void prover_ovf(s21_decimal_extended *r, int *temp_scale, s21_decimal *result);

// округления и ниверсия
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
// преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *decimal);
int s21_from_decimal_to_int(s21_decimal src, int *decimal);
int s21_from_float_to_decimal(float src, s21_decimal *decimal);
int s21_from_decimal_to_float(s21_decimal src, float *decimal);
// сравнения
int s21_is_less(s21_decimal decim_1, s21_decimal decim_2);
int s21_is_equal(s21_decimal decim_1, s21_decimal decim_2);
int s21_is_less_or_equal(s21_decimal decim_1, s21_decimal decim_2);
int s21_is_greater(s21_decimal decim_1, s21_decimal decim_2);
int s21_is_greater_or_equal(s21_decimal decim_1, s21_decimal decim_2);
int s21_is_not_equal(s21_decimal decim_1, s21_decimal decim_2);
// арифметика
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif
