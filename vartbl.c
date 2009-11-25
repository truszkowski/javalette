/**
 * @brief Tablica symboli zmiennych.
 * @author Piotr Truszkowski
 */

#include "vartbl.h"

#include <assert.h>
#include <string.h>

j_vartbl_t *j_new_vartbl(
		size_t                       size)
{
	j_vartbl_t *new = 
		(j_vartbl_t*)malloc(sizeof(j_vartbl_t));

	assert(NULL != new);

	new->hsize = size;
	new->nsize = 0;

	new->hash = 
		(j_vartbl_list_t**)malloc(sizeof(j_vartbl_list_t*)*size);

	assert(NULL != new->hash);

	bzero(new->hash, sizeof(j_vartbl_list_t*)*size);

	return new;
}

void j_delete_vartbl(
		j_vartbl_t                  *tbl)
{
	size_t i;

	for (i = 0; i < tbl->hsize; ++i) {
		j_vartbl_list_t *e = tbl->hash[i];
		while (NULL != e) {
			j_vartbl_list_t *next = e->next;
			free(e);
			e = next;
		}
	}
	
	free(tbl->hash);
	free(tbl);
}

int j_vartbl_add(
		j_vartbl_t                  *tbl,
		j_type_t                     type,
		j_id_t                      *id,
		j_expr_t                    *expr,
		int                          offset)
{
	size_t idx = id->number % tbl->hsize;
	j_vartbl_list_t *e = tbl->hash[idx];

	assert(j_type_is_numeric(type));

	while (e) {
		if (e->inf.id->number == id->number) return 0;
		e = e->next;
	}

	j_vartbl_list_t *new = 
		(j_vartbl_list_t*)malloc(sizeof(j_vartbl_list_t));

	assert(NULL != new);

	new->next = tbl->hash[idx];
	new->inf.type = type;
	new->inf.id = id;
	new->inf.expr = expr;
	new->inf.offset = offset;
	
	tbl->hash[idx] = new;
	++tbl->nsize;

	return 1;
}

j_type_t j_vartbl_get_type(
		j_vartbl_t                  *tbl,
		size_t                       number)
{
	j_vartbl_list_t *e = tbl->hash[number % tbl->hsize];

	while (e) {
		if (e->inf.id->number == number) return e->inf.type;
		e = e->next;
	}

	return J_BAD_TYPE;
}

j_vartbl_inf_t *j_vartbl_get_inf(
		j_vartbl_t                  *tbl,
		size_t                       number)
{
	j_vartbl_list_t *e = tbl->hash[number % tbl->hsize];

	while (e) {
		if (e->inf.id->number == number) return &e->inf;
		e = e->next;
	}

	return NULL;
}


