/**
 * @brief Program.
 * @author Piotr Truszkowski
 */

#ifndef _J_PROGRAM_H_
#define _J_PROGRAM_H_

#include "types.h"
#include "names.h"
#include "context.h"

#include <stdio.h>

typedef struct j_program_t {
	//- Drzewo programu, po analizie leksykalnej i skladniowej.
	struct j_def_fun_list_t       *root;
	//- Tablica nazw identyfikatorow wystepujacych w programie.
	struct j_names_t              *names;
	//- Glowny kontekst, zawiera definicje globalnych funkcji.
	struct j_context_t            *ctx;
	//- Nazwa kompilowanego pliku
	const char                    *filename;
	//- Lista wszystkich funkcji
	struct j_def_fun_t            *funlist;
	size_t                         fcount;
	//- Lista wszystkich wartosci
	struct j_value_t              *vallist;
	size_t                         vcount;
	//- Licznik instrukcji warunkowych
	size_t                         ccount;
	//- Licznik instrukcji iteracji
	size_t                         icount;
	//- Licznik wyrazen
	size_t                         ecount;
} j_program_t;

/**
 * @brief Utworzenie programu.
 */
j_program_t *j_new_program(
		const char                  *filename);

/**
 * @brief Zwolnienie zasobow.
 */
void j_delete_program(
		j_program_t                 *program);

/**
 * @brief Wydruk programu.
 */
void j_print_program(
		j_program_t                 *program,
		FILE                        *stream);

#ifdef USE_OLD
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
int j_program_analyze_assign(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_assign_t                  *assign);

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

void j_program_generate_asm_code_statement(
		j_program_t                 *program,
		FILE                        *output,
		j_context_t                 *curr,
		j_statement_t               *s);

void j_program_generate_asm_code_decl(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_decl_t          *decl);

void j_program_generate_asm_code_assign(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_assign_t                  *assign);

void j_program_generate_asm_code_cond(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_cond_t          *cond);

void j_program_generate_asm_code_iter(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_iter_t          *iter);

void j_program_generate_asm_code_return(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr,
		j_statement_return_t        *ret);

void j_program_generate_asm_code_expr(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_expr_t                    *expr);
#endif

#endif

