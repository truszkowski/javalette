/**
 * @brief Tablica nazw.
 * @author Piotr Truszkowski
 */

#ifndef _J_NAMES_H_
#define _J_NAMES_H_

#include <stdlib.h>

typedef struct j_names_ent_t {
	struct j_names_ent_t          *next_by_name;
	struct j_names_ent_t          *next_by_number;
	char                          *name;
	size_t                         number;
} j_names_ent_t;

typedef struct j_names_t {
	struct j_names_ent_t         **hash_by_name;
	struct j_names_ent_t         **hash_by_number;
	size_t                         hsize;
	size_t                         nsize;
} j_names_t;

/**
 * @brief Utworzenie tablicy nazw
 */
j_names_t *j_new_names(
		size_t                       size);

/**
 * @brief Zwolnienie zasobow tablicy
 */
void j_delete_names(
		struct j_names_t            *names);

/**
 * @brief Pobranie ID dla zadanej nazwy
 * Zwraca ID a 0 gdy nie ma takiej nazwy w tablicy.
 */
size_t j_names_get(
		struct j_names_t            *names,
		const char                  *name);

/**
 * @brief Dodanie nazwy do tablicy.
 * Zwraca ID dodanej(lub juz istniejacej) nazwy.
 */
size_t j_names_add(
		struct j_names_t            *names,
		const char                  *name);

/**
 * @brief Pobranie nazwy dla zadanego numeru.
 * Zwraca 'NULL' jesli brak takiego numeru w zbiorze.
 */
const char *j_names_get_name(
		struct j_names_t            *names,
		size_t                       number);

/**
 * @brief Wartosc hasha ze stringa.
 */
size_t j_names_hash(
		const char                  *name);

#endif

