/**
 * @brief Tworzenie nowych wezlow
 * @author Piotr Truszkowski
 */

#ifndef _J_NEW_H_
#define _J_NEW_H_

#include "types.h"

/*- funkcje do tworzenia drzewa struktur -*/

j_def_fun_list_t *j_new_def_fun_list(
		j_def_fun_list_t            *jdfl, 
		j_def_fun_t                 *jdf);

j_def_fun_t *j_new_def_fun(
		j_type_t                     jt, 
		j_id_t                      *ji, 
		j_fun_args_t                *jfa, 
		j_statement_block_t         *jsb);

j_fun_args_t *j_new_fun_args(
		j_fun_args_t                *jfg,
		j_type_t                     jt,
		j_id_t                      *ji);

j_statement_block_t *j_new_statement_block(
		j_statement_list_t          *jsl);

j_statement_list_t *j_new_statement_list(
		j_statement_list_t          *jsl,
		j_statement_t               *js);

j_statement_t *j_new_statement(
		j_statement_type_t           jt,
		void                        *ptr);

j_id_t *j_new_id(
		size_t                       number);

j_statement_decl_t *j_new_statement_decl(
		j_type_t                     jt,
		j_decl_list_t               *jdl);

j_decl_list_t *j_new_decl_list(
		j_decl_t                    *jd,
		j_decl_list_t               *jdl);

j_decl_t *j_new_decl(
		j_id_t                      *ji,
		j_expr_t                    *je);

j_function_t *j_new_function(
		j_id_t                      *ji,
		j_function_args_t           *jfa);

j_function_args_t *j_new_function_args(
		j_expr_t                    *je,
		j_function_args_t           *jfa);

j_statement_return_t *j_new_statement_return(
		j_expr_t                    *je);

j_statement_cond_t *j_new_statement_cond(
		j_expr_t                    *je,
		j_statement_t               *iftrue,
		j_statement_t               *iffalse);

j_statement_expr_t *j_new_statement_expr(
		j_expr_t                    *je);

j_idexpr_t *j_new_idexpr(
		j_idexpr_type_t              type,
		j_id_t                      *ji,
		j_expr_t                    *je);

j_statement_iter_t *j_new_statement_iter(
		j_expr_t                    *je,
		j_statement_t               *js,
		j_expr_t                    *begin,
		j_expr_t                    *step);

j_expr_t *j_new_binary_expr(
		j_expr_type_t                type,
		j_expr_t                    *left,
		j_expr_t                    *right);

j_expr_t *j_new_unary_expr(
		j_expr_type_t                type,
		j_expr_t                    *je);

j_expr_t *j_new_value_expr(
		j_value_t                   *jv);

j_expr_t *j_new_function_expr(
		j_function_t                *jf);

j_expr_t *j_new_id_expr(
		j_idexpr_t                   *ji);

j_expr_t *j_new_casted_expr(
		j_expr_t                    *je,
		j_type_t                     jt);

j_value_t *j_new_value(
		j_type_t                     jt,
		char                        *value);

#endif

