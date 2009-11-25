/**
 * @brief Zwalnianie wezlow
 * @author Piotr Truszkowski
 */

#ifndef _J_DELETE_H_
#define _J_DELETE_H_

#include "types.h"

/*- funkcje do tworzenia drzewa struktur -*/

void j_delete_id(
		j_id_t                      *ji);

void j_delete_value(
		j_value_t                   *jt);

void j_delete_def_fun_list(
		j_def_fun_list_t            *jdfl);

void j_delete_def_fun(
		j_def_fun_t                 *jdf);

void j_delete_fun_args(
		j_fun_args_t                *jfa);

void j_delete_statement_block(
		j_statement_block_t         *jsb);

void j_delete_statement_list(
		j_statement_list_t          *jsl);

void j_delete_statement(
		j_statement_t               *js);

void j_delete_statement_decl(
		j_statement_decl_t          *jsd);

void j_delete_decl_list(
		j_decl_list_t               *jdl);

void j_delete_decl(
		j_decl_t                    *jd);

void j_delete_function(
		j_function_t                *jf);

void j_delete_function_args(
		j_function_args_t           *jfa);

void j_delete_expr(
		j_expr_t                    *je);

void j_delete_statement_return(
		j_statement_return_t        *jsr);

void j_delete_statement_cond(
		j_statement_cond_t          *jsd);

void j_delete_statement_expr(
		j_statement_expr_t          *jse);

void j_delete_idexpr(
		j_idexpr_t                  *ja);

void j_delete_statement_iter(
		j_statement_iter_t          *jsi);

#endif

