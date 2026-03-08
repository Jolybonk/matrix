#ifndef MATRIX_GAUSS_H_INCLUDED
#define MATRIX_GAUSS_H_INCLUDED
#include "matrix.h"

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


#endif // MATRIX_GAUSS_H_INCLUDED
