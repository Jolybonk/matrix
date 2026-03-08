#include "matrix.h"
#include "matrix_math.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Функция для теста матричной экспоненты
void test_matrix_exp() {
    printf("\nMATRIX EXPONENTIAL TEST \n");

    // Создаем диагональную матрицу
    matrix *A = matrix_alloc_id(3);
    *matrix_ptr(A, 0, 0) = 1.0;
    *matrix_ptr(A, 1, 1) = 2.0;
    *matrix_ptr(A, 2, 2) = -1.0;

    printf("Original matrix A:\n");
    matrix_print(A, "%6.2f");

    matrix *expA = matrix_exp(A, 1e-10);
    if (expA) {
        printf("\nexp(A):\n");
        matrix_print(expA, "%10.6f");

        // Теоретический результат
        printf("\nTheoretical result (for verification):\n");
        printf("  %8.6f  0.000000  0.000000\n", exp(1.0));
        printf("  0.000000 %8.6f  0.000000\n", exp(2.0));
        printf("  0.000000  0.000000 %8.6f\n", exp(-1.0));

        matrix_free(expA);
    }

    matrix_free(A);
}

// Функция для теста метода Гаусса
void test_gauss_solve() {
    printf("\nGAUSS METHOD TEST\n");

    matrix *A = matrix_alloc(3, 3);
    *matrix_ptr(A, 0, 0) = 2;   *matrix_ptr(A, 0, 1) = 1;   *matrix_ptr(A, 0, 2) = -1;
    *matrix_ptr(A, 1, 0) = -3;  *matrix_ptr(A, 1, 1) = -1;  *matrix_ptr(A, 1, 2) = 2;
    *matrix_ptr(A, 2, 0) = -2;  *matrix_ptr(A, 2, 1) = 1;   *matrix_ptr(A, 2, 2) = 2;

    matrix *B = matrix_alloc(1, 3);
    *matrix_ptr(B, 0, 0) = 8;
    *matrix_ptr(B, 1, 0) = -11;
    *matrix_ptr(B, 2, 0) = -3;

    matrix *X = matrix_alloc(1, 3);

    printf("Coefficient matrix A:\n");
    matrix_print(A, "%6.2f");
    printf("\nRight-hand side vector B:\n");
    matrix_print(B, "%6.2f");

    int result = matrix_gauss_solve(A, B, X);

    if (result == 0) {
        printf("\nSolution X:\n");
        matrix_print(X, "%10.6f");

        // Проверяем решение
        double residual = matrix_residual_norm(A, B, X);
        printf("\nResidual norm ||A*X - B|| = %e\n", residual);

        printf("\nExpected solution: x = 2, y = 3, z = -1\n");
    } else {
        printf("\nError solving system!\n");
    }

    matrix_free(A);
    matrix_free(B);
    matrix_free(X);
}

// Функция для теста арифметических операций
void test_arithmetic() {
    printf("\n ARITHMETIC OPERATIONS TEST \n");

    matrix *A = matrix_alloc(2, 2);
    matrix *B = matrix_alloc(2, 2);

    *matrix_ptr(A, 0, 0) = 1; *matrix_ptr(A, 0, 1) = 2;
    *matrix_ptr(A, 1, 0) = 3; *matrix_ptr(A, 1, 1) = 4;

    *matrix_ptr(B, 0, 0) = 5; *matrix_ptr(B, 0, 1) = 6;
    *matrix_ptr(B, 1, 0) = 7; *matrix_ptr(B, 1, 1) = 8;

    printf("Matrix A:\n");
    matrix_print(A, "%5.0f");
    printf("\nMatrix B:\n");
    matrix_print(B, "%5.0f");

    // Сложение
    matrix *C = matrix_alloc(2, 2);
    matrix_add2(C, A, B);
    printf("\nA + B:\n");
    matrix_print(C, "%5.0f");

    // Умножение на скаляр
    matrix *D = matrix_alloc(2, 2);
    matrix_smul2(D, A, 2.5);
    printf("\nA * 2.5:\n");
    matrix_print(D, "%8.2f");

    // Умножение матриц
    matrix *E = matrix_alloc(2, 2);
    matrix_mul2(E, A, B);
    printf("\nA * B:\n");
    matrix_print(E, "%5.0f");

    // Транспонирование
    matrix_transpose(A);
    printf("\nTransposed A:\n");
    matrix_print(A, "%5.0f");

    // Норма матрицы
    printf("\nNorm of matrix B: %f\n", matrix_norm(B));

    matrix_free(A);
    matrix_free(B);
    matrix_free(C);
    matrix_free(D);
    matrix_free(E);
}
//Отдельный тест системы для Гаусса
void test_gauss_special() {
    printf("\nGAUSS METHOD TEST (SPECIAL SYSTEM)\n");

    // Система:
    // 0x + 0y + 1z = 1
    // 0x + 1y + 0z = 2
    // 1x + 0y + 0z = 3

    matrix *A = matrix_alloc(3, 3);
    *matrix_ptr(A, 0, 0) = 0; *matrix_ptr(A, 0, 1) = 0; *matrix_ptr(A, 0, 2) = 1;
    *matrix_ptr(A, 1, 0) = 0; *matrix_ptr(A, 1, 1) = 1; *matrix_ptr(A, 1, 2) = 0;
    *matrix_ptr(A, 2, 0) = 1; *matrix_ptr(A, 2, 1) = 0; *matrix_ptr(A, 2, 2) = 0;

    matrix *B = matrix_alloc(1, 3);
    *matrix_ptr(B, 0, 0) = 1;
    *matrix_ptr(B, 1, 0) = 2;
    *matrix_ptr(B, 2, 0) = 3;

    matrix *X = matrix_alloc(1, 3);

    printf("Coefficient matrix A:\n");
    matrix_print(A, "%6.2f");
    printf("\nRight-hand side B:\n");
    matrix_print(B, "%6.2f");

    int result = matrix_gauss_solve(A, B, X);

    if (result == 0) {
        printf("\nSolution X:\n");
        matrix_print(X, "%10.6f");

        double residual = matrix_residual_norm(A, B, X);
        printf("\nResidual norm ||A*X - B|| = %e\n", residual);

        printf("\nExpected solution: x = 3, y = 2, z = 1\n");
    } else {
        printf("\nError solving system!\n");
    }

    matrix_free(A);
    matrix_free(B);
    matrix_free(X);
}

int main() {
    printf("MATRIX LIBRARY TESTING\n");

    // Тест 1: Базовые операции
    printf("\nPART 1: Basic matrix operations\n");
    printf("--------------------------------------\n");

    matrix *a = matrix_alloc(3, 3);
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            *matrix_ptr(a, i, j) = i * 3 + j + 1;
        }
    }

    printf("Matrix a:\n");
    matrix_print(a, "%5.0f");

    matrix *id = matrix_alloc_id(3);
    printf("\nIdentity matrix:\n");
    matrix_print(id, "%5.0f");

    matrix *copy = matrix_copy(a);
    printf("\nCopy of matrix a:\n");
    matrix_print(copy, "%5.0f");

    double val;
    if (matrix_get(a, 1, 1, &val) == 0) {
        printf("\nElement [1][1] = %f\n", val);
    }

    matrix_free(a);
    matrix_free(id);
    matrix_free(copy);

    // Тест 2: Арифметические операции
    test_arithmetic();

    // Тест 3: Матричная экспонента
    test_matrix_exp();

    // Тест 4: Решение СЛАУ методом Гаусса
    test_gauss_solve();
    test_gauss_special();

    printf("ALL TESTS COMPLETED\n");
    getchar();

    return 0;
}
