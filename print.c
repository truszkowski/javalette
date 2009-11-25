/**
 * @brief Wypisywanie informacji o wezlach
 * @author Piotr Truszkowski
 */

#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static void j_print_indent(FILE *stream, int n)
{
	while (n > 0) 
		fprintf(stream, "  "), --n;
}

void j_print_value(
		j_value_t                   *jv,
		FILE                        *stream,
		int                          indent)
{
	(void)indent;
	switch (jv->type) {
		case J_INTEGER:
		case J_BOOLEAN:
		case J_DOUBLE:
		case J_STRING:
			fprintf(stream, "%s", jv->value); break;
		default:
			fprintf(stream, "fatal: unknown type : %d\n", jv->type);
			abort();
	}
}

void j_print_def_fun_list(
		j_def_fun_list_t            *jdfl,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);
	j_print_def_fun(jdfl->def_fun, stream, indent);

	if (jdfl->def_fun_list)
		j_print_def_fun_list(jdfl->def_fun_list, stream, indent);
}

void j_print_def_fun(
		j_def_fun_t                 *jdf,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);

	j_print_type(jdf->type, stream, indent);
	fprintf(stream, " ");
	j_print_id(jdf->id, stream, indent);
	fprintf(stream, "(");

	if (jdf->fun_args)
		j_print_fun_args(jdf->fun_args, stream, indent);
	
	fprintf(stream, ")\n");

	if (jdf->builtin) {
		j_print_indent(stream, indent+1);
		fprintf(stream, "<< builtin >>\n");
	} else {
		j_print_statement_block(jdf->block, stream, indent);
	}

	fprintf(stream, "\n");
}

void j_print_fun_args(
		j_fun_args_t                *jfa,
		FILE                        *stream,
		int                          indent)
{
	j_print_type(jfa->type, stream, indent);
	fprintf(stream, " ");
	j_print_id(jfa->id, stream, indent);
	
	if (jfa->fun_args) {
		fprintf(stream, ", ");
		j_print_fun_args(jfa->fun_args, stream, indent);
	}
}

void j_print_type(
		j_type_t                     jt,
		FILE                        *stream,
		int                          indent)
{
	(void)indent;
	
	switch (jt) {
		case J_BOOLEAN:
			fprintf(stream, "boolean"); break;
		case J_INTEGER:
			fprintf(stream, "int"    ); break;
		case J_DOUBLE:
			fprintf(stream, "double" ); break;
		case J_STRING:
			fprintf(stream, "string" ); break;
		case J_VOID:
			fprintf(stream, "void"   ); break;
		default:
			fprintf(stream, "fatal: unknown type : %d\n", jt);
			abort();
	}
}

void j_print_id(
		j_id_t                      *ji,
		FILE                        *stream,
		int                          indent)
{
	(void)indent;
	fprintf(stream, "ID%zd", ji->number);
}

void j_print_statement_block(
		j_statement_block_t         *jsb,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);
	fprintf(stream, "{\n");
	
	j_print_statement_list(jsb->list, stream, indent+1);
	
	j_print_indent(stream, indent);
	fprintf(stream, "}\n");
}

void j_print_statement_list(
		j_statement_list_t          *jsl,
		FILE                        *stream,
		int                          indent)
{
	j_print_statement(jsl->statement, stream, indent);
	if (jsl->list)
		j_print_statement_list(jsl->list, stream, indent);
}

void j_print_statement(
		j_statement_t               *js,
		FILE                        *stream,
		int                          indent)
{
	switch (js->type) {
		case J_ST_BLOCK:   j_print_statement_block(js->statement.block, stream, indent); break;
		case J_ST_DECL:    j_print_statement_decl(js->statement.decl, stream, indent); break;
		case J_ST_COND:    j_print_statement_cond(js->statement.cond, stream, indent); break;
		case J_ST_ITER:    j_print_statement_iter(js->statement.iter, stream, indent); break;
		case J_ST_RETURN:  j_print_statement_return(js->statement.ret, stream, indent); break;
		case J_ST_EXPR:    j_print_statement_expr(js->statement.expr, stream, indent); break;
		case J_ST_FUNC:    j_print_def_fun(js->statement.func, stream, indent); break;
		default:
			fprintf(stream, "fatal: unknown statement type : %d\n", js->type);
			abort();
	}
}

void j_print_idexpr(
		j_idexpr_t                  *ja,
		FILE                        *stream,
		int                          indent)
{
	j_print_id(ja->id, stream, indent);
	switch (ja->type) {
		case J_ID_ONLY:
			break;
		case J_ASSIGN:
			fprintf(stream, " = ");
			j_print_expr(ja->expr, stream, indent);
			break;
		case J_ASSIGN_DEC:
			fprintf(stream, "--");
			break;
		case J_ASSIGN_INC:
			fprintf(stream, "++");
			break;
		default:
			fprintf(stream, "fatal: unknown assign type : %d\n", ja->type); 
			abort();
	}
}

void j_print_statement_cond(
		j_statement_cond_t          *jsc,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);
	fprintf(stream, "if (");
	j_print_expr(jsc->expr, stream, indent);
	fprintf(stream, ")\n");

	if (J_ST_BLOCK == jsc->iftrue->type)
		j_print_statement(jsc->iftrue, stream, indent);
	else 
		j_print_statement(jsc->iftrue, stream, indent+1);

	if (jsc->iffalse) {
		j_print_indent(stream, indent);
		fprintf(stream, "else\n");
		if (J_ST_BLOCK == jsc->iffalse->type)
			j_print_statement(jsc->iffalse, stream, indent);
		else 
			j_print_statement(jsc->iffalse, stream, indent+1);
	}
}

void j_print_statement_iter(
		j_statement_iter_t          *jsi,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);
	if (jsi->begin) {
		assert(NULL != jsi->step);
		fprintf(stream, "for (");
		j_print_expr(jsi->begin, stream, indent);
		fprintf(stream, "; ");
		j_print_expr(jsi->expr, stream, indent);
		fprintf(stream, "; ");
		j_print_expr(jsi->step, stream, indent);
		fprintf(stream, ")\n");
	} else {
		assert(NULL == jsi->step);
		fprintf(stream, "while (");
		j_print_expr(jsi->expr, stream, indent);
		fprintf(stream, ")\n");
	}

	if (J_ST_BLOCK == jsi->statement->type)
		j_print_statement(jsi->statement, stream, indent);
	else {
		j_print_indent(stream, indent);
		fprintf(stream, "{\n");
		j_print_statement(jsi->statement, stream, indent+1);
		j_print_indent(stream, indent);
		fprintf(stream, "}\n");
	}
}

void j_print_statement_return(
		j_statement_return_t        *jsr,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);
	if (jsr->expr) {
		fprintf(stream, "return ");
		j_print_expr(jsr->expr, stream, indent);
		fprintf(stream, ";\n");
	} else {
		fprintf(stream, "return;\n");
	}
}

void j_print_statement_expr(
		j_statement_expr_t          *jse,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);
	j_print_expr(jse->expr, stream, indent);
	fprintf(stream, ";\n");
}

void j_print_statement_decl(
		j_statement_decl_t          *jsd,
		FILE                        *stream,
		int                          indent)
{
	j_print_indent(stream, indent);
	j_print_type(jsd->type, stream, indent);
	fprintf(stream, " ");
	j_print_decl_list(jsd->list, stream, indent);
	fprintf(stream, ";\n");
}

void j_print_decl_list(
		j_decl_list_t               *jdl,
		FILE                        *stream,
		int                          indent)
{
	j_print_decl(jdl->decl, stream, indent);
	if (jdl->list) {
		fprintf(stream, ", ");
		j_print_decl_list(jdl->list, stream, indent);
	} 
}

void j_print_decl(
		j_decl_t                    *jd,
		FILE                        *stream,
		int                          indent)
{
	(void)indent;
	j_print_id(jd->id, stream, indent);
	if (jd->expr) {
		fprintf(stream, " = ");
		j_print_expr(jd->expr, stream, indent);
	}
}

void j_print_function(
		j_function_t                *jf,
		FILE                        *stream,
		int                          indent)
{
	(void)indent;
	j_print_id(jf->id, stream, indent);
	fprintf(stream, "(");
	if (jf->args)
		j_print_function_args(jf->args, stream, indent);
	fprintf(stream, ")");
}

void j_print_function_args(
		j_function_args_t           *jfa,
		FILE                        *stream,
		int                          indent)
{
	(void)indent;
	j_print_expr(jfa->expr, stream, indent);
	if (jfa->args) {
		fprintf(stream, ", ");
		j_print_function_args(jfa->args, stream, indent);
	}
}

void j_print_expr(
		j_expr_t                    *je,
		FILE                        *stream,
		int                          indent)
{
	switch (je->etype) {
		case J_EXPR_VALUE:
			fprintf(stream, "(");
			j_print_value(je->value, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_FUNCTION:
			fprintf(stream, "(");
			j_print_function(je->function, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_ID:
			fprintf(stream, "(");
			j_print_idexpr(je->id, stream, indent); 
			fprintf(stream, ")");
			break;
		case J_EXPR_NOT:
			fprintf(stream, "!");
			j_print_expr(je->left, stream, indent);
			break;
		case J_EXPR_NEG:
			fprintf(stream, "(");
			fprintf(stream, "-");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_ADD:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " + ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_SUB:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " - ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_MUL:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " * ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_DIV:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " / ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_MOD:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " %% ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_AND:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " && ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_OR:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " || ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_EQ:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " == ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_NE:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " != ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_GT:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " < ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_GE:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " <= ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_LT:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " > ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_LE:
			fprintf(stream, "(");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, " >= ");
			j_print_expr(je->right, stream, indent);
			fprintf(stream, ")");
			break;
		case J_EXPR_CAST:
			fprintf(stream, "(");
			fprintf(stream, "(");
			j_print_type(je->type, stream, indent);
			fprintf(stream, ")");
			j_print_expr(je->left, stream, indent);
			fprintf(stream, ")");
			break;
		default:
			fprintf(stream, "fatal: unknown expresion type : %d\n", je->etype);
			abort();
	}
}


