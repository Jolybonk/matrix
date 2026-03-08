#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

// Incomplete type - only declaration
typedef struct matrix matrix;

// All functions remain the same
matrix *matrix_alloc(size_t w, size_t h);
matrix *matrix_copy(const matrix *m);
void matrix_free(matrix *m);

double *matrix_ptr(matrix *m, size_t i, size_t j);
const double *matrix_cptr(const matrix *m, size_t i, size_t j);

int matrix_get(const matrix *m, size_t i, size_t j, double *result);
int matrix_set(matrix *m, size_t i, size_t j, double value);

void matrix_set_zero(matrix *m);
matrix *matrix_set_id(matrix *m);
matrix *matrix_alloc_zero(size_t w, size_t h);
matrix *matrix_alloc_id(size_t n);

matrix *matrix_assign(matrix *dst, const matrix *src);

int matrix_read(matrix *m);
void matrix_print(const matrix *m, const char *format);

size_t matrix_rows(const matrix *m);
size_t matrix_cols(const matrix *m);

#endif
