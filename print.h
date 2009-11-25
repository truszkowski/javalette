/**
 * @brief Wypisywanie informacji o wezlach
 * @author Piotr Truszkowski
 */

#ifndef _J_PRINT_H_
#define _J_PRINT_H_

#include "types.h"

#include <stdio.h>

/*- funkcje opisujace struktury -*/

void j_print_type(
		j_type_t                     jt,
		FILE                        *stream,
		int                          indent);

void j_print_id(
		j_id_t                      *ji,
		FILE                        *stream,
		int                          indent);

void j_print_value(
		j_value_t                   *jv,
		FILE                        *stream,
		int                          indent);

void j_print_def_fun_list(
		j_def_fun_list_t            *jdfl,
		FILE                        *stream,
		int                          indent);

void j_print_def_fun(
		j_def_fun_t                 *jdf,
		FILE                        *stream,
		int                          indent);

void j_print_fun_args(
		j_fun_args_t                *jfa,
		FILE                        *stream,
		int                          indent);

void j_print_statement_block(
		j_statement_block_t         *jsb,
		FILE                        *stream,
		int                          indent);

void j_print_statement_list(
		j_statement_list_t          *jsl,
		FILE                        *stream,
		int                          indent);

void j_print_statement(
		j_statement_t               *js,
		FILE                        *stream,
		int                          indent);

void j_print_statement_decl(
		j_statement_decl_t          *jsd,
		FILE                        *stream,
		int                          indent);

void j_print_statement_cond(
		j_statement_cond_t          *jsc,
		FILE                        *stream,
		int                          indent);

void j_print_statement_iter(
		j_statement_iter_t          *jsi,
		FILE                        *stream,
		int                          indent);

void j_print_statement_return(
		j_statement_return_t        *jsr,
		FILE                        *stream,
		int                          indent);

void j_print_statement_expr(
		j_statement_expr_t          *jse,
		FILE                        *stream,
		int                          indent);

void j_print_idexpr(
		j_idexpr_t                  *ja,
		FILE                        *stream,
		int                          indent);

void j_print_decl_list(
		j_decl_list_t               *jdl,
		FILE                        *stream,
		int                          indent);

void j_print_decl(
		j_decl_t                    *jd,
		FILE                        *stream,
		int                          indent);

void j_print_expr(
		j_expr_t                    *je,
		FILE                        *stream,
		int                          indent);

void j_print_function(
		j_function_t                *jf,
		FILE                        *stream,
		int                          indent);

void j_print_function_args(
		j_function_args_t           *jfa,
		FILE                        *stream,
		int                          indent);


#endif

