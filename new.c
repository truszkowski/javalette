/**
 * @brief Tworzenie nowych wezlow
 * @author Piotr Truszkowski
 */

#include "new.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*- Bedziemy zapamietywac miejsce wystapienia -*/

extern int rownum;
extern int colnum;

/*- Tworzenie -*/

j_id_t *j_new_id(
		size_t                       number)
{
	j_id_t *new = 
		(j_id_t*)malloc(sizeof(j_id_t));

	assert(NULL != new);

	new->number = number;
	new->row = rownum;
	new->col = colnum;
	new->ctx = NULL;

	return new;
}

j_value_t *j_new_value(
		j_type_t                     type,
		char                        *value)
{
	j_value_t *new = 
		(j_value_t*)malloc(sizeof(j_value_t));

	assert(NULL != new);

	new->type = type;
	new->value = value;
	new->row = rownum;
	new->col = colnum;
	new->list = NULL;

	return new;
}

j_def_fun_list_t *j_new_def_fun_list(
		j_def_fun_list_t            *jdfl, 
		j_def_fun_t                 *jdf)
{
	j_def_fun_list_t *new = 
		(j_def_fun_list_t*)malloc(sizeof(j_def_fun_list_t));

	assert(NULL != new);

	new->def_fun = jdf;
	new->def_fun_list = jdfl;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_def_fun_t *j_new_def_fun(
		j_type_t                     jt, 
		j_id_t                      *ji, 
		j_fun_args_t                *jfa, 
		j_statement_block_t         *jsb)
{
	j_def_fun_t *new = 
		(j_def_fun_t*)malloc(sizeof(j_def_fun_t));

	assert(NULL != new);

	new->type = jt;
	new->id = ji;
	new->fun_args = jfa;
	new->block = jsb;
	new->row = rownum;
	new->col = colnum;
	new->builtin = NULL;
	new->ctx = NULL;
	new->list = NULL;
	new->idnumber = 0;

	return new;
}

j_fun_args_t *j_new_fun_args(
		j_fun_args_t                *jfg,
		j_type_t                     jt,
		j_id_t                      *ji)
{
	j_fun_args_t *new = 
		(j_fun_args_t*)malloc(sizeof(j_fun_args_t));

	assert(NULL != new);

	new->type = jt;
	new->id = ji;
	new->fun_args = jfg;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_statement_block_t *j_new_statement_block(
		j_statement_list_t          *jsl)
{
	j_statement_block_t *new = 
		(j_statement_block_t*)malloc(sizeof(j_statement_block_t));

	assert(NULL != new);

	new->list = jsl;
	new->row = rownum;
	new->col = colnum;
	new->ctx = NULL;

	return new;
}

j_statement_list_t *j_new_statement_list(
		j_statement_list_t          *jsl,
		j_statement_t               *js)
{
	j_statement_list_t *new = 
		(j_statement_list_t*)malloc(sizeof(j_statement_list_t));

	assert(NULL != new);

	new->statement = js;
	new->list = jsl;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_statement_t *j_new_statement(
		j_statement_type_t           jst,
		void                        *ptr)
{
	j_statement_t *new = 
		(j_statement_t*)malloc(sizeof(j_statement_t));

	assert(NULL != new);

	new->type = jst;
	new->statement.ptr = ptr;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_statement_decl_t *j_new_statement_decl(
		j_type_t                     jt,
		j_decl_list_t               *jdl)
{
	j_statement_decl_t *new =
		(j_statement_decl_t*)malloc(sizeof(j_statement_decl_t));

	assert(NULL != new);

	new->type = jt;
	new->list = jdl;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_decl_list_t *j_new_decl_list(
		j_decl_t                    *jd,
		j_decl_list_t               *jdl)
{
	j_decl_list_t *new =
		(j_decl_list_t*)malloc(sizeof(j_decl_list_t));

	assert(NULL != new);

	new->decl = jd;
	new->list = jdl;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_decl_t *j_new_decl(
		j_id_t                      *ji,
		j_expr_t                    *je)
{
	j_decl_t *new = 
		(j_decl_t*)malloc(sizeof(j_decl_t));

	assert(NULL != new);

	new->id = ji;
	new->expr = je;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_function_t *j_new_function(
		j_id_t                      *ji,
		j_function_args_t           *jfa)
{
	j_function_t *new =
		(j_function_t*)malloc(sizeof(j_function_t));

	assert(NULL != new);

	new->id = ji;
	new->args = jfa;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_function_args_t *j_new_function_args(
		j_expr_t                    *je,
		j_function_args_t           *jfa)
{
	j_function_args_t *new = 
		(j_function_args_t*)malloc(sizeof(j_function_args_t));

	assert(NULL != new);

	new->expr = je;
	new->args = jfa;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_statement_return_t *j_new_statement_return(
		j_expr_t                    *je)
{
	j_statement_return_t *new = 
		(j_statement_return_t*)malloc(sizeof(j_statement_return_t));

	assert(NULL != new);

	new->expr = je;
	new->row = rownum;
	new->col = colnum;
	new->stack = 0;

	return new;
}

j_statement_cond_t *j_new_statement_cond(
		j_expr_t                     *je,
		j_statement_t                *iftrue,
		j_statement_t                *iffalse)
{
	j_statement_cond_t *new = 
		(j_statement_cond_t*)malloc(sizeof(j_statement_cond_t));

	assert(NULL != new);

	new->expr = je;
	new->iftrue = iftrue;
	new->iffalse = iffalse;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

j_statement_expr_t *j_new_statement_expr(
		j_expr_t                     *je)
{
	j_statement_expr_t *new = 
		(j_statement_expr_t*)malloc(sizeof(j_statement_expr_t));

	assert(NULL != new);

	new->expr = je;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_idexpr_t *j_new_idexpr(
		j_idexpr_type_t              type,
		j_id_t                      *ji,
		j_expr_t                    *je)
{
	j_idexpr_t *new =
		(j_idexpr_t*)malloc(sizeof(j_idexpr_t));

	assert(NULL != new);

	new->type = type;
	new->id = ji;
	new->expr = je;
	new->row = rownum;
	new->col = colnum;

	return new;
}

j_statement_iter_t *j_new_statement_iter(
		j_expr_t                    *je,
		j_statement_t               *js,
		j_expr_t                    *begin,
		j_expr_t                    *step)
{
	j_statement_iter_t *new = 
		(j_statement_iter_t*)malloc(sizeof(j_statement_iter_t));

	assert(NULL != new);

	new->expr = je;
	new->statement = js;
	new->begin = begin;
	new->step = step;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

j_expr_t *j_new_binary_expr(
		j_expr_type_t                type,
		j_expr_t                    *left,
		j_expr_t                    *right)
{
	j_expr_t *new =
		(j_expr_t*)malloc(sizeof(j_expr_t));

	assert(NULL != new);

	new->etype = type;
	new->left = left;
	new->right = right;
	new->value = NULL;
	new->function = NULL;
	new->id = NULL;
	new->type = J_BAD_TYPE;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

j_expr_t *j_new_unary_expr(
		j_expr_type_t                type,
		j_expr_t                    *expr)
{
	j_expr_t *new =
		(j_expr_t*)malloc(sizeof(j_expr_t));

	assert(NULL != new);

	new->etype = type;
	new->left = expr;
	new->right = NULL;
	new->value = NULL;
	new->function = NULL;
	new->id = NULL;
	new->type = J_BAD_TYPE;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

j_expr_t *j_new_value_expr(
		j_value_t                   *jv)
{
	j_expr_t *new =
		(j_expr_t*)malloc(sizeof(j_expr_t));

	assert(NULL != new);

	new->etype = J_EXPR_VALUE;
	new->left = NULL;
	new->right = NULL;
	new->value = jv;
	new->function = NULL;
	new->id = NULL;
	new->type = J_BAD_TYPE;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

j_expr_t *j_new_function_expr(
		j_function_t                *jf)
{
	j_expr_t *new =
		(j_expr_t*)malloc(sizeof(j_expr_t));

	assert(NULL != new);

	new->etype = J_EXPR_FUNCTION;
	new->left = NULL;
	new->right = NULL;
	new->value = NULL;
	new->function = jf;
	new->id = NULL;
	new->type = J_BAD_TYPE;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

j_expr_t *j_new_id_expr(
		j_idexpr_t                  *ji)
{
	j_expr_t *new =
		(j_expr_t*)malloc(sizeof(j_expr_t));

	assert(NULL != new);

	new->etype = J_EXPR_ID;
	new->left = NULL;
	new->right = NULL;
	new->value = NULL;
	new->function = NULL;
	new->id = ji;
	new->type = J_BAD_TYPE;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

j_expr_t *j_new_casted_expr(
		j_expr_t                    *je,
		j_type_t                     jt)
{
	j_expr_t *new =
		(j_expr_t*)malloc(sizeof(j_expr_t));

	assert(NULL != new);

	new->etype = J_EXPR_CAST;
	new->left = je;
	new->right = NULL;
	new->value = NULL;
	new->function = NULL;
	new->id = NULL;
	new->type = jt;
	new->row = rownum;
	new->col = colnum;
	new->idnumber = 0;

	return new;
}

