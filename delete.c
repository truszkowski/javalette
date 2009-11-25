/**
 * @brief Zwalnianie wezlow
 * @author Piotr Truszkowski
 */

#include "delete.h"
#include "context.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

/*- funkcje do tworzenia drzewa struktur -*/

void j_delete_id(
		j_id_t                       *ji)
{
	assert(NULL != ji);

	free(ji);
}

void j_delete_value(
		j_value_t                   *jt)
{
	assert(NULL != jt);

	free(jt->value);
	free(jt);
}

void j_delete_def_fun_list(
		j_def_fun_list_t            *jdfl)
{
	assert(NULL != jdfl);

	j_delete_def_fun(jdfl->def_fun);
	if (jdfl->def_fun_list)
		j_delete_def_fun_list(jdfl->def_fun_list);

	free(jdfl);
}

void j_delete_def_fun(
		j_def_fun_t                 *jdf)
{
	assert(NULL != jdf);

	j_delete_id(jdf->id);
	if (jdf->fun_args)
		j_delete_fun_args(jdf->fun_args);

	assert(jdf->builtin || NULL != jdf->block);

	if (jdf->block)
		j_delete_statement_block(jdf->block);

	//- Nie zwalniamy kontekstu - robi to block.

	free(jdf);
}

void j_delete_fun_args(
		j_fun_args_t                *jfa)
{
	assert(NULL != jfa);

	j_delete_id(jfa->id);
	if (jfa->fun_args)
		j_delete_fun_args(jfa->fun_args);

	free(jfa);
}

void j_delete_statement_block(
		j_statement_block_t         *jsb)
{
	assert(NULL != jsb);

	j_delete_statement_list(jsb->list);

	if (jsb->ctx) //- byla alokacja
		j_delete_context(jsb->ctx);

	free(jsb);
}

void j_delete_statement_list(
		j_statement_list_t          *jsl)
{
	assert(NULL != jsl);

	j_delete_statement(jsl->statement);
	if (jsl->list) 
		j_delete_statement_list(jsl->list);

	free(jsl);
}

void j_delete_statement(
		j_statement_t               *js)
{
	assert(NULL != js);

	switch (js->type) {
		case J_ST_BLOCK:
			j_delete_statement_block(js->statement.block); break;
		case J_ST_DECL:
			j_delete_statement_decl(js->statement.decl); break;
		case J_ST_RETURN:
			j_delete_statement_return(js->statement.ret); break;
		case J_ST_EXPR:
			j_delete_statement_expr(js->statement.expr); break;
		case J_ST_COND:
			j_delete_statement_cond(js->statement.cond); break;
		case J_ST_ITER:
			j_delete_statement_iter(js->statement.iter); break;
		case J_ST_FUNC:
			j_delete_def_fun(js->statement.func); break;
		default:
			fprintf(stderr, "fatal: unknown statement type : %d\n", js->type);
			abort();
	}

	free(js);
}

void j_delete_statement_decl(
		j_statement_decl_t          *jsd)
{
	assert(NULL != jsd);

	j_delete_decl_list(jsd->list);

	free(jsd);
}

void j_delete_decl_list(
		j_decl_list_t               *jdl)
{
	assert(NULL != jdl);

	j_delete_decl(jdl->decl);
	if (jdl->list)
		j_delete_decl_list(jdl->list);

	free(jdl);
}

void j_delete_decl(
		j_decl_t                    *jd)
{
	assert(NULL != jd);

	j_delete_id(jd->id);
	if (jd->expr)
		j_delete_expr(jd->expr);

	free(jd);
}

void j_delete_function(
		j_function_t                *jf)
{
	assert(NULL != jf);

	j_delete_id(jf->id);
	if (jf->args) 
		j_delete_function_args(jf->args);

	free(jf);
}

void j_delete_function_args(
		j_function_args_t           *jfa)
{
	assert(NULL != jfa);

	j_delete_expr(jfa->expr);
	if (jfa->args)
		j_delete_function_args(jfa->args);

	free(jfa);
}

void j_delete_expr(
		j_expr_t                    *je)
{
	assert(NULL != je);

	if (je->left) j_delete_expr(je->left);
	if (je->right) j_delete_expr(je->right);
	if (je->value) j_delete_value(je->value);
	if (je->function) j_delete_function(je->function);
	if (je->id) j_delete_idexpr(je->id);

	free(je);
}

void j_delete_statement_return(
		j_statement_return_t        *jsr)
{
	assert(NULL != jsr);

	if (jsr->expr)
		j_delete_expr(jsr->expr);

	free(jsr);
}

void j_delete_statement_cond(
		j_statement_cond_t          *jsd)
{
	assert(NULL != jsd);

	j_delete_expr(jsd->expr);
	j_delete_statement(jsd->iftrue);
	if (jsd->iffalse)
		j_delete_statement(jsd->iffalse);

	free(jsd);
}

void j_delete_statement_expr(
		j_statement_expr_t          *jse)
{
	assert(NULL != jse);

	j_delete_expr(jse->expr);

	free(jse);
}

void j_delete_idexpr(
		j_idexpr_t                  *ja)
{
	assert(NULL != ja);

	j_delete_id(ja->id);
	if (ja->expr)
		j_delete_expr(ja->expr);

	free(ja);
}

void j_delete_statement_iter(
		j_statement_iter_t          *jsi)
{
	assert(NULL != jsi);

	j_delete_expr(jsi->expr);
	j_delete_statement(jsi->statement);

	if (jsi->begin)
		j_delete_expr(jsi->begin);

	if (jsi->step)
		j_delete_expr(jsi->step);

	free(jsi);
}

