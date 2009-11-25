/**
 * @brief Tablica nazw.
 * @author Piotr Truszkowski
 */

#include "names.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

size_t j_names_hash(
		const char                  *name)
{
	size_t hash = 0, i = 0;

	while (name[i])
		((char*)&hash)[i&0x03] ^= ((name[i]+i)&0xff), ++i;

	return hash;
}

j_names_t *j_new_names(
		size_t                       size)
{
	j_names_t *new = 
		(j_names_t*)malloc(sizeof(j_names_t));

	assert(NULL != new);

	new->hsize = size;
	new->nsize = 0;

	new->hash_by_name = //- Hash po nazwach
		(j_names_ent_t**)malloc(sizeof(j_names_t*)*size);
	new->hash_by_number = //- Hash po numerach
		(j_names_ent_t**)malloc(sizeof(j_names_t*)*size);

	assert(NULL != new->hash_by_name);
	assert(NULL != new->hash_by_number);

	bzero(new->hash_by_name, sizeof(j_names_t*)*size);
	bzero(new->hash_by_number, sizeof(j_names_t*)*size);

	return new;
}

void j_delete_names(
		struct j_names_t            *names)
{
	size_t i;
	
	for (i = 0; i < names->hsize; ++i) {
		j_names_ent_t *e = names->hash_by_name[i];
		while (NULL != e) {
			j_names_ent_t *next = e->next_by_name;
			
			free(e->name);
			free(e);
			
			e = next;
		}
	}

	free(names->hash_by_name);
	free(names->hash_by_number);
	free(names);
}

size_t j_names_get(
		struct j_names_t            *names,
		const char                  *name)
{
	j_names_ent_t *e = names->hash_by_name[j_names_hash(name) % names->hsize];

	while (e) {
		if (strcmp(e->name, name) == 0) return e->number;
		e = e->next_by_name;
	}

	return 0;
}


size_t j_names_add(
		struct j_names_t            *names,
		const char                  *name)
{
	size_t idx = j_names_hash(name) % names->hsize;
	j_names_ent_t *e = names->hash_by_name[idx];

	while (e) {
		if (strcmp(e->name, name) == 0) return e->number;
		e = e->next_by_name;
	}

	j_names_ent_t *new = 
		(j_names_ent_t*)malloc(sizeof(j_names_ent_t));
	assert(NULL != new);
		
	new->next_by_name = names->hash_by_name[idx];
	new->name = strdup(name);
	new->number = (++names->nsize);

	assert(NULL != new->name);

	names->hash_by_name[idx] = new;
	
	idx = new->number % names->hsize;
	new->next_by_number = names->hash_by_number[idx];
	names->hash_by_number[idx] = new;

	return new->number;
}

const char *j_names_get_name(
		struct j_names_t            *names,
		size_t                       number)
{
	j_names_ent_t *e = names->hash_by_number[number % names->hsize];

	while (e) {
		if (number == e->number) return e->name;
		e = e->next_by_number;
	}

	return NULL; 
}

