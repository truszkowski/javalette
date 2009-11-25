/**
 * @brief Tablica symboli funkcyjnych.
 * @author Piotr Truszkowski
 */

#ifndef _J_FUN_TABLE_H_
#define _J_FUN_TABLE_H_

#include "types.h"

#include <stdlib.h>

// Informacje o funkcji
typedef struct j_funtbl_inf_t {
	struct j_def_fun_t            *fun;
} j_funtbl_inf_t;

// Lista informacji o funkcjach
typedef struct j_funtbl_list_t {
	struct j_funtbl_list_t        *next;
	struct j_funtbl_inf_t          inf;
} j_funtbl_list_t;

// Tablica informacji o funkcjach
typedef struct j_funtbl_t {
	struct j_funtbl_list_t       **hash;
	size_t                         hsize;
	size_t                         nsize;
} j_funtbl_t;

/**
 * @brief Utworzenie nowej tablicy funkcji.
 */
j_funtbl_t *j_new_funtbl(
		size_t                       size);

/**
 * @brief Zwolnienie zajmowanych zasobow.
 */
void j_delete_funtbl(
		j_funtbl_t                  *tbl);

/**
 * @brief Dodanie funkcji do zbioru.
 * Zwraca '1' jesli udalo sie dodac lub '0' jesli 
 * taka funkcja juz istnieje w zbiorze.
 */
int j_funtbl_add(
		j_funtbl_t                  *tbl,
		j_def_fun_t                 *fun);

/**
 * @brief Pobranie definicji zapodanej funkcji.
 * Zwraca 'NULL' jesli nie ma takiej funkcji w zbiorze.
 */
j_def_fun_t *j_funtbl_get(
		j_funtbl_t                  *tbl, 
		size_t                       number);

/**
 * @brief Pobranie informacji o funkcji.
 * Zwraca 'NULL' jesli nie ma takiej funkcji w zbiorze.
 */
j_funtbl_inf_t *j_funtbl_get_inf(
		j_funtbl_t                  *tbl,
		size_t                       number);

#endif

