#include "matrix_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Вспомогательные функции

static int check_same_size(const matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return -1;
    return (matrix_rows(m1) == matrix_rows(m2) && 
            matrix_cols(m1) == matrix_cols(m2)) ? 0 : -1;
}

//Арифметические операции (с изменением первого аргумента)

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

//Арифметические операции (с результатом в отдельную матрицу)

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

//Умножение матриц
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
    
    // Обычное умножение
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

//Манипуляции с матрицами

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

//Решение СЛАУ методом Гаусса

int matrix_gauss_solve(const matrix *A, const matrix *B, matrix *X) {
    if (!A || !B || !X) return -1;
    if (matrix_rows(A) != matrix_cols(A)) return -1; 
    if (matrix_rows(A) != matrix_rows(B)) return -1; 
    
    size_t n = matrix_rows(A);
    size_t m = matrix_cols(B); 
    
    if (matrix_rows(X) != n || matrix_cols(X) != m) return -1;
    
    // Создаем копии матриц
    matrix *A_copy = matrix_copy(A);
    matrix *B_copy = matrix_copy(B);
    
    if (!A_copy || !B_copy) {
        matrix_free(A_copy);
        matrix_free(B_copy);
        return -1;
    }
    
    // Прямой ход метода Гаусса с выбором главного элемента
    for (size_t k = 0; k < n; k++) {
        size_t max_row = k;
        double max_val = fabs(*matrix_cptr(A_copy, k, k));
        
        for (size_t i = k + 1; i < n; i++) {
            double val = fabs(*matrix_cptr(A_copy, i, k));
            if (val > max_val) {
                max_val = val;
                max_row = i;
            }
        }
        
        // Проверка на вырожденность
        if (max_val < 1e-12) {
            matrix_free(A_copy);
            matrix_free(B_copy);
            return -1; 
        }
        
        // Перестановка строк
        if (max_row != k) {
            matrix_swap_rows(A_copy, k, max_row);
            matrix_swap_rows(B_copy, k, max_row);
        }
        
        // Нормализация текущей строки
        double divisor = *matrix_cptr(A_copy, k, k);
        matrix_scale_row(A_copy, k, 1.0 / divisor);
        matrix_scale_row(B_copy, k, 1.0 / divisor);
        
        // Исключение переменной из остальных строк
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

//Матричная экспонента

matrix *matrix_exp(const matrix *A, double eps) {
    if (!A || matrix_rows(A) != matrix_cols(A)) return NULL;
    
    size_t n = matrix_rows(A);
    
    matrix *result = matrix_alloc_id(n); 
    if (!result) return NULL;
    
    matrix *term = matrix_copy(A); 
    if (!term) {
        matrix_free(result);
        return NULL;
    }
    
    matrix_add(result, term);
    
    double norm_term = matrix_norm(term);
    size_t k = 2; 
    
    while (norm_term >= eps) {
        matrix *new_term = matrix_alloc(n, n);
        if (!new_term) {
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }
        
        if (matrix_mul2(new_term, term, A) != 0) {
            matrix_free(new_term);
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }
        
        matrix_sdiv(new_term, (double)k);
        
        matrix_add(result, new_term);
        
        matrix_free(term);
        term = new_term;
        
        norm_term = matrix_norm(term);
        k++;
    }
    
    matrix_free(term);
    return result;
}