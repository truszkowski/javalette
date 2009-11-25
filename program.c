/**
 * @brief Program.
 * @author Piotr Truszkowski
 */

#include "program.h"
#include "types.h"
#include "new.h"
#include "delete.h"
#include "print.h"
#include "names.h"
#include "context.h"
#include "config.h"
#include "utils.h"
#include "asm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

j_program_t *j_new_program(
		const char                  *filename)
{
	j_program_t *new =
		(j_program_t*)malloc(sizeof(j_program_t));

	assert(NULL != new);

	new->root = NULL;
	new->names = j_new_names(J_NAMES_HASH_SIZE);
	new->ctx = j_new_context(NULL, NULL, J_SYM_HASH_SIZE);
	new->filename = filename;
	new->funlist = NULL;
	new->fcount = 0;
	new->vallist = NULL;
	new->vcount = 0;
	new->ccount = 0;
	new->icount = 0;
	new->ecount = 0;

	return new;
}

void j_delete_program(
		j_program_t                 *program)
{
	if (NULL != program->root)
		j_delete_def_fun_list(program->root);

	j_delete_names(program->names);
	j_delete_context(program->ctx);
}

void j_print_program(
		j_program_t                 *program,
		FILE                        *stream)
{
	j_print_def_fun_list(program->root, stream, 0);
}

