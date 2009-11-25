/**
 * @brief Analiza kontekstowa(semantyczna) programu.
 * @author Piotr Truszkowski
 */

#ifndef _J_ANALYZE_H_
#define _J_ANALYZE_H_

#include "types.h"
#include "names.h"
#include "context.h"
#include "program.h"

#include <stdio.h>

/**
 * @brief Zalaczenie standardowo wbudowanych funkcji.
 */
void j_program_attach_std(
		j_program_t                 *program);

/**
 * @brief Analiza kontekstowa programu.
 */
int j_analyze_program(
		j_program_t                 *program);

/**
 * @brief Analiza kontekstowa funkcji wzgledem programu i kontekstu.
 */
int j_program_analyze_def_fun(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_def_fun_t                 *def_fun);

/**
 * @brief Analiza kontekstowa instrukcji wzgledem programu i kontekstu.
 */
int j_program_analyze_statement(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_t               *st);

/**
 * @brief Analiza kontekstowa bloku wzgledem programu i kontekstu.
 */
int j_program_analyze_block(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_block_t         *block);

/**
 * @brief Analiza kontekstowa iteracji wzgledem programu i kontekstu.
 */
int j_program_analyze_iter(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_iter_t          *iter);

/**
 * @brief Analiza kontekstowa deklaracji wzgledem programu i kontekstu.
 */
int j_program_analyze_decl(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_decl_t          *decl);

/**
 * @brief Analiza kontekstowa przypisania wzgledem programu i kontekstu.
 */
int j_program_analyze_idexpr(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_idexpr_t                  *idexpr);

/**
 * @brief Analiza kontekstowa warunku wzgledem programu i kontekstu.
 */
int j_program_analyze_cond(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_cond_t          *cond);

/**
 * @brief Analiza kontekstowa powrotu wzgledem programu i kontekstu.
 */
int j_program_analyze_return(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_return_t        *ret);

/**
 * @brief Analiza kontekstowa wyrazenia wzgledem programu i kontekstu.
 */
j_type_t j_program_analyze_expr_type(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_expr_t                    *expr);

#endif

