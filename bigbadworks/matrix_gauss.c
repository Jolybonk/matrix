#include "matrix_gauss.h"
#include "matrix_math.h"
#include <math.h>
#include <stdlib.h>

int matrix_gauss_solve(const matrix *A, const matrix *B, matrix *X) {
    if (!A || !B || !X) return -1;
    if (matrix_rows(A) != matrix_cols(A)) return -1;
    if (matrix_rows(A) != matrix_rows(B)) return -1;

    size_t n = matrix_rows(A);
    size_t m = matrix_cols(B);

    if (matrix_rows(X) != n || matrix_cols(X) != m) return -1;

    matrix *A_copy = matrix_copy(A);
    matrix *B_copy = matrix_copy(B);

    if (!A_copy || !B_copy) {
        matrix_free(A_copy);
        matrix_free(B_copy);
        return -1;
    }

    for (size_t k = 0; k < n; k++) {
        // Поиск главного элемента
        size_t max_row = k;
        double max_val = fabs(*matrix_cptr(A_copy, k, k));

        for (size_t i = k + 1; i < n; i++) {
            double val = fabs(*matrix_cptr(A_copy, i, k));
            if (val > max_val) {
                max_val = val;
                max_row = i;
            }
        }

        if (max_val < 1e-12) {
            matrix_free(A_copy);
            matrix_free(B_copy);
            return -1;
        }

        if (max_row != k) {
            matrix_swap_rows(A_copy, k, max_row);
            matrix_swap_rows(B_copy, k, max_row);
        }

        double divisor = *matrix_cptr(A_copy, k, k);
        matrix_scale_row(A_copy, k, 1.0 / divisor);
        matrix_scale_row(B_copy, k, 1.0 / divisor);

        for (size_t i = 0; i < n; i++) {
            if (i != k) {
                double factor = *matrix_cptr(A_copy, i, k);
                if (fabs(factor) > 1e-12) {
                    matrix_add_rows(A_copy, i, k, -factor);
                    matrix_add_rows(B_copy, i, k, -factor);
                }
            }
        }
    }

    matrix_assign(X, B_copy);

    matrix_free(A_copy);
    matrix_free(B_copy);

    return 0;
}

double matrix_residual_norm(const matrix *A, const matrix *B, const matrix *X) {
    if (!A || !B || !X) return -1.0;

    size_t n = matrix_rows(A);
    size_t m = matrix_cols(B);

    matrix *AX = matrix_alloc(m, n);
    if (!AX) return -1.0;

    if (matrix_mul2(AX, A, X) != 0) {
        matrix_free(AX);
        return -1.0;
    }

    matrix *R = matrix_alloc(m, n);
    if (!R) {
        matrix_free(AX);
        return -1.0;
    }

    matrix_sub2(R, AX, B);
    double norm = matrix_norm(R);

    matrix_free(AX);
    matrix_free(R);

    return norm;
}
