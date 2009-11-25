/**
 * @brief Analiza kontekstowa(semantyczna) programu.
 * @author Piotr Truszkowski
 */

#include "analyze.h"
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

//- Podlaczenie kolejnej wartosci do listy wszystkich
static void attach_val(
		j_program_t                 *p,
		j_value_t                   *v)
{
	if (NULL == p->vallist) 
		p->vallist = v;
	else {
		v->list = p->vallist;
		p->vallist = v;
	}
}

//- Podlaczenie kolejnej funkcji do listy wszystkich
static void attach_fun(
		j_program_t                 *p,
		j_def_fun_t                 *f)
{
	if (NULL == p->funlist) 
		p->funlist = f;
	else {
		f->list = p->funlist;
		p->funlist = f;
	}
}

void j_program_attach_std(
		j_program_t                 *program)
{
	/*- Dodajemy funkcje wbudowane -*/

	j_def_fun_t *df1 = j_new_def_fun(
			J_VOID, 
			j_new_id(j_names_add(program->names, "printString")),
			j_new_fun_args(NULL, J_STRING, j_new_id(j_names_add(program->names, "s"))), 
			NULL);

	j_def_fun_t *df2 = j_new_def_fun(
			J_VOID, 
			j_new_id(j_names_add(program->names, "printInt")),
			j_new_fun_args(NULL, J_INTEGER, j_new_id(j_names_add(program->names, "i"))), 
			NULL);

	j_def_fun_t *df3 = j_new_def_fun(
			J_VOID, 
			j_new_id(j_names_add(program->names, "printDouble")),
			j_new_fun_args(NULL, J_DOUBLE, j_new_id(j_names_add(program->names, "d"))),
			NULL);

	j_def_fun_t *df4 = j_new_def_fun(
			J_VOID,
			j_new_id(j_names_add(program->names, "error")),
			NULL,
			NULL);

	j_def_fun_t *df5 = j_new_def_fun(
			J_INTEGER,
			j_new_id(j_names_add(program->names, "readInt")),
			NULL,
			NULL);

	j_def_fun_t *df6 = j_new_def_fun(
			J_DOUBLE,
			j_new_id(j_names_add(program->names, "readDouble")),
			NULL,
			NULL);

	/*- Odhaczamy, ze sa to funkcje wbudowane -*/

	df1->builtin = J_ASM_PRINT_STRING_BUILTIN;
	df2->builtin = J_ASM_PRINT_INT_BUILTIN;
	df3->builtin = J_ASM_PRINT_DOUBLE_BUILTIN;
	df4->builtin = J_ASM_ERROR_BUILTIN;
	df5->builtin = J_ASM_READ_INT_BUILTIN;
	df6->builtin = J_ASM_READ_DOUBLE_BUILTIN;;

	/*- Dodajemy do struktury -*/

	program->root = j_new_def_fun_list(
			j_new_def_fun_list(
				j_new_def_fun_list(
					j_new_def_fun_list(
						j_new_def_fun_list(
							j_new_def_fun_list(
								program->root, df6), df5), df4), df3), df2), df1);

	/*- Ok -*/
}

int j_analyze_program(
		j_program_t                 *program)
{
	/*- W programie musi wystapic bezargumentowa funkcja main zwracajaca int -*/
	size_t main_number = j_names_get(program->names, "main");
	int found_main = 0;

	/*- Najpierw wrzucamy definicje funkcji -*/

	j_def_fun_list_t *dfl = program->root;
	
	while (NULL != dfl) {
		if (!j_context_add_fun(program->ctx, dfl->def_fun)) {
			j_def_fun_t *first = j_context_find_fun(program->ctx, dfl->def_fun->id->number);
			assert(NULL != first); //- Jesli nie moglismy dodac to musi juz byc taka funkcja

			if (first->builtin) {
				fprintf(stderr, "%s:%d,%d: error: cannot declare this function '%s' here, it's builtin\n", program->filename,
						dfl->def_fun->id->row, dfl->def_fun->id->col, j_names_get_name(program->names, dfl->def_fun->id->number));
				return 0;
			} else {
				fprintf(stderr, "%s:%d,%d: error: function '%s' is already declared in : %d,%d\n", program->filename, 
						dfl->def_fun->id->row, dfl->def_fun->id->col, j_names_get_name(program->names, dfl->def_fun->id->number), first->id->row, first->id->col);
				return 0;
			}
		}

		//- Nadajemy funkcji numerek
		dfl->def_fun->idnumber = (++program->fcount);
		attach_fun(program, dfl->def_fun);

		if (dfl->def_fun->id->number == main_number) {
			if (J_INTEGER != dfl->def_fun->type) {
				fprintf(stderr, "%s:%d,%d: error: 'main' must return 'int' type\n", program->filename,
						dfl->def_fun->id->row, dfl->def_fun->id->col);
				return 0;
			}
			if (NULL != dfl->def_fun->fun_args) {
				fprintf(stderr, "%s:%d,%d: error: 'main' must be declared without arguments\n", program->filename,
						dfl->def_fun->id->row, dfl->def_fun->id->col);
				return 0;
			}
			found_main = 1;
		}

		dfl = dfl->def_fun_list;
	}

	if (!found_main) {
		fprintf(stderr, "%s:-,-: error: missing main function\n", program->filename);
		return 0;
	}

	/*- Teraz analizujemy pokolei ka¿d± funkcjê -*/

	dfl = program->root;

	while (NULL != dfl) {
		if (!dfl->def_fun->builtin) {
			if (!j_program_analyze_def_fun(program, program->ctx, dfl->def_fun)) { 
				//printf("Cannot parse: '%s'\n", j_names_get_name(program->names, dfl->def_fun->id->number));
				return 0;
			}
			//- Funkcja przeanalizowana
		}
		
		dfl = dfl->def_fun_list;
	}
	
	return 1; //- Ok !!!
}

int j_program_analyze_def_fun(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_def_fun_t                 *def_fun)
{
	if (NULL != curr->prev) {
		/*- Mamy do czynienia z zagniezdzona funkcja,
		 *- trzeba wykonac to i owo, co juz jest 
		 *- zrobione dla funkcji globalnych.
		 */

		if (!j_context_add_fun(curr, def_fun)) {
			j_def_fun_t *first = j_context_find_fun(curr, def_fun->id->number);
			assert(NULL != first);

			fprintf(stderr, "%s:%d,%d: error: function '%s' is already declared in current context : %d,%d\n", program->filename,
					def_fun->id->row, def_fun->id->col, 
					j_names_get_name(program->names, def_fun->id->number),
					first->id->row, first->id->col);
			return 0;
		}

		
		//- Nadajemy funkcji numerek
		def_fun->idnumber = (++program->fcount);
		attach_fun(program, def_fun);
	}
	
	//- Odhaczamy w jakim kontekscie funkcja zostala zadeklarowana.
	// def_fun->id->ctx = curr; Nie jest to konieczne dla deklaracji
	
	//- Tworzymy kontekst dla funkcji i bloku jej instrukcji. 

	curr = j_new_context(curr, def_fun, J_SYM_HASH_SIZE);
	def_fun->ctx = curr;
	
	/*- Sprawdzamy argumenty -*/

	j_fun_args_t *args = def_fun->fun_args;
	
	while (args) {
		//- Jedynie funkcje wbudowane moga korzystac ze stringow !!!
		if (!j_type_is_numeric(args->type)) {
			fprintf(stderr, "%s:%d,%d: error: argument of '%s' must be numeric\n", program->filename,
					args->id->row, args->id->col, j_names_get_name(program->names, args->id->number));
			return 0;
		}
		args = args->fun_args;
	}

	/*- Dodajemy argumenty funkcji -*/

	//- Na strosie parametrow sa odkladane nastepujaco:
	//- 1:[ebp+8], 2:[ebp+12], 3:[ebp+16], ..., N:[ebp+4*(N+1)]
	int off = 8;
	args = def_fun->fun_args;

	while (args) {
		if (!j_context_add_var(def_fun->ctx, args->type, args->id, NULL, off)) {
			fprintf(stderr, "%s:%d,%d: error: variable '%s' is already declared in current context\n", program->filename,
					args->id->row, args->id->col, j_names_get_name(program->names, args->id->number));
			return 0;
		}

		off += 4;
		args = args->fun_args;
	}

	//- Dla instrukcji blokowej podczepiamy bierzacy kontekst wraz ze argumentami
	//- funkcji. Kontekst tworzymy zawsze przed wejsciem do instrunkcji blokowej.

	if (!j_program_analyze_block(program, def_fun->ctx, def_fun->block)) return 0;

	if (!curr->returnable) {
		fprintf(stderr, "%s:%d,%d: error: missing return statement in function '%s'\n", program->filename,
				def_fun->id->row, def_fun->id->col, j_names_get_name(program->names, def_fun->id->number));
		return 0;
	}

	return 1;
}

int j_program_analyze_statement(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_t               *st)
{
	switch (st->type) {
		case J_ST_BLOCK:
			{
				//- Wchodzac do instrukcji blokowej, potrzeba zdefiniowac nowy kontekst.
				j_context_t *next = j_new_context(curr, curr->fun, J_SYM_HASH_SIZE);
				return j_program_analyze_block(program, next, st->statement.block);
			}
		case J_ST_DECL:
			return j_program_analyze_decl(program, curr, st->statement.decl);
		case J_ST_COND:
			return j_program_analyze_cond(program, curr, st->statement.cond);
		case J_ST_ITER:
			return j_program_analyze_iter(program, curr, st->statement.iter);
		case J_ST_RETURN:
			return j_program_analyze_return(program, curr, st->statement.ret);
		case J_ST_EXPR:
			return (J_BAD_TYPE == j_program_analyze_expr_type(program, curr, st->statement.expr->expr)) ? 0:1;
		case J_ST_FUNC:
			return j_program_analyze_def_fun(program, curr, st->statement.func);
		default:
			fprintf(stderr, "fatal: unknown statement type : %d\n", st->type);
			abort();
	}
}

int j_program_analyze_block(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_block_t         *block)
{
	block->ctx = curr;

	j_statement_list_t *list = block->list;

	while (list) {
		if (!j_program_analyze_statement(program, block->ctx, list->statement)) return 0;
		list = list->list;
	}

	//- Jesli w bloku byla instrukcja powrotu to przekazujemy informacje wyzej...
	if (curr->returnable) {
		curr->prev->returnable = 1;
	}

	return 1;
}


int j_program_analyze_iter(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_iter_t          *iter)
{
	if (iter->begin &&
			J_BAD_TYPE == j_program_analyze_expr_type(program, curr, iter->begin)) return 0;
	if (iter->step &&
			J_BAD_TYPE == j_program_analyze_expr_type(program, curr, iter->step)) return 0;
	
	iter->idnumber = (++program->icount);

	j_type_t t = j_program_analyze_expr_type(program, curr, iter->expr);
	
	if (J_BAD_TYPE == t) return 0;

	if (!j_type_is_numeric(t)) {
		fprintf(stderr, "%s:%d,%d: error: expresion must be numeric\n", program->filename,
				iter->expr->row, iter->expr->col);
		return 0;
	}

	//- Oczekujemy, ze warunek petli bedzie boolean-em
	if (J_BOOLEAN != t)
		iter->expr = j_new_casted_expr(iter->expr, J_BOOLEAN);

	int r = curr->returnable;
	int st = j_program_analyze_statement(program, curr, iter->statement);

	curr->returnable = r;
	return st;
}

int j_program_analyze_decl(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_decl_t          *decl)
{
	j_type_t t = decl->type;
	j_decl_list_t *list = decl->list;

	//- Robimy trik, najpierw przepatrzymy wyrazenie jakie przypisujemy na zmienna
	//- a potem sama zmienna. Nazwa deklarowanej zmiennej moze wystapic w wyrazeniu,
	//- zatem niech jej wartoscia bedzie wartosc z ktoregos nizej kontekstu.
	//- Gdy w kontekstach nizej nie ma takiej zmiennej uznajemy to za blad.

	if (!j_type_is_numeric(t)) {
		fprintf(stderr, "%s:%d,%d: error: variable '%s' must be numeric\n", program->filename,
				decl->list->decl->id->row, decl->list->decl->id->col, 
				j_names_get_name(program->names, decl->list->decl->id->number));
		return 0;
	}

	j_funtbl_inf_t *finf = j_context_find_fun_inf(curr, curr->fun->id->number);
	assert(NULL != finf);

	while (list) {
		j_decl_t *d = list->decl;

		if (d->expr) { // Jesli jest ustawione wyrazenie
			j_type_t te = j_program_analyze_expr_type(program, curr, d->expr);
			if (J_BAD_TYPE == te) return 0;

			if (!j_type_is_numeric(te)) {
				fprintf(stderr, "%s:%d,%d: error: variable '%s' must be numeric\n", program->filename,
						d->id->row, d->id->col, j_names_get_name(program->names, d->id->number));
				return 0;
			}
			
			if (t != te) 
				d->expr = j_new_casted_expr(d->expr, t);
		}

		//- Na curr->stack mamy powiedziane ile juz miejsca na zmienne przeznano 
		//- na stosie, wliczajac zagniezdzone wywolania. Trzeba wyliczyc roznice
		//- wiedzy poziomem stosu bierzacej funkcji a poprzedniej.

		//- UWAGA, jest tez globalny kontekst !!!
		int stack = j_context_stack_size(curr);

		//- Zmienne w funkcji odkladamy nastepujaco:
		//- [ebp-4], [ebp-8], ...
		if (!j_context_add_var(curr, t, d->id, d->expr, -4 - stack)) {
			fprintf(stderr, "%s:%d,%d: error: variable '%s' is already declared in current context\n", program->filename,
					d->id->row, d->id->col, j_names_get_name(program->names, d->id->number));
			return 0;
		}

		d->id->ctx = curr;

		curr->stack += 4;

		list = list->list;
	}
	
	return 1;
}

int j_program_analyze_idexpr(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_idexpr_t                  *idexpr)
{
	j_type_t t = j_context_find_var_type(curr, idexpr->id->number);

	if (J_BAD_TYPE == t) {
		fprintf(stderr, "%s:%d,%d: error: undeclared variable '%s', not found in current context\n", program->filename,
				idexpr->id->row, idexpr->id->col, 
				j_names_get_name(program->names, idexpr->id->number));
		return 0;
	}
	
	idexpr->id->ctx = j_context_find_var_home(curr, idexpr->id->number);

	switch (idexpr->type) {
		case J_ID_ONLY:
			break;
		case J_ASSIGN:
			{
				j_type_t te = j_program_analyze_expr_type(program, curr, idexpr->expr);
				if (J_BAD_TYPE == te) return 0;
				if (!j_type_is_numeric(te)) { 
					fprintf(stderr, "%s:%d,%d: error: expresion must be numeric\n", program->filename,
							idexpr->id->row, idexpr->id->col);
					return 0;
				}

				if (te != t) 
					idexpr->expr = j_new_casted_expr(idexpr->expr, t);
			}
			break;
		case J_ASSIGN_DEC:
		case J_ASSIGN_INC:
			if (!j_type_is_numeric(t)) {
				fprintf(stderr, "%s:%d,%d: error: variable '%s' must be numeric\n", program->filename,
						idexpr->id->row, idexpr->id->col, 
						j_names_get_name(program->names, idexpr->id->number));
				return 0;
			}
			if (J_BOOLEAN == t) {
				fprintf(stderr, "%s:%d,%d: error: variable '%s' is boolean, cannot use '--' or '++'\n", program->filename,
						idexpr->id->row, idexpr->id->col, j_names_get_name(program->names, idexpr->id->number));
				return 0;
			}
			break;
		default:
			fprintf(stderr, "fatal: unknown assignment type : %d\n", idexpr->type);
			abort();
	}
	
	return 1;
}

int j_program_analyze_cond(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_cond_t          *cond)
{
	cond->idnumber = (++program->ccount);

	j_type_t t = j_program_analyze_expr_type(program, curr, cond->expr);
	if (J_BAD_TYPE == t) return 0;

	if (!j_type_is_numeric(t)) {
		fprintf(stderr, "%s:%d,%d: error: condition must be numeric\n", program->filename,
				cond->expr->row, cond->expr->col);
		return 0;
	}

	if (J_BOOLEAN != t) 
		cond->expr = j_new_casted_expr(cond->expr, J_BOOLEAN);

	//- Trzeba sprawdzic czy bedzie instrukcja powrotu.
	//- Tylko w 'if-e' trzeba sprawdzac rozne rozgalezienia.

	int returnable = curr->returnable;
	int iftrue = 0, iffalse = 0;

	if (!j_program_analyze_statement(program, curr, cond->iftrue)) return 0;

	iftrue = curr->returnable;
	curr->returnable = 0;

	if (cond->iffalse && !j_program_analyze_statement(program, curr, cond->iffalse)) return 0;
		
	iffalse = curr->returnable;

	if (returnable || (iftrue && iffalse)) {
		//- Juz byla instrukcja powrotu lub wszystkie galezie if-a maja instrukcje powrotu.
		curr->returnable = 1;
	}

	return 1;
}

int j_program_analyze_return(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_statement_return_t        *ret)
{
	assert(NULL != curr->fun);

	j_type_t t = curr->fun->type;
	
	//- Odhaczamy w bierzacym kontekscie, ze jest return 
	//- oraz odhaczamy jaka jest wysokosc stosu do zdjecia.

	curr->returnable = 1;
	ret->stack = curr->stack;

	if (J_VOID == t) { //- Funkcja ma nic nie zwracac
		if (!ret->expr) return 1;
		t = j_program_analyze_expr_type(program, curr, ret->expr);
		if (J_BAD_TYPE == t) return 0;
		if (J_VOID == t) return 1;

		fprintf(stderr, "%s:%d,%d: error: cannot return any value in void-function : '%s'\n", program->filename,
				ret->row, ret->col, j_names_get_name(program->names, curr->fun->id->number));
		return 0;
	}

	if (j_type_is_numeric(t)) { //- Funkcja ma zwracac typ numeryczny
		j_type_t te = J_VOID;

		if (ret->expr) { 
			te = j_program_analyze_expr_type(program, curr, ret->expr);
			if (J_BAD_TYPE == te) return 0;
			if (j_type_is_numeric(te)) {
				if (te != t) 
					ret->expr = j_new_casted_expr(ret->expr, t);
				return 1;
			}
		} 
		
		fprintf(stderr, "%s:%d,%d: error: incompatible types, function '%s' must return type '%s', not '%s'\n", program->filename,
				ret->row, ret->col, j_names_get_name(program->names, curr->fun->id->number), j_type_to_string(t), j_type_to_string(te));
		return 0;
	}

	fprintf(stderr, "fatal: unknown type : %d\n", t);
	abort();
}

j_type_t j_program_analyze_expr_type(
		j_program_t                 *program,
		j_context_t                 *curr,
		j_expr_t                    *expr)
{
	expr->idnumber = (++program->ecount);

	if (J_EXPR_VALUE == expr->etype) {
		expr->type = expr->value->type;
		expr->value->idnumber = (++program->vcount);
		attach_val(program, expr->value);

		return expr->value->type;
	}

	if (J_EXPR_FUNCTION == expr->etype) {
		j_def_fun_t *d = j_context_find_fun(curr, expr->function->id->number);

		//- Funkcja musi pamietac w jakim kontekscie funkcja jest zadeklarowana - 
		//- - zeby potem uniknac wywolywania funkcji bedacej w aktualnym kontekscie
		//- ale jeszcze nie zdefiniowanej.
		expr->function->id->ctx = j_context_find_fun_home(curr, expr->function->id->number);

		if (NULL == d) {
			fprintf(stderr, "%s:%d,%d: error: undeclared function '%s', not found in current context\n", program->filename,
					expr->function->row, expr->function->col, 
					j_names_get_name(program->names, expr->function->id->number));
			return J_BAD_TYPE;
		}

		//- Jesli ok, to wynik funkcji moze byc tylko VOID lub numeryczny
		assert(j_type_is_numeric(d->type) || J_VOID == d->type);

		//- Pozostaje sprawdzic liste argumentow
		
		j_fun_args_t *dargs = d->fun_args;
		j_function_args_t *fargs = expr->function->args;

		while (1) {
			if (NULL == dargs && NULL == fargs) break;
			if (NULL == dargs) {
				fprintf(stderr, "%s:%d,%d: error: too many arguments for '%s'\n", program->filename,
						expr->function->row, expr->function->row, 
						j_names_get_name(program->names, expr->function->id->number));
				return J_BAD_TYPE;
			}

			if (NULL == fargs) {
				fprintf(stderr, "%s:%d,%d: error: too few arguments for '%s'\n", program->filename,
						expr->function->row, expr->function->row,
						j_names_get_name(program->names, expr->function->id->number));
				return J_BAD_TYPE;
			}

			j_type_t ta = j_program_analyze_expr_type(program, curr, fargs->expr);
			if (J_BAD_TYPE == ta) return J_BAD_TYPE;

			if (ta != dargs->type && !j_type_is_numeric(ta)) {
				fprintf(stderr, "%s:%d,%d: error: wrong expresion type : '%s', expected : '%s'\n", program->filename,
						fargs->expr->row, fargs->expr->col, j_type_to_string(ta), j_type_to_string(dargs->type));
				return J_BAD_TYPE;
			}

			if (fargs->expr->type != dargs->type) { //- Rzutujemy jak inne typy
				fargs->expr = j_new_casted_expr(fargs->expr, dargs->type);
				fargs->expr->idnumber = (++program->ecount);
			}

			dargs = dargs->fun_args;
			fargs = fargs->args;
		}

		expr->type = d->type;
		return d->type;
	}

	if (J_EXPR_ID == expr->etype) {
		if (!j_program_analyze_idexpr(program, curr, expr->id)) return J_BAD_TYPE;
		expr->type = j_context_find_var_type(curr, expr->id->id->number);
		return expr->type;
	}

	if (J_EXPR_CAST == expr->etype) {
		j_type_t t = j_program_analyze_expr_type(program, curr, expr->left);
		if (J_BAD_TYPE == t) return J_BAD_TYPE;
		if (!j_type_is_numeric(t)) {
			fprintf(stderr, "%s:%d,%d: error: expresion must be numeric\n", program->filename,
					expr->left->row, expr->left->col);
			return J_BAD_TYPE;
		}
		if (!j_type_is_numeric(expr->type) && J_VOID != expr->type) {
			fprintf(stderr, "%s:%d,%d: error: cannot cast to '%s'\n", program->filename,
					expr->row, expr->col, j_type_to_string(expr->type));
			return J_BAD_TYPE;
		}

		return expr->type;
	}
	
	if (J_EXPR_NOT == expr->etype) {
		j_type_t t = j_program_analyze_expr_type(program, curr, expr->left);
		if (J_BAD_TYPE == t) return J_BAD_TYPE;
		if (!j_type_is_numeric(t)) {
			fprintf(stderr, "%s:%d,%d: error: cannot use boolean negation to non-numeric value\n", program->filename,
					expr->row, expr->col);
			return J_BAD_TYPE;
		}

		if (J_BOOLEAN != t) { //- Zrzucamy na boolean
			expr->left = j_new_casted_expr(expr->left, J_BOOLEAN);
			expr->left->idnumber = (++program->ecount);
		}

		expr->type = J_BOOLEAN;
		return J_BOOLEAN;
	}

	if (J_EXPR_NEG == expr->etype) {
		j_type_t t = j_program_analyze_expr_type(program, curr, expr->left);
		if (J_BAD_TYPE == t) return J_BAD_TYPE;
		if (!j_type_is_numeric(t)) {
			fprintf(stderr, "%s:%d,%d: error: cannot use numeric negation to non-numeric value\n", program->filename,
					expr->row, expr->col);
			return J_BAD_TYPE;
		}

		if (J_BOOLEAN == t) { // -true, -false -- zrzutujmy na int-a...
			expr->left = j_new_casted_expr(expr->left, J_INTEGER);
			expr->left->idnumber = (++program->ecount);
			t = J_INTEGER;
		}

		expr->type = t;
		return t;
	}

	/*- Pozostaly do sprawdzenia tylko dwu-argumentowe operacje -*/

	j_type_t t1 = j_program_analyze_expr_type(program, curr, expr->left),
					 t2 = j_program_analyze_expr_type(program, curr, expr->right);

	if (J_BAD_TYPE == t1 || J_BAD_TYPE == t2) return J_BAD_TYPE;

	/*- lewa i prawa wartosc musza byc numeryczne -*/

	if (!j_type_is_numeric(t1)) {
		fprintf(stderr, "%s:%d,%d: error: first argument must be numeric\n", program->filename,
				expr->left->row, expr->left->col);
		return J_BAD_TYPE;
	}

	if (!j_type_is_numeric(t2)) {
		fprintf(stderr, "%s:%d,%d: error: second argument must be numeric\n", program->filename,
				expr->right->row, expr->right->col);
		return J_BAD_TYPE;
	}

	if (J_EXPR_AND == expr->etype ||
			J_EXPR_OR == expr->etype) { 
		if (J_BOOLEAN != expr->left->type) {
			expr->left = j_new_casted_expr(expr->left, J_BOOLEAN);
			expr->left->idnumber = (++program->ecount);
		}
		if (J_BOOLEAN != expr->right->type) {
			expr->right = j_new_casted_expr(expr->right, J_BOOLEAN);
			expr->right->idnumber = (++program->ecount);
		}

		expr->type = J_BOOLEAN;
		return J_BOOLEAN;
	}

	if (J_EXPR_EQ == expr->etype || J_EXPR_NE == expr->etype ||
			J_EXPR_LT == expr->etype || J_EXPR_GT == expr->etype ||
			J_EXPR_LE == expr->etype || J_EXPR_GE == expr->etype) {
		int cmp = j_type_compare(expr->left->type, expr->right->type);
		if (cmp < 0) { //- left ma szerszy typ
			expr->right = j_new_casted_expr(expr->right, expr->left->type);
			expr->right->idnumber = (++program->ecount);
		} else if (cmp > 0) { //- right ma szerszy typ
			expr->left = j_new_casted_expr(expr->left, expr->right->type);
			expr->left->idnumber = (++program->ecount);
		}

		expr->type = J_BOOLEAN;
		return J_BOOLEAN;
	}

	if (J_BOOLEAN == t1) t1 = J_INTEGER;
	if (J_BOOLEAN == t2) t2 = J_INTEGER;
	
	assert(J_INTEGER == t1 || J_DOUBLE == t1);
	assert(J_INTEGER == t2 || J_DOUBLE == t2);

	if (J_EXPR_ADD == expr->etype || 
			J_EXPR_SUB == expr->etype || 
			J_EXPR_MUL == expr->etype || 
			J_EXPR_DIV == expr->etype) {
		int cmp = j_type_compare(expr->left->type, expr->right->type);
		if (cmp < 0) { //- left ma szerszy typ
			expr->right = j_new_casted_expr(expr->right, expr->left->type);
			expr->right->idnumber = (++program->ecount);
		} else if (cmp > 0) {
			expr->left = j_new_casted_expr(expr->left, expr->right->type);
			expr->left->idnumber = (++program->ecount);
		} else {
			//- Yf... operacje na booleanach == na intach
			if (J_BOOLEAN == expr->left->type) {
				expr->left = j_new_casted_expr(expr->left, J_INTEGER);
				expr->left->idnumber = (++program->ecount);
			}
			if (J_BOOLEAN == expr->right->type) {
				expr->right = j_new_casted_expr(expr->right, J_INTEGER);
				expr->right->idnumber = (++program->ecount);
			}
		}

		assert(expr->left->type == expr->right->type);
		assert(J_INTEGER == expr->left->type || J_DOUBLE == expr->left->type);

		expr->type = expr->left->type;
		return expr->type;
	}

	if (J_EXPR_MOD == expr->etype) {
		if (J_INTEGER != expr->left->type) {
			expr->left = j_new_casted_expr(expr->left, J_INTEGER);
			expr->left->idnumber = (++program->ecount);
		}
		if (J_INTEGER != expr->right->type) {
			expr->right = j_new_casted_expr(expr->right, J_INTEGER);
			expr->right->idnumber = (++program->ecount);
		}

		expr->type = J_INTEGER;
		return J_INTEGER;
	}

	fprintf(stderr, "fatal: unknown expresion type : %d\n", expr->etype);
	abort();
}

