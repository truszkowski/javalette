/**
 * @brief Kontekst wystepowania zmiennych, funkcji.
 * @author Piotr Truszkowski
 */

#include "context.h"

#include <assert.h>
#include <string.h>

j_context_t *j_new_context(
		j_context_t                 *prev, 
		j_def_fun_t                 *fun,
		size_t                       hsize)
{
	j_context_t *new = 
		(j_context_t*)malloc(sizeof(j_context_t));

	assert(NULL != new);

	new->ftbl = j_new_funtbl(hsize);
	new->vtbl = j_new_vartbl(hsize);
	new->prev = prev;
	new->fun = fun;
	new->returnable = 0;
	new->stack = (NULL != prev) ? prev->stack : 0;

	return new;
}

void j_delete_context(
		j_context_t                 *ctx)
{
	j_delete_funtbl(ctx->ftbl);
	j_delete_vartbl(ctx->vtbl);
	free(ctx);
}

int j_context_add_fun(
		j_context_t                 *ctx,
		j_def_fun_t                 *df)
{
	return j_funtbl_add(ctx->ftbl, df);
}

int j_context_add_var(
		j_context_t                 *ctx,
		j_type_t                     type,
		j_id_t                      *id,
		j_expr_t                    *expr,
		int                          offset)
{
	return j_vartbl_add(ctx->vtbl, type, id, expr, offset);
}

j_def_fun_t *j_context_find_fun(
		j_context_t                 *ctx,
		size_t                       number)
{
	while (ctx) { //- Przeszukujemy konteksty w dol
		j_def_fun_t *def = j_funtbl_get(ctx->ftbl, number);
		if (NULL != def) return def;
		ctx = ctx->prev;
	}

	return NULL;
}

j_type_t j_context_find_var_type(
		j_context_t                 *ctx,
		size_t                       number)
{
	while (ctx) { //- Przeszukujemy konteksty w dol
		j_type_t t = j_vartbl_get_type(ctx->vtbl, number);
		if (J_BAD_TYPE != t) return t;
		ctx = ctx->prev;
	}

	return J_BAD_TYPE;
}

j_vartbl_inf_t *j_context_find_var_inf(
		j_context_t                 *ctx,
		size_t                       number,
		size_t                      *depth)
{
	j_def_fun_t *prev = ctx->fun;
	(*depth) = 0;

	while (ctx) { //- Przeszukujemy konteksty w dol
		j_vartbl_inf_t *e = j_vartbl_get_inf(ctx->vtbl, number);
		if (NULL != e) return e;
		ctx = ctx->prev;

		if (ctx->fun != prev) {
			++(*depth);
			prev = ctx->fun;
		}
	}

	return NULL;
}

j_funtbl_inf_t *j_context_find_fun_inf(
		j_context_t                 *ctx,
		size_t                       number)
{
	while (ctx) { //- Przeszukujemy konteksty w dol
		j_funtbl_inf_t *e = j_funtbl_get_inf(ctx->ftbl, number);
		if (NULL != e) return e;
		ctx = ctx->prev;
	}

	return NULL;
}

int j_context_stack_size(
		j_context_t                 *ctx)
{
	int stack = ctx->stack;
	j_def_fun_t *curr = ctx->fun;

	while (ctx) { //- Przeszukujemy konteksty w dol
		if (ctx->fun != curr) {
			stack -= ctx->stack;
			break;
		}
		ctx = ctx->prev;
	}

	return stack;
}

j_context_t *j_context_find_var_home(
		j_context_t                 *ctx,
		size_t                       number)
{
	while (ctx) { //- Przeszukujemy konteksty w dol
		j_vartbl_inf_t *e = j_vartbl_get_inf(ctx->vtbl, number);
		if (NULL != e) return ctx;
		ctx = ctx->prev;
	}

	return NULL;
}

j_vartbl_inf_t *j_context_find_var_inf_directly(
		j_context_t                *ctx,
		j_context_t                *home,
		size_t                      number,
		size_t                     *depth)
{
	j_def_fun_t *prev = ctx->fun;
	(*depth) = 0;

	while (ctx) { //- Przeszukujemy konteksty w dol
		j_vartbl_inf_t *e = j_vartbl_get_inf(ctx->vtbl, number);
		//- Szukamy konkretnego kontekstu - home.
		if (ctx == home && NULL != e) return e;
		ctx = ctx->prev;

		if (ctx->fun != prev) {
			++(*depth);
			prev = ctx->fun;
		}
	}

	return NULL;

}

j_context_t *j_context_find_fun_home(
		j_context_t                 *ctx,
		size_t                       number)
{
	while (ctx) { //- Przeszukujemy konteksty w dol
		j_funtbl_inf_t *e = j_funtbl_get_inf(ctx->ftbl, number);
		if (NULL != e) return ctx;
		ctx = ctx->prev;
	}

	return NULL;
}

j_funtbl_inf_t *j_context_find_fun_inf_directly(
		j_context_t                *ctx,
		j_context_t                *home,
		size_t                      number)
{
	while (ctx) { //- Przeszukujemy konteksty w dol
		j_funtbl_inf_t *e = j_funtbl_get_inf(ctx->ftbl, number);
		//- Szukamy konkretnego kontekstu - home.
		if (ctx == home && NULL != e) return e;
		ctx = ctx->prev;
	}

	return NULL;
}

