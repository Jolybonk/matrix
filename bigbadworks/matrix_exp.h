#ifndef MATRIX_EXP_H_INCLUDED
#define MATRIX_EXP_H_INCLUDED

#include "matrix.h"

/**
 * Compute matrix exponential: exp(A) = E + A + A^2/2! + A^3/3! + ...
 * @param A source matrix (square)
 * @param eps precision (stop when norm of the next term < eps)
 * @return new matrix exp(A) or NULL on error
 */
matrix *matrix_exp(const matrix *A, double eps);

#endif // MATRIX_EXP_H_INCLUDED
