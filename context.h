/**
 * @brief Kontekst wystepowania zmiennych, funkcji.
 * Mozna to utozsamiac ze srodowiskiem.
 * @author Piotr Truszkowski
 */

#ifndef _J_CONTEXT_H_
#define _J_CONTEXT_H_

#include "types.h"
#include "funtbl.h"
#include "vartbl.h"

#include <stdlib.h>

typedef struct j_context_t {
	struct j_context_t            *prev;
	struct j_funtbl_t             *ftbl;
	struct j_vartbl_t             *vtbl;
	struct j_def_fun_t            *fun;
	int                            returnable;
	int                            stack;
} j_context_t;

/**
 * @brief Utworzenie kontekstu.
 */
j_context_t *j_new_context(
		j_context_t                 *prev,
		j_def_fun_t                 *fun,
		size_t                       hsize);

/**
 * @brief Zwolnienie zasobow
 */
void j_delete_context(
		j_context_t                 *ctx);

/**
 * @brief Dodanie funkcji do zbioru zdefiniowanych funkcji w tym kontekscie.
 */
int j_context_add_fun(
		j_context_t                 *ctx,
		j_def_fun_t                 *df);

/**
 * @brief Dodanie zmiennej do zbioru zdefiniowanych zmiennych w tym kontekscie.
 */
int j_context_add_var(
		j_context_t                 *ctx,
		j_type_t                     type,
		j_id_t                      *id,
		j_expr_t                    *expr,
		int                          offset);

/**
 * @brief Odszukanie funkcji o zadanym ID (sa unikalne), 'NULL' jak nie ma.
 */
j_def_fun_t *j_context_find_fun(
		j_context_t                 *ctx,
		size_t                       number);

/**
 * @brief Odszukanie typu zmiennej o zadanym ID, 'J_BAD_TYPE' jak nie ma.
 */
j_type_t j_context_find_var_type(
		j_context_t                 *ctx,
		size_t                       number);

/**
 * @brief Odszukanie informacji o zmiennej o zadanym ID, 'NULL' jaki nie ma.
 */
j_vartbl_inf_t *j_context_find_var_inf(
		j_context_t                 *ctx,
		size_t                       number,
		size_t                      *depth);

/**
 * @brief Odszukanie informacji o funkcji o zadanym ID, 'NULL' jak nie ma.
 */
j_funtbl_inf_t *j_context_find_fun_inf(
		j_context_t                 *ctx,
		size_t                       number);

/**
 * @brief Rozmiar stosu przeznaczony na bierzaca funkcje.
 */
int j_context_stack_size(
		j_context_t                 *ctx);

/**
 * @brief Odszukanie kontekstu w ktorym zmienna zostala zadeklarowana.
 */
j_context_t *j_context_find_var_home(
		j_context_t                 *ctx,
		size_t                       number);

/**
 * @brief Odszukanie informacji o zmiennej o zadanym ID, 'NULL' jak nie ma.
 * home == tutaj ma sie znalezc szukana zmienna.
 */
j_vartbl_inf_t *j_context_find_var_inf_directly(
		j_context_t                *ctx,
		j_context_t                *home,
		size_t                      number,
		size_t                     *depth);

/**
 * @brief Odszukanie kontekstu w ktorym funkcja zostala zadeklarowana.
 */
j_context_t *j_context_find_fun_home(
		j_context_t                 *ctx,
		size_t                       number);

/**
 * @brief Odszukanie informacji o funkcji o zadanym ID, 'NULL' jak nie ma.
 * home == tutaj ma sie znalezc szukana funkcja.
 */
j_funtbl_inf_t *j_context_find_fun_inf_directly(
		j_context_t                *ctx,
		j_context_t                *home,
		size_t                      number);

#endif

