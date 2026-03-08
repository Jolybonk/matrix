#ifndef MATRIX_GAUSS_H_INCLUDED
#define MATRIX_GAUSS_H_INCLUDED
#include "matrix.h"

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

#endif // MATRIX_GAUSS_H_INCLUDED
