#include "s21_decimal.h"

//////////////////////////////////////////////////////////////////////
/// ДЛЯ ОБЫЧНОГО ДЕЦИМАЛА
//////////////////////////////////////////////////////////////////////

int is_decimal_zero(s21_decimal *decimal) {
  return (decimal->bits[0] == 0 && decimal->bits[1] == 0 &&
          decimal->bits[2] == 0);
}

void clear_decimal(s21_decimal *decimal) {
  decimal->bits[0] = decimal->bits[1] = decimal->bits[2] = decimal->bits[3] = 0;
}

void set_sign(s21_decimal *decimal, int sign) {
  s21_decimal_bit3 buffer;
  buffer.bit = decimal->bits[3];
  buffer.parts.sign_bit = sign;
  decimal->bits[3] = buffer.bit;
}

int get_sign(s21_decimal decimal) { return !!(decimal.bits[3] & (1u << 31)); }

int get_pow(s21_decimal decimal) {
  s21_decimal_bit3 buf;
  buf.bit = decimal.bits[3];
  int pow = buf.parts.power_bit;
  return pow;
}

void set_pow(s21_decimal *decimal, int pow) {
  s21_decimal_bit3 buf;
  buf.bit = decimal->bits[3];
  buf.parts.power_bit = pow;
  decimal->bits[3] = buf.bit;
}

int get_bit(s21_decimal decimal, int index) {
  return !!(decimal.bits[index / 32] & (1u << (index % 32)));
}

void set_bit_to_index(s21_decimal *decimal, int index, int bit) {
  if (bit) {
    decimal->bits[index / 32] = decimal->bits[index / 32] | (1u << index % 32);
  } else {
    decimal->bits[index / 32] = decimal->bits[index / 32] & ~(1u << index % 32);
  }
}

int is_decimal_correct(s21_decimal decimal) {
  int flag_check = 1;
  if (get_pow(decimal) > 28 || get_pow(decimal) < 0) {
    flag_check = 0;
  }
  s21_decimal_bit3 buf;
  buf.bit = decimal.bits[3];
  if (buf.parts.zero_bit1 != 0 || buf.parts.zero_bit2 != 0) {
    flag_check = 0;
  }
  return flag_check;
}

void plus_one(s21_decimal *buf) {
  int i = 0;
  for (; i < MANTISS_BITS; i++) {
    if (get_bit(*buf, i) == 0) {
      set_bit_to_index(buf, i, 1);
      break;
    } else {
      set_bit_to_index(buf, i, 0);
    }
  }
}

int div_on_ten(s21_decimal *buf, int scale) {
  unsigned long long fact_num;
  int j = 0, ostatok = 0, have_after_dot = 0, i = 0;
  for (; i < scale; i++) {
    j = first_bit_nzero(*buf);
    fact_num = buf->bits[j];
    for (; j >= 0; j--) {
      if (j == 0) {
        buf->bits[j] = fact_num / 10;
        ostatok = fact_num % 10;
      } else {
        ostatok = fact_num % 10;
        buf->bits[j] = fact_num / 10;
        fact_num = ostatok * (MAX_32bit) + buf->bits[j - 1];
      }
      if (ostatok != 0) {
        have_after_dot = 10;
      }
    }
  }
  buf->bits[3] = 0;
  return ostatok + have_after_dot;
}

int first_bit_nzero(s21_decimal decimal) {
  int k = 0, j = 0;
  for (j = 2; j >= 0; j--) {
    k = j;
    if (decimal.bits[j] != 0) {
      j = -1;
    }
  }
  return k;
}

//////////////////////////////////////////////////////////////////////
/// ДЛЯ РАСШИРЕННОГО ДЕЦИМАЛА
//////////////////////////////////////////////////////////////////////

void clear_decimal_ext(s21_decimal_extended *ext_decimal) {
  for (int i = 0; i < 7; i++) {
    ext_decimal->bits[i] = 0;
  }
}

int is_decimal_zero_ext(s21_decimal_extended *decimal) {
  return (decimal->bits[3] == 0 && decimal->bits[4] == 0 &&
          decimal->bits[5] == 0);
}

void pow_equate_ext(s21_decimal_extended *value_1,
                    s21_decimal_extended *value_2) {
  int pow_1 = 0, pow_2 = 0, pow_diff = 0;
  s21_decimal_extended temp_value, temp_value_sum;
  pow_1 = get_pow_ext(*value_1);
  pow_2 = get_pow_ext(*value_2);
  pow_diff = (pow_1 >= pow_2) ? pow_1 - pow_2 : pow_2 - pow_1;

  clear_decimal_ext(&temp_value);
  clear_decimal_ext(&temp_value_sum);

  for (int i = 0; i < pow_diff; i++) {
    clear_decimal_ext(&temp_value);
    clear_decimal_ext(&temp_value_sum);
    if (pow_1 >= pow_2) {
      temp_value = *value_2;
      shift_left_ext(value_2, 3);
      add_extended(*value_2, temp_value, &temp_value_sum);
      *value_2 = temp_value_sum;
      add_extended(*value_2, temp_value, &temp_value_sum);
      *value_2 = temp_value_sum;
    } else {
      temp_value = *value_1;
      shift_left_ext(value_1, 3);
      add_extended(*value_1, temp_value, &temp_value_sum);
      *value_1 = temp_value_sum;
      add_extended(*value_1, temp_value, &temp_value_sum);
      *value_1 = temp_value_sum;
    }
  }
}

void copy_to_ext_decimal(s21_decimal_extended *ext_decimal,
                         s21_decimal decimal) {
  clear_decimal_ext(ext_decimal);
  ext_decimal->bits[0] = decimal.bits[0];
  ext_decimal->bits[1] = decimal.bits[1];
  ext_decimal->bits[2] = decimal.bits[2];
  ext_decimal->bits[6] = decimal.bits[3];
}

void copy_ext_to_decimal(s21_decimal_extended ext_decimal,
                         s21_decimal *decimal) {
  clear_decimal(decimal);
  decimal->bits[0] = ext_decimal.bits[0];
  decimal->bits[1] = ext_decimal.bits[1];
  decimal->bits[2] = ext_decimal.bits[2];
  decimal->bits[3] = ext_decimal.bits[6];
}

int get_pow_ext(s21_decimal_extended decimal) {
  s21_decimal_bit3 buf;
  buf.bit = decimal.bits[6];
  int pow = buf.parts.power_bit;
  return pow;
}

int get_bit_ext(s21_decimal_extended decimal, int index) {
  return !!(decimal.bits[index / 32] & (1 << (index % 32)));
}

void set_bit_to_index_ext(s21_decimal_extended *decimal, int index, int bit) {
  if (bit) {
    decimal->bits[index / 32] =
        decimal->bits[index / 32] | (1u << (index % 32));
  } else {
    decimal->bits[index / 32] =
        decimal->bits[index / 32] & ~(1u << (index % 32));
  }
}

void shift_left_ext(s21_decimal_extended *src, unsigned int steps) {
  for (unsigned int s = 0; s < steps; s++) {
    for (int i = 191; i >= 0; i--) {
      int temp_bit = 0;
      if (i > 0) temp_bit = get_bit_ext(*src, i - 1);
      set_bit_to_index_ext(src, i, temp_bit);
    }
  }
}

int is_greater_ext(s21_decimal_extended value_1, s21_decimal_extended value_2) {
  int result = 0, out = 0;
  for (int i = 6; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i]) out = 1;
    }
  }
  return result;
}

void div_on_ten_ext(s21_decimal_extended *decimal) {
  unsigned long long carry = 0;
  for (int i = 5; i >= 0; --i) {
    unsigned long long current = (carry << 32) + decimal->bits[i];
    decimal->bits[i] = current / 10;
    carry = current % 10;
  }
}

void increase_scale_ext(s21_decimal_extended *decimal) {
  s21_decimal_extended ten = {{10, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  mul_extended(*decimal, ten, &tmp);
  *decimal = tmp;
  clear_decimal_ext(&tmp);
}

int shift_ext_left(s21_decimal_extended *decimal, int steps) {
  int error = 0, buffer[7] = {0};
  for (int k = 0; k < steps; k++) {
    for (int i = 0; i < 6; i++) {
      buffer[i] = get_bit_ext(*decimal, (i + 1) * BLOCK_BITS - 1);
    }
    for (int i = 6; i > 0 && !error; i--) {
      if (get_bit_ext(*decimal, MAX_BITS - 1)) error = 1;
      decimal->bits[i] <<= 1;
      set_bit_to_index_ext(decimal, i * BLOCK_BITS, buffer[i - 1]);
    }
    decimal->bits[0] <<= 1;
  }
  return error;
}

void shift_ext_right(s21_decimal_extended *decimal, int steps) {
  int buffer[7] = {0};
  for (int k = 0; k < steps; k++) {
    for (int i = 0; i < 6; i++) {
      buffer[i] = get_bit_ext(*decimal, (i + 1) * BLOCK_BITS);
    }
    for (int i = 0; i < 6; i++) {
      decimal->bits[i] >>= 1;
      set_bit_to_index_ext(decimal, (i + 1) * BLOCK_BITS - 1, buffer[i]);
    }
    decimal->bits[6] >>= 1;
  }
}

void high_old_bit_ext(s21_decimal_extended v1, s21_decimal_extended v2,
                      int *bit_1, int *bit_2) {
  for (int i = MAX_MANTISS_BITS - 1; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && get_bit_ext(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && get_bit_ext(v2, i)) *bit_2 = i;
  }
}

void prover_ovf(s21_decimal_extended *r, int *temp_scale, s21_decimal *result) {
  if (is_decimal_zero_ext(r) && *temp_scale <= 28) {
    copy_ext_to_decimal(*r, result);
    set_pow(result, *temp_scale);
  } else {
    while (!is_decimal_zero_ext(r) && *temp_scale > 0) {
      div_on_ten_ext(r);
      (*temp_scale)--;
    }
  }
}