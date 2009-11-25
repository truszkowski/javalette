/**
 * @brief Tablica symboli zmiennych.
 * @author Piotr Truszkowski
 */

#ifndef _J_VAR_TABLE_H_
#define _J_VAR_TABLE_H_

#include "types.h"

#include <stdlib.h>

typedef struct j_vartbl_inf_t {
	j_type_t                       type;
	j_id_t                        *id;
	j_expr_t                      *expr;
	int                            offset;
} j_vartbl_inf_t;

typedef struct j_vartbl_list_t {
	struct j_vartbl_list_t        *next;
	struct j_vartbl_inf_t          inf;
} j_vartbl_list_t;

// Tablica informacji o zmiennych
typedef struct j_vartbl_t {
	struct j_vartbl_list_t       **hash;
	size_t                         hsize;
	size_t                         nsize;
} j_vartbl_t;

/**
 * @brief Utworzenie nowej tablicy zmiennych.
 */
j_vartbl_t *j_new_vartbl(
		size_t                       size);

/**
 * @brief Zwolnienie zajmowanych zasobow.
 */
void j_delete_vartbl(
		j_vartbl_t                  *tbl);

/**
 * @brief Dodanie zmiennej do zbioru.
 * Zwraca '1' jesli udalo sie dodac lub '0' jesli 
 * taka funkcja juz istnieje w zbiorze.
 */
int j_vartbl_add(
		j_vartbl_t                  *tbl,
		j_type_t                     type,
		j_id_t                      *id,
		j_expr_t                    *expr,
		int                          offset);

/**
 * @brief Pobranie typu zapodanej zmiennej
 * Zwraca 'J_BAD_TYPE' jesli nie ma takiej
 */
j_type_t j_vartbl_get_type(
		j_vartbl_t                  *tbl, 
		size_t                       number);

/**
 * @brief Pobiranie informacji o zmiennej
 * Zwraca 'NULL' jesli nie ma takiej
 */
j_vartbl_inf_t *j_vartbl_get_inf(
		j_vartbl_t                  *tbl,
		size_t                       number);

#endif

