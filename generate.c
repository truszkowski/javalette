/**
 * @brief Generowanie kodu asemblera.
 * @author Piotr Truszkowski
 */

#include "generate.h"
#include "types.h"
#include "new.h"
#include "delete.h"
#include "print.h"
#include "names.h"
#include "context.h"
#include "config.h"
#include "utils.h"
#include "asm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void j_program_generate_asm_code(
		j_program_t                 *program,
		FILE                        *output)
{
	//- Sekcja 'rodata'
	
	fprintf(output, J_ASM_SECTION_RODATA);

	//- Na 'program->vallist' jest lista wszystkich uzytych w programie wartosci. 
	//- Wystarczy zatem, kazdy jej element dopisac do sekcji 'rodata'.

	j_value_t *vl = program->vallist;

	while (NULL != vl) {
		fprintf(output, "\t%s%zd\t%s %s\n",
				J_VAL_LABEL, vl->idnumber, 
				J_BOOLEAN == vl->type ? "equ" : 
				J_INTEGER == vl->type ? "equ" : 
				J_DOUBLE  == vl->type ? "dd"  : "db",
				J_STRING  == vl->type ? j_string_to_asm_string(vl->value):vl->value);
	
		vl = vl->list;
	}

	fprintf(output, "\n");
	
	//- Sekcja 'text'
	
	fprintf(output, J_ASM_SECTION_TEXT);
	
	//- Makra

	fprintf(output, J_ASM_MACRO);

	//- Main 

	size_t main_number = j_names_get(program->names, "main");
	j_funtbl_inf_t *main_inf = j_context_find_fun_inf(program->ctx, main_number);

	assert(NULL != main_inf);

	fprintf(output, 
			"main:\n"
			"\tf_enter\n"
			"\tfinit\n"
			"\tcall near %s%d\n"
			"\tf_leave\n"
			"\tret\n"
			"\n",
			J_FUN_LABEL, main_inf->fun->idnumber);

	//- Na 'program->funlist' jest lista wysztkich zdefiniowanych funkcji. 
	//- Wystarczy zatem, kazdy jej element dopisac do pliku.

	j_def_fun_t *fl = program->funlist;

	while (NULL != fl) {
		//- Nie jest istotne w jakiej kolejnosci bedziemy generowac
		//- kod dla funkcji. Offsety, gdzie sie znajduja zmienne na
		//- stosie jest juz policzone - w trakcie analizy kontekstowej.

		j_program_generate_asm_code_fun(program, output, fl);
		fl = fl->list;
	}
}

void j_program_generate_asm_code_fun(
		j_program_t                 *program,
		FILE                        *output,
		j_def_fun_t                 *fun)
{
	fprintf(output, "%s%zd:\n", J_FUN_LABEL, fun->idnumber);
	fprintf(output, ";; Function: '%s' ;;\n", j_names_get_name(program->names, fun->id->number));

	if (fun->builtin)
		fprintf(output, "%s\n", fun->builtin);
	else {
		fprintf(output, "\tf_enter\n");
	
		j_program_generate_asm_code_block(program, output, fun->block);

		assert(fun->block->ctx->returnable);
	}
}

void j_program_generate_asm_code_block(
		j_program_t                 *program,
		FILE                        *output,
		j_statement_block_t         *block)
{
	j_statement_list_t *list = block->list;

	while (list) {
		j_program_generate_asm_code_statement(program, output, block->ctx, list->statement);
		list = list->list;
	}
	
	if (block->ctx->stack > 0 && 
			NULL != block->ctx->prev && 
			block->ctx->stack > block->ctx->prev->stack)
		fprintf(output, "\tadd esp , %d ; -B-L-O-C-K- -!-\n", 
				(block->ctx->stack - block->ctx->prev->stack));
}

void j_program_generate_asm_code_statement(
		j_program_t                 *program,
		FILE                        *output,
		j_context_t                 *curr,
		j_statement_t               *st)
{
	switch (st->type) {
		case J_ST_BLOCK:
			j_program_generate_asm_code_block(program, output, st->statement.block);
			break;
		case J_ST_DECL:
			j_program_generate_asm_code_decl(program, output, curr, st->statement.decl);
			break;
		case J_ST_COND:
			j_program_generate_asm_code_cond(program, output, curr, st->statement.cond);
			break;
		case J_ST_ITER:
			j_program_generate_asm_code_iter(program, output, curr, st->statement.iter);
			break;
		case J_ST_RETURN:
			j_program_generate_asm_code_return(program, output, curr, st->statement.ret);
			break;
		case J_ST_EXPR:
			j_program_generate_asm_code_expr(program, output, curr, st->statement.expr->expr);
			break;
		case J_ST_FUNC:
			//- Kod dla kazdej funkcji piszemy w osobnych miejscach.
			break;
		default:
			fprintf(stderr, "fatal: unknown statement type : %d\n", st->type);
			abort();
	}
}

void j_program_generate_asm_code_decl(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_decl_t          *decl)
{
	j_decl_list_t *list = decl->list;
	int off = 0;

	// Rezerwujemy miejsce na zmienne
	while (NULL != list)
		off += 4, list = list->list;
	
	fprintf(output, "\tsub esp , %d ; reserve\n", off);
	list = decl->list;

	while (NULL != list) {
		size_t depth;
		//- Nie ma znaczenia, ktora funkcje wywolamy w przypadku deklaracji.
		j_vartbl_inf_t *inf = j_context_find_var_inf(curr, list->decl->id->number, &depth);
		//j_vartbl_inf_t *inf = j_context_find_var_inf_directly(curr, list->decl->id->ctx, list->decl->id->number, &depth);
		assert(NULL != inf);
		assert(0 == depth); //- zmienna musi byc zdeklarowana w TYM kontekscie !!!

		if (NULL != list->decl->expr) {
			// Tutaj expr umiesci na eax wartosc wyrazenia
			j_program_generate_asm_code_expr(program, output, curr, list->decl->expr);
		} else {
			fprintf(output, "\txor eax , eax\n");
		}

		assert(inf->offset < 0);
		fprintf(output, "\tmov [ebp-%d] , eax ; variable '%s' \n", 
				-inf->offset, j_names_get_name(program->names, list->decl->id->number));
		list = list->list;
	}
}

void j_program_generate_asm_code_cond(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_cond_t          *cond)
{
	//- Na eax zostanie umieszczony warunek
	j_program_generate_asm_code_expr(program, output, curr, cond->expr);

	fprintf(output, 
			"\ttest eax , eax\n"
			"\tjz near %s%zdelse\n",
			J_COND_LABEL, cond->idnumber);

	//- Jak warunek jest prawdziwy
	j_program_generate_asm_code_statement(program, output, curr, cond->iftrue);

	fprintf(output, 
			"\tjmp near %s%zdendif\n"
			"%s%zdelse:\n",
			J_COND_LABEL, cond->idnumber,
			J_COND_LABEL, cond->idnumber);

	//- Jak warunek jest falszywy 
	if (NULL != cond->iffalse)
		j_program_generate_asm_code_statement(program, output, curr, cond->iffalse);

	fprintf(output, 
			"%s%zdendif:\n",
			J_COND_LABEL, cond->idnumber);
}

void j_program_generate_asm_code_iter(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_statement_iter_t          *iter)
{
	//- Przypisanie wstepne
	if (NULL != iter->begin)
		j_program_generate_asm_code_expr(program, output, curr, iter->begin);

	fprintf(output, 
			"%s%zdbegin:\n",
			J_ITER_LABEL, iter->idnumber);

	//- Warunek petli
	j_program_generate_asm_code_expr(program, output, curr, iter->expr);

	fprintf(output, 
			"\ttest eax , eax\n"
			"\tjz near %s%zdend\n",
			J_ITER_LABEL, iter->idnumber);

	//- Do wykonania w petli
	j_program_generate_asm_code_statement(program, output, curr, iter->statement);

	//- Przypisanie po kazdym obrocie petli
	if (NULL != iter->step)
		j_program_generate_asm_code_expr(program, output, curr, iter->step);

	fprintf(output, 
			"\tjmp near %s%zdbegin\n"
			"%s%zdend:\n",
			J_ITER_LABEL, iter->idnumber,
			J_ITER_LABEL, iter->idnumber);
}

#include "expr_resolv.h"

void j_program_generate_asm_code_return(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr,
		j_statement_return_t        *ret)
{
	fprintf(output, ";; Return ;;\n");

	if (NULL != ret->expr) {
		//- Wyliczamy zwracane wyrazenie (wartosc bedzie odlozona na eax)
		j_program_generate_asm_code_expr(program, output, curr, ret->expr);
	} 

	//- Zwijamy stos
	if (ret->stack)
		fprintf(output, "\tadd esp , %d\n", ret->stack);

	//- Protokol wyjscia z funkcji
	fprintf(output, "\tf_leave\n");
	fprintf(output, "\tret\n\n");
}

void j_program_generate_asm_code_expr(
		j_program_t                 *program, 
		FILE                        *output, 
		j_context_t                 *curr, 
		j_expr_t                    *expr)
{
	//- Tworzymy drzewko zaleznosci...
	j_expr_tree_t *etree = j_new_expr_tree(program, output, curr, expr);
	assert(NULL != etree);

	//- Generujemy kod, na eax bedzie wartosc wyrazenia...
	j_expr_tree_generate_asm_code(etree);

	j_delete_expr_tree(etree);	
}

