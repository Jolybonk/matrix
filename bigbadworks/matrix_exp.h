#ifndef MATRIX_EXP_H_INCLUDED
#define MATRIX_EXP_H_INCLUDED

#include "matrix.h"

/**
 * ¬ычисление матричной экспоненты: exp(A) = E + A + A^2/2! + A^3/3! + ...
 * @param A исходна€ матрица (квадратна€)
 * @param eps точность (остановка когда норма очередного члена < eps)
 * @return нова€ матрица exp(A) или NULL при ошибке
 */
matrix *matrix_exp(const matrix *A, double eps);

#endif // MATRIX_EXP_H_INCLUDED
