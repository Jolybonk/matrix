#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Complete structure definition
struct matrix {
    double *data;
    size_t w;
    size_t h;
};

// Helper functions

// Index checking
#ifdef DEBUG
#define CHECK_INDICES(m, i, j) do { \
    assert((i) < (m)->h && (j) < (m)->w); \
} while(0)
#else
#define CHECK_INDICES(m, i, j) ((void)0)
#endif

// Creation and destruction

matrix *matrix_alloc(size_t w, size_t h) {
    if (w == 0 || h == 0) {
        return NULL;
    }

    matrix *m = (matrix*)malloc(sizeof(matrix));
    if (!m) {
        return NULL;
    }

    m->data = (double*)malloc(w * h * sizeof(double));
    if (!m->data) {
        free(m);
        return NULL;
    }

    m->w = w;
    m->h = h;

    return m;
}

matrix *matrix_copy(const matrix *m) {
    if (!m) {
        return NULL;
    }

    matrix *copy = matrix_alloc(m->w, m->h);
    if (!copy) {
        return NULL;
    }

    for (size_t i = 0; i < m->w * m->h; i++) {
        copy->data[i] = m->data[i];
    }

    return copy;
}

void matrix_free(matrix *m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

// Element access

double *matrix_ptr(matrix *m, size_t i, size_t j) {
    CHECK_INDICES(m, i, j);
    return &m->data[i * m->w + j];
}

const double *matrix_cptr(const matrix *m, size_t i, size_t j) {
    CHECK_INDICES(m, i, j);
    return &m->data[i * m->w + j];
}

int matrix_get(const matrix *m, size_t i, size_t j, double *result) {
    if (!m || !result || i >= m->h || j >= m->w) {
        return -1;
    }
    *result = m->data[i * m->w + j];
    return 0;
}

int matrix_set(matrix *m, size_t i, size_t j, double value) {
    if (!m || i >= m->h || j >= m->w) {
        return -1;
    }
    m->data[i * m->w + j] = value;
    return 0;
}

// Initialization

void matrix_set_zero(matrix *m) {
    if (!m) return;

    for (size_t i = 0; i < m->w * m->h; i++) {
        m->data[i] = 0.0;
    }
}

matrix *matrix_set_id(matrix *m) {
    if (!m || m->w != m->h) {
        return NULL;
    }

    matrix_set_zero(m);

    for (size_t i = 0; i < m->h; i++) {
        m->data[i * m->w + i] = 1.0;
    }

    return m;
}

matrix *matrix_alloc_zero(size_t w, size_t h) {
    matrix *m = matrix_alloc(w, h);
    if (m) {
        matrix_set_zero(m);
    }
    return m;
}

matrix *matrix_alloc_id(size_t n) {
    matrix *m = matrix_alloc(n, n);
    if (m) {
        matrix_set_id(m);
    }
    return m;
}

// Assignment

matrix *matrix_assign(matrix *dst, const matrix *src) {
    if (!dst || !src || dst->w != src->w || dst->h != src->h) {
        return NULL;
    }

    for (size_t i = 0; i < dst->w * dst->h; i++) {
        dst->data[i] = src->data[i];
    }

    return dst;
}

// Input/Output

int matrix_read(matrix *m) {
    if (!m) return -1;

    printf("Enter matrix elements %zux%zu:\n", m->h, m->w);
    for (size_t i = 0; i < m->h; i++) {
        printf("Row %zu: ", i + 1);
        for (size_t j = 0; j < m->w; j++) {
            if (scanf("%lf", &m->data[i * m->w + j]) != 1) {
                return -1;
            }
        }
    }

    return 0;
}

void matrix_print(const matrix *m, const char *format) {
    if (!m) {
        printf("NULL\n");
        return;
    }

    if (!format) {
        format = "%8.3f";
    }

    for (size_t i = 0; i < m->h; i++) {
        for (size_t j = 0; j < m->w; j++) {
            printf(format, m->data[i * m->w + j]);
        }
        printf("\n");
    }
}

// Matrix information

size_t matrix_rows(const matrix *m) {
    return m ? m->h : 0;
}

size_t matrix_cols(const matrix *m) {
    return m ? m->w : 0;
}
