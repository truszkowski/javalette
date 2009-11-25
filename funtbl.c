/**
 * @brief Tablica symboli funkcyjnych.
 * @author Piotr Truszkowski
 */

#include "funtbl.h"

#include <assert.h>
#include <string.h>

j_funtbl_t *j_new_funtbl(
		size_t                       size)
{
	j_funtbl_t *new = 
		(j_funtbl_t*)malloc(sizeof(j_funtbl_t));

	assert(NULL != new);

	new->hsize = size;
	new->nsize = 0;

	new->hash = 
		(j_funtbl_list_t**)malloc(sizeof(j_funtbl_list_t*)*size);

	assert(NULL != new->hash);

	bzero(new->hash, sizeof(j_funtbl_list_t*)*size);

	return new;
}

void j_delete_funtbl(
		j_funtbl_t                  *tbl)
{
	size_t i;

	for (i = 0; i < tbl->hsize; ++i) {
		j_funtbl_list_t *e = tbl->hash[i];
		while (NULL != e) {
			j_funtbl_list_t *next = e->next;
			free(e);
			e = next;
		}
	}
	
	free(tbl->hash);
	free(tbl);
}

int j_funtbl_add(
		j_funtbl_t                  *tbl,
		j_def_fun_t                 *fun)
{
	size_t idx = fun->id->number % tbl->hsize;
	j_funtbl_list_t *e = tbl->hash[idx];

	while (e) {
		if (e->inf.fun->id->number == fun->id->number) return 0;
		e = e->next;
	}

	j_funtbl_list_t *new = 
		(j_funtbl_list_t*)malloc(sizeof(j_funtbl_list_t));

	assert(NULL != new);

	new->next = tbl->hash[idx];
	new->inf.fun = fun;
	
	tbl->hash[idx] = new;
	++tbl->nsize;

	return 1;
}

j_def_fun_t *j_funtbl_get(
		j_funtbl_t                  *tbl,
		size_t                       number)
{
	j_funtbl_list_t *e = tbl->hash[number % tbl->hsize];

	while (e) {
		if (e->inf.fun->id->number == number) return e->inf.fun;
		e = e->next;
	}

	return NULL;
}

j_funtbl_inf_t *j_funtbl_get_inf(
		j_funtbl_t                  *tbl,
		size_t                       number)
{
	j_funtbl_list_t *e = tbl->hash[number % tbl->hsize];

	while (e) {
		if (e->inf.fun->id->number == number) return &e->inf;
		e = e->next;
	}

	return NULL;
}


