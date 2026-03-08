#include "matrix_exp.h"
#include "matrix_math.h"
#include <math.h>
#include <stdlib.h>

matrix *matrix_exp(const matrix *A, double eps) {
    if (!A || matrix_rows(A) != matrix_cols(A)) return NULL;

    size_t n = matrix_rows(A);

    // E + A
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
