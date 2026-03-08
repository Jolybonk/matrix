#include "matrix_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Helper functions

static int check_same_size(const matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return -1;
    return (matrix_rows(m1) == matrix_rows(m2) &&
            matrix_cols(m1) == matrix_cols(m2)) ? 0 : -1;
}

// Arithmetic operations (with change of the first argument)

int matrix_add(matrix *m1, const matrix *m2) {
    if (check_same_size(m1, m2) != 0) return -1;

    size_t rows = matrix_rows(m1);
    size_t cols = matrix_cols(m1);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            *matrix_ptr(m1, i, j) += *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

int matrix_sub(matrix *m1, const matrix *m2) {
    if (check_same_size(m1, m2) != 0) return -1;

    size_t rows = matrix_rows(m1);
    size_t cols = matrix_cols(m1);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            *matrix_ptr(m1, i, j) -= *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

void matrix_smul(matrix *m, double d) {
    if (!m) return;

    size_t rows = matrix_rows(m);
    size_t cols = matrix_cols(m);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            *matrix_ptr(m, i, j) *= d;
        }
    }
}

void matrix_sdiv(matrix *m, double d) {
    if (!m || d == 0.0) return;
    matrix_smul(m, 1.0 / d);
}

// Arithmetic operations (with the result in a separate matrix)

int matrix_add2(matrix *m, const matrix *m1, const matrix *m2) {
    if (!m || !m1 || !m2) return -1;
    if (check_same_size(m1, m2) != 0) return -1;
    if (check_same_size(m, m1) != 0) return -1;

    size_t rows = matrix_rows(m);
    size_t cols = matrix_cols(m);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            *matrix_ptr(m, i, j) = *matrix_cptr(m1, i, j) + *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

int matrix_sub2(matrix *m, const matrix *m1, const matrix *m2) {
    if (!m || !m1 || !m2) return -1;
    if (check_same_size(m1, m2) != 0) return -1;
    if (check_same_size(m, m1) != 0) return -1;

    size_t rows = matrix_rows(m);
    size_t cols = matrix_cols(m);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            *matrix_ptr(m, i, j) = *matrix_cptr(m1, i, j) - *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

int matrix_smul2(matrix *m, const matrix *m1, double d) {
    if (!m || !m1) return -1;
    if (check_same_size(m, m1) != 0) return -1;

    size_t rows = matrix_rows(m);
    size_t cols = matrix_cols(m);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            *matrix_ptr(m, i, j) = *matrix_cptr(m1, i, j) * d;
        }
    }
    return 0;
}

int matrix_sdiv2(matrix *m, const matrix *m1, double d) {
    if (d == 0.0) return -1;
    return matrix_smul2(m, m1, 1.0 / d);
}

// Matrix multiplication
int matrix_mul(matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return -1;
    if (matrix_cols(m1) != matrix_rows(m2)) return -1;

    size_t rows = matrix_rows(m1);
    size_t cols = matrix_cols(m2);
    size_t inner = matrix_cols(m1);

    matrix *temp = matrix_alloc(cols, rows);
    if (!temp) return -1;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < inner; k++) {
                sum += *matrix_cptr(m1, i, k) * *matrix_cptr(m2, k, j);
            }
            *matrix_ptr(temp, i, j) = sum;
        }
    }

    matrix_assign(m1, temp);
    matrix_free(temp);

    return 0;
}

int matrix_mul2(matrix *m, const matrix *m1, const matrix *m2) {
    if (!m || !m1 || !m2) return -1;
    if (matrix_cols(m1) != matrix_rows(m2)) return -1;
    if (matrix_rows(m) != matrix_rows(m1) || matrix_cols(m) != matrix_cols(m2)) return -1;

    if (m == m1 || m == m2) {
        matrix *temp = matrix_alloc(matrix_cols(m2), matrix_rows(m1));
        if (!temp) return -1;

        int result = matrix_mul2(temp, m1, m2);
        if (result == 0) {
            matrix_assign(m, temp);
        }
        matrix_free(temp);
        return result;
    }

    size_t rows = matrix_rows(m1);
    size_t cols = matrix_cols(m2);
    size_t inner = matrix_cols(m1);

    // Regular multiplication
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < inner; k++) {
                sum += *matrix_cptr(m1, i, k) * *matrix_cptr(m2, k, j);
            }
            *matrix_ptr(m, i, j) = sum;
        }
    }

    return 0;
}

// Matrix manipulations

void matrix_transpose(matrix *m) {
    if (!m) return;

    if (matrix_rows(m) == matrix_cols(m)) {
        for (size_t i = 0; i < matrix_rows(m); i++) {
            for (size_t j = i + 1; j < matrix_cols(m); j++) {
                double temp = *matrix_ptr(m, i, j);
                *matrix_ptr(m, i, j) = *matrix_ptr(m, j, i);
                *matrix_ptr(m, j, i) = temp;
            }
        }
    } else {
        matrix *temp = matrix_transpose_new(m);
        if (temp) {
            matrix_assign(m, temp);
            matrix_free(temp);
        }
    }
}

matrix *matrix_transpose_new(const matrix *m) {
    if (!m) return NULL;

    matrix *result = matrix_alloc(matrix_rows(m), matrix_cols(m));
    if (!result) return NULL;

    for (size_t i = 0; i < matrix_rows(m); i++) {
        for (size_t j = 0; j < matrix_cols(m); j++) {
            *matrix_ptr(result, j, i) = *matrix_cptr(m, i, j);
        }
    }

    return result;
}

double matrix_norm(const matrix *m) {
    if (!m) return 0.0;

    double max_sum = 0.0;

    for (size_t i = 0; i < matrix_rows(m); i++) {
        double row_sum = 0.0;
        for (size_t j = 0; j < matrix_cols(m); j++) {
            row_sum += fabs(*matrix_cptr(m, i, j));
        }
        if (row_sum > max_sum) {
            max_sum = row_sum;
        }
    }

    return max_sum;
}

void matrix_swap_rows(matrix *m, size_t i, size_t j) {
    if (!m || i >= matrix_rows(m) || j >= matrix_rows(m) || i == j) return;

    for (size_t col = 0; col < matrix_cols(m); col++) {
        double temp = *matrix_ptr(m, i, col);
        *matrix_ptr(m, i, col) = *matrix_ptr(m, j, col);
        *matrix_ptr(m, j, col) = temp;
    }
}

void matrix_swap_cols(matrix *m, size_t i, size_t j) {
    if (!m || i >= matrix_cols(m) || j >= matrix_cols(m) || i == j) return;

    for (size_t row = 0; row < matrix_rows(m); row++) {
        double temp = *matrix_ptr(m, row, i);
        *matrix_ptr(m, row, i) = *matrix_ptr(m, row, j);
        *matrix_ptr(m, row, j) = temp;
    }
}

void matrix_scale_row(matrix *m, size_t i, double d) {
    if (!m || i >= matrix_rows(m)) return;

    for (size_t j = 0; j < matrix_cols(m); j++) {
        *matrix_ptr(m, i, j) *= d;
    }
}

void matrix_add_rows(matrix *m, size_t i, size_t j, double coeff) {
    if (!m || i >= matrix_rows(m) || j >= matrix_rows(m)) return;

    for (size_t col = 0; col < matrix_cols(m); col++) {
        *matrix_ptr(m, i, col) += coeff * *matrix_ptr(m, j, col);
    }
}
