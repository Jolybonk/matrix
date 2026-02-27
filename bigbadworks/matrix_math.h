#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include "matrix.h"

// Арифметические операции (с изменением первого аргумента)

/**
 * m1 = m1 + m2
 * @return 0 при успехе, -1 если размеры не совпадают
 */
int matrix_add(matrix *m1, const matrix *m2);

/**
 * m1 = m1 - m2
 * @return 0 при успехе, -1 если размеры не совпадают
 */
int matrix_sub(matrix *m1, const matrix *m2);

/**
 * m = m * d (умножение на скаляр)
 */
void matrix_smul(matrix *m, double d);

/**
 * m = m / d (деление на скаляр)
 */
void matrix_sdiv(matrix *m, double d);

// Арифметические операции (с результатом в отдельную матрицу)

/**
 * m = m1 + m2
 * @return 0 при успехе, -1 если размеры не совпадают
 */
int matrix_add2(matrix *m, const matrix *m1, const matrix *m2);

/**
 * m = m1 - m2
 * @return 0 при успехе, -1 если размеры не совпадают
 */
int matrix_sub2(matrix *m, const matrix *m1, const matrix *m2);

/**
 * m = m1 * d
 * @return 0 при успехе, -1 если ошибка
 */
int matrix_smul2(matrix *m, const matrix *m1, double d);

/**
 * m = m1 / d
 * @return 0 при успехе, -1 если ошибка
 */
int matrix_sdiv2(matrix *m, const matrix *m1, double d);

// Умножение матриц

/**
 * m1 = m1 * m2
 * @return 0 при успехе, -1 если размеры не подходят для умножения
 */
int matrix_mul(matrix *m1, const matrix *m2);

/**
 * m = m1 * m2
 * @return 0 при успехе, -1 если размеры не подходят
 */
int matrix_mul2(matrix *m, const matrix *m1, const matrix *m2);

// Манипуляции с матрицами

/**
 * Транспонирование матрицы (для квадратной - на месте)
 * @param m матрица
 */
void matrix_transpose(matrix *m);

/**
 * Создает транспонированную копию матрицы
 * @param m исходная матрица
 * @return новая транспонированная матрица или NULL при ошибке
 */
matrix *matrix_transpose_new(const matrix *m);

/**
 * Норма матрицы: максимум по строкам суммы модулей элементов
 */
double matrix_norm(const matrix *m);

/**
 * Перестановка строк i и j
 */
void matrix_swap_rows(matrix *m, size_t i, size_t j);

/**
 * Перестановка столбцов i и j
 */
void matrix_swap_cols(matrix *m, size_t i, size_t j);

/**
 * Умножение строки i на число d
 */
void matrix_scale_row(matrix *m, size_t i, double d);

/**
 * Сложение строки i и строки j (с коэффициентом)
 * row_i = row_i + coeff * row_j
 */
void matrix_add_rows(matrix *m, size_t i, size_t j, double coeff);

// Решеаие СЛАУ методом Гаусса

/**
 * Решение системы AX = B методом Гаусса с выбором главного элемента
 * @param A матрица коэффициентов (квадратная, невырожденная)
 * @param B матрица правых частей
 * @param X матрица для результата
 * @return 0 при успехе, -1 если матрица вырожденная или ошибка
 */
int matrix_gauss_solve(const matrix *A, const matrix *B, matrix *X);

/**
 * Проверка решения: вычисляет невязку R = A*X - B
 * @param A матрица коэффициентов
 * @param B матрица правых частей
 * @param X найденное решение
 * @return норма невязки
 */
double matrix_residual_norm(const matrix *A, const matrix *B, const matrix *X);

// Матричная экспонента

/**
 * Вычисление матричной экспоненты: exp(A) = E + A + A^2/2! + A^3/3! + ...
 * @param A исходная матрица (квадратная)
 * @param eps точность (остановка когда норма очередного члена < eps)
 * @return новая матрица exp(A) или NULL при ошибке
 */
matrix *matrix_exp(const matrix *A, double eps);

#endif // MATRIX_MATH_H