#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h> 

/*
 * Модуль для работы с матрицами (вариант (c))
 * 
 * Представление: матрица хранится как одномерный массив double *data
 * Доступ к элементу [i][j]: data[i * w + j]
 * где w - ширина (количество столбцов), h - высота (количество строк)
 */

// Неполный тип для инкапсуляции
typedef struct matrix matrix;

// Создание и удаление

/**
 * Создает новую матрицу размером w×h
 * @param w количество столбцов
 * @param h количество строк
 * @return указатель на новую матрицу или NULL при ошибке выделения памяти
 * 
 * @note После использования нужно вызвать matrix_free()
 */
matrix *matrix_alloc(size_t w, size_t h);

/**
 * Создает копию матрицы
 * @param m исходная матрица
 * @return указатель на новую матрицу или NULL при ошибке
 * 
 * @note После использования нужно вызвать matrix_free()
 */
matrix *matrix_copy(const matrix *m);

/**
 * Освобождает память, занятую матрицей
 * @param m матрица (может быть NULL)
 */
void matrix_free(matrix *m);

// Доступ к элементам

/**
 * Возвращает указатель на элемент (i,j) для изменения
 * @param m матрица
 * @param i номер строки (0-based)
 * @param j номер столбца (0-based)
 * @return указатель на элемент
 * 
 * @warning Без проверки границ! i должно быть < h, j < w
 */
double *matrix_ptr(matrix *m, size_t i, size_t j);

/**
 * Возвращает константный указатель на элемент (i,j) только для чтения
 * @param m матрица
 * @param i номер строки (0-based)
 * @param j номер столбца (0-based)
 * @return константный указатель на элемент
 * 
 * @warning Без проверки границ! i должно быть < h, j < w
 */
const double *matrix_cptr(const matrix *m, size_t i, size_t j);

/**
 * Безопасное получение значения элемента
 * @param m матрица
 * @param i номер строки
 * @param j номер столбеца
 * @param result указатель для сохранения значения
 * @return 0 при успехе, -1 если индексы вне диапазона
 */
int matrix_get(const matrix *m, size_t i, size_t j, double *result);

/**
 * Безопасная установка значения элемента
 * @param m матрица
 * @param i номер строки
 * @param j номер столбца
 * @param value новое значение
 * @return 0 при успехе, -1 если индексы вне диапазона
 */
int matrix_set(matrix *m, size_t i, size_t j, double value);

// Инициализация

/**
 * Заполняет матрицу нулями
 * @param m матрица
 */
void matrix_set_zero(matrix *m);

/**
 * Превращает матрицу в единичную (квадратная!)
 * @param m матрица
 * @return указатель на m или NULL если матрица не квадратная
 * 
 * @note Если матрица не квадратная, ничего не делает и возвращает NULL
 */
matrix *matrix_set_id(matrix *m);

/**
 * Создает нулевую матрицу
 * @param w количество столбцов
 * @param h количество строк
 * @return указатель на новую матрицу или NULL при ошибке
 * 
 * @note После использования нужно вызвать matrix_free()
 */
matrix *matrix_alloc_zero(size_t w, size_t h);

/**
 * Создает единичную матрицу (квадратную)
 * @param n размер (n×n)
 * @return указатель на новую матрицу или NULL при ошибке
 * 
 * @note После использования нужно вызвать matrix_free()
 */
matrix *matrix_alloc_id(size_t n);

// Присваивание 

/**
 * Копирует содержимое src в dst
 * @param dst целевая матрица
 * @param src исходная матрица
 * @return dst при успехе, NULL если размеры не совпадают
 */
matrix *matrix_assign(matrix *dst, const matrix *src);

// Ввод/вывод 

/**
 * Ввод матрицы с клавиатуры
 * @param m матрица для заполнения
 * @return 0 при успехе, -1 при ошибке ввода
 * 
 * @note Ожидает ввод h строк по w чисел в каждой
 */
int matrix_read(matrix *m);

/**
 * Вывод матрицы на экран
 * @param m матрица
 * @param format строка формата для printf
 */
void matrix_print(const matrix *m, const char *format);

// Информация о матрице

/**
 * Возвращает количество строк
 */
size_t matrix_rows(const matrix *m);

/**
 * Возвращает количество столбцов
 */
size_t matrix_cols(const matrix *m);

#endif // MATRIX_H