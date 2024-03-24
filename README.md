# Decimal library in C

Описание на русском [тут](./README_RUS.md).

1. [Description of the group project](#description-of-the-group-project)
2. [Library Description](#description-of-the-library)
3. [Compilation and testing](#compilation-and-testing)
4. [Technical assignment](#technical-assignment)

## Description of the group project

  This project implements the decimal library in the C programming language. It is missing from the language standard.<br>
  The library is important for calculations where accuracy is important, because it does not have the error inherent in C floating point types. <br><br>
  The project was carried out by four people, my part is [comparison operators](#comparison-operators)

## Description of the library

The Decimal type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335. The default Decimal value is 0. 

The following operations on Decimal numbers are available:
- [Arithmetic operations](#arithmetic-operators)
- [Comparisons](#comparison-operators)
- [Conversions](#converters)
- [Rounding and others](#other-functions)

A decimal number is a floating-point value consisting of a sign, a numeric value where each digit is in the range from 0 to 9, and a scaling factor that indicates the position of the decimal point separating the integer and fractional parts of the numeric value.

## Compilation and testing

`s21_decimal.a` - build a clean library.

### Testing

`test` - run function tests.

`leaks` - check the tested code for leaks.

`gcov_report` - view the code test coverage.

## Technical assignment
### Binary representation

The Decimal binary representation consists of a 1-bit sign, a 96-bit integer, and a scaling factor used to divide an integer and indicate which part of it is a decimal fraction. The scaling factor is implicitly equal to the number 10, raised to a power in the range from 0 to 28.

A decimal number can be implemented as a four-element array of signed 32-bit integers (`int bits[4];`).
`bits[0]`, `bits[1]`, and `bits[2]` contain the low, middle, and high 32 bits of a 96-bit integer, respectively.

`bits[3]` contains the zoom factor and the sign, and consists of the following parts:
- Bits 0 to 15, the lowest word, are not used and must be zero;
- Bits 16 to 23 must contain an exponent from 0 to 28, which indicates the power of 10 to separate an integer;
- Bits 24 to 30 are not used and must be zero;
- Bit 31 contains a sign; 0 means positive and 1 means negative.


### Arithmetic operators

| Name of the operator | Operator | Function |
| ------ | :------: |------------------------------------------------------------------------------|
| Addition | + | int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) \ 
| Subtraction | - | int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) \ 
| Multiplication | * | int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) \ 
| Division | / | int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) \ 

The functions return an error code:

- 0 - OK
- 1 - the number is too large or equal to infinity
- 2 - the number is too small or equal to negative infinity
- 3 - division by 0

*Clarification about numbers that do not fit into the mantissa:*
- *When getting numbers that do not fit into the mantissa for arithmetic operations, use bank rounding (for example, 79,228,162,514,264,337,593,543,950,335 - 0.6 = 79,228,162,514,264,337,593,543,950,334)*

### Comparison operators

| Name of the operator | Operator | Function |
| ------ | ------ | ------ |
| Less | < | int s21_is_less(s21_decimal, s21_decimal) |
| Less than or equal to | <= | int s21_is_less_or_equal(s21_decimal, s21_decimal) |
| More | \> | int s21_is_greater(s21_decimal, s21_decimal) |
| Greater than or equal to | \>= | int s21_is_greater_or_equal(s21_decimal, s21_decimal) |
| Equal to | == | int s21_is_equal(s21_decimal, s21_decimal) |
| Not equal to | != | int s21_is_not_equal(s21_decimal, s21_decimal) |

Return value:
- 0 - FALSE
- 1 - TRUE

### Converters

| Converter | Function |
| ------ | ------ |
| From int | int s21_from_int_to_decimal(int src, s21_decimal *dst) |
| From float | int s21_from_float_to_decimal(float src, s21_decimal *dst) |
| In int | int s21_from_decimal_to_int(s21_decimal src, int *dst) |
| In float | int s21_from_decimal_to_float(s21_decimal src, float *dst) |

The return value is the error code:
- 0 - OK
- 1 - conversion error


*Clarification about the conversion of a float type number:*

- *If the numbers are too small (0 < |x| < 1e-28), return an error and a value equal to 0*
- *If the numbers are too large (|x| > 79,228,162,514,264,337,593,543,950,335) or equal to infinity, return an error*
- When processing a number with the float type, convert all significant decimal digits contained in it. If there are more than 7 such digits, then the value of the number is rounded to the nearest one, which has no more than 7 significant digits.<br>
*Clarification about converting from a decimal number to an int type:*
- *If there is a fractional part in a decimal number, then it should be discarded (for example, 0.9 is converted to 0)*

### Other functions

| Description | Function |
| ------ |----------------------------------------------------------|
| Rounds the specified Decimal number to the nearest integer towards negative infinity. | int s21_floor(s21_decimal value, s21_decimal *result) |
| Rounds Decimal to the nearest integer. | int s21_round(s21_decimal value, s21_decimal *result) |
| Returns whole digits of the specified Decimal number; any fractional digits are discarded, including trailing zeros. | int s21_truncate(s21_decimal value, s21_decimal *result) |
| Returns the result of multiplying the specified Decimal by -1. | int s21_negate(s21_decimal value, s21_decimal *result) |

The return value is the error code:

- 0 - OK
- 1 - calculation error

### Requirements

- Implement the functions described above. 
- The library should be developed in the C language of the C11 standard using the gcc compiler;
- The library code should be located in the src folder in the develop branch;
- Do not use outdated and disused language constructs and library functions. Pay attention to the legacy and obsolete marks in the official documentation on the language and the libraries used. Focus on the POSIX.1-2017 standard;
- When writing code, you must adhere to Google Style;
- Form the solution as a static library named *s21_decimal.a* (with the header file s21_decimal.h);
- The library must be developed in accordance with the principles of structured programming;
- Use the s21 prefix before each function_;
- Prepare a complete unit test coverage of the library's functions using the Check library;
- Unit tests must cover at least 80% of each function;
- Provide a Makefile for building the library and tests (with the goals of all, clean, test, s21_decimal.a, gcov_report);
- The gcov_report goal should generate a gcov report in the form of an html page. To do this, unit tests must be run with gcov flags;
- When implementing decimal, focus on the binary representation with the integer array `bits`, Observe the position of the digits of the number in the array `bits`;
- It is forbidden to use the __int128 type;
- Trailing zeros can be either left or deleted (with the exception of the `s21_truncate` function);
- The type being defined must support numbers from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335.