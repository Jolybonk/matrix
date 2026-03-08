#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include "matrix.h"

// Arithmetic operations (with change of the first argument)

/**
 * m1 = m1 + m2
 * @return 0 if success, -1 if the sizes do not match
 */
int matrix_add(matrix *m1, const matrix *m2);

/**
 * m1 = m1 - m2
 * @return 0 if success, -1 if the sizes do not match
 */
int matrix_sub(matrix *m1, const matrix *m2);

/**
 * m = m * d (multiply by scalar)
 */
void matrix_smul(matrix *m, double d);

/**
 * m = m / d (division by scalar)
 */
void matrix_sdiv(matrix *m, double d);

// Arithmetic operations (with the result in a separate matrix)

/**
 * m = m1 + m2
 * @return 0 on success, -1 if the sizes do not match
 */
int matrix_add2(matrix *m, const matrix *m1, const matrix *m2);

/**
 * m = m1 - m2
 * @return 0 on success, -1 if the sizes do not match
 */
int matrix_sub2(matrix *m, const matrix *m1, const matrix *m2);

/**
 * m = m1 * d
 * @return 0 on success, -1 on error
 */
int matrix_smul2(matrix *m, const matrix *m1, double d);

/**
 * m = m1 / d
 * @return 0 on success, -1 on error
 */
int matrix_sdiv2(matrix *m, const matrix *m1, double d);

// Matrix multiplication

/**
 * m1 = m1 * m2
 * @return 0 on success, -1 if the dimensions are not suitable for multiplication
 */
int matrix_mul(matrix *m1, const matrix *m2);

/**
 * m = m1 * m2
 * @return 0 on success, -1 if the dimensions do not match
 */
int matrix_mul2(matrix *m, const matrix *m1, const matrix *m2);

// Matrix manipulations

/**
 * Matrix transposition (for square - in place)
 * @param m matrix
 */
void matrix_transpose(matrix *m);

/**
 * Creates a transposed copy of a matrix.
 * @param m is the original matrix
 * @return the new transposed matrix or NULL on error
 */
matrix *matrix_transpose_new(const matrix *m);

/**
 * Matrix norm: maximum of row sums of absolute values of elements
 */
double matrix_norm(const matrix *m);

/**
 * Swap rows i and j
 */
void matrix_swap_rows(matrix *m, size_t i, size_t j);

/**
 * Swap columns i and j
 */
void matrix_swap_cols(matrix *m, size_t i, size_t j);

/**
 * Multiply row i by scalar d
 */
void matrix_scale_row(matrix *m, size_t i, double d);

/**
 * Add row i and row j (with coefficient)
 * row_i = row_i + coeff * row_j
 */
void matrix_add_rows(matrix *m, size_t i, size_t j, double coeff);

// Solving SLAE by Gaussian elimination

/**
 * Solve system AX = B by Gaussian elimination with partial pivoting
 * @param A coefficient matrix (square, non-singular)
 * @param B right-hand side matrix
 * @param X result matrix
 * @return 0 on success, -1 if matrix is singular or error
 */
int matrix_gauss_solve(const matrix *A, const matrix *B, matrix *X);

/**
 * Check solution: compute residual R = A*X - B
 * @param A coefficient matrix
 * @param B right-hand side matrix
 * @param X computed solution
 * @return residual norm
 */
double matrix_residual_norm(const matrix *A, const matrix *B, const matrix *X);

// Matrix exponential

/**
 * Compute matrix exponential: exp(A) = E + A + A^2/2! + A^3/3! + ...
 * @param A source matrix (square)
 * @param eps precision (stop when norm of the next term < eps)
 * @return new matrix exp(A) or NULL on error
 */
matrix *matrix_exp(const matrix *A, double eps);

#endif // MATRIX_MATH_H
