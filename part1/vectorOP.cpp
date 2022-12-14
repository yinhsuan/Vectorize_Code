#include "PPintrin.h"

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  //  Because "_pp_vload", "_pp_vlt", "_pp_vsub" process #VECTOR_WIDTH data at a time in the template
  //  And the code did not check whether (N % verifyResult) != 0 before assign maskAll with _pp_init_ones()
  //  And in verifyResult(), it will compare the number until verifyResult no matter what N is given ()
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

void clampedExpVector(float *values, int *exponents, float *output, int N)
{
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //

  __pp_vec_float val, result;
  __pp_vec_int exp;
  __pp_vec_int zero = _pp_vset_int(0);
  __pp_vec_int one = _pp_vset_int(1);
  __pp_vec_float onef = _pp_vset_float(1.f);
  __pp_vec_float clampf = _pp_vset_float(9.999999f);
  __pp_mask maskAll, isToMul, notToMul, isToClamp, initVal;

  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    maskAll = _pp_init_ones(); // all ones
    isToMul = _pp_init_ones(0); // all zeros
    notToMul = _pp_init_ones(0);
    isToClamp = _pp_init_ones(0);
    initVal = _pp_init_ones(N%VECTOR_WIDTH);
    result = _pp_vset_float(1.f);
    val = _pp_vset_float(0.f);
    exp = _pp_vset_int(1); // set to 1 but 0

    // handle (N % verifyResult) != 0
    if (i + VECTOR_WIDTH > N) {
      _pp_vload_float(val, values + i, initVal); // float x = values[i];
      _pp_vload_int(exp, exponents + i, initVal); // int y = exponents[i];
    }
    else {
      _pp_vload_float(val, values + i, maskAll); // float x = values[i];
      _pp_vload_int(exp, exponents + i, maskAll); // int y = exponents[i];
    }

    _pp_veq_int(notToMul, exp, zero, maskAll); // if (y == 0) {
    _pp_vstore_float(output + i, onef, notToMul); // output[i] = 1.f; }

    _pp_vgt_int(isToMul, exp, zero, maskAll); // else { => exp[i] > 0
    while (_pp_cntbits(isToMul) > 0) { // while (count > 0) {
      _pp_vmult_float(result, result, val, isToMul); // result *= x;
      _pp_vsub_int(exp, exp, one, maskAll); // count--; }
      _pp_vgt_int(isToMul, exp, zero, maskAll); // update isToMul
    }
    isToMul = _pp_mask_not(notToMul);
    _pp_vgt_float(isToClamp, result, clampf, isToMul); // if (result > 9.999999f) {
    _pp_vmove_float(result, clampf, isToClamp); // result = 9.999999f; }
    _pp_vstore_float(output + i, result, isToMul); // output[i] = result; }
  }
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N)
{

  //
  // PP STUDENTS TODO: Implement your vectorized version of arraySumSerial here
  //

  float sum = 0.f;
  int window;
  __pp_vec_float val, result;
  __pp_mask maskAll;

  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    window = VECTOR_WIDTH;
    maskAll = _pp_init_ones();

    _pp_vload_float(val, values + i, maskAll);
    _pp_vmove_float(result, val, maskAll);

    while (window > 1) {
      _pp_hadd_float(result, result);
      _pp_interleave_float(result, result);
      window /= 2;
    }
    sum += result.value[0];
  }

  return sum;
}