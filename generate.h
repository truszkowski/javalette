/**
 * @brief Generowanie kodu asemblera.
 * @author Piotr Truszkowski
 */

#ifndef _J_GENERATE_H_
#define _J_GENERATE_H_

#include "types.h"
#include "names.h"
#include "context.h"
#include "program.h"

#include <stdio.h>

/**
 * @brief Generowanie kodu asemblera dla programu
 */
void j_program_generate_asm_code(
		j_program_t                 *program,
		FILE                        *output);

/**
 * @brief Generowanie kodu asemblera dla funkcji
 */
void j_program_generate_asm_code_fun(
		j_program_t                 *program,
		FILE                        *output,
		j_def_fun_t                 *fun);

/**
 * @brief Generowanie kodu asemblera dla bloku
 */
void j_program_generate_asm_code_block(
		j_program_t                 *program,
		FILE                        *output,
		j_statement_block_t         *block);

/**
 * @brief Generowanie kodu asemblera dla instrukcji
 */
void j_program_generate_asm_code_statement(
		j_program_t                 *program,
		FILE                        *output,
		j_context_t                 *curr,
		j_statement_t               *s);

/**
 * @brief Generowanie kodu asemblera dla deklaracji
 */
void j_program_generate_asm_code_decl(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_decl_t          *decl);

/**
 * @brief Generowanie kodu asemblera dla instrukcji warunkowej
 */
void j_program_generate_asm_code_cond(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_cond_t          *cond);

/**
 * @brief Generowanie kodu asemblera dla instrukcji iteracji
 */
void j_program_generate_asm_code_iter(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_iter_t          *iter);

/**
 * @brief Generowanie kodu asemblera dla instrukcji powrotu
 */
void j_program_generate_asm_code_return(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr,
		j_statement_return_t        *ret);

/**
 * @brief Generowanie kodu asemblera dla wyrazenia
 */
void j_program_generate_asm_code_expr(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_expr_t                    *expr);
	
#endif

