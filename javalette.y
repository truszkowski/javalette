/**
 * @brief Gramatyka jezyka javalette.
 * Wystepuja konflikty przesuniecie-redukcja, sa one jednak
 * przez bizona prawidlowo(zgodnie z intuicja) rozwiazywane. 
 * Dzieki temu nie trzeba zamazywac obrazu gramatyki.
 * @author Piotr Truszkowski
 */ 

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include "javalette.h"

extern int colnum;
extern int rownum;
extern const char *input_file;

#define YYERROR_VERBOSE

/*- Ladne wypisywanie gdzie jest blad -*/
void yyerror(const char *s)
{
	fprintf(stderr, "%s:%d,%d: %s\n", input_file, rownum, colnum, s);
	fflush(stdout);
	fflush(stderr);
	exit(-1);
}

/*- Samemu nie definiuje -*/
int yylex(void);

%}

%union {
	int /*j_type_t*/               type;
	struct j_id_t                 *id;
	struct j_value_t              *value;
	struct j_def_fun_list_t       *def_fun_list;
	struct j_def_fun_t            *def_fun;
	struct j_fun_args_t           *fun_args;
	struct j_statement_t          *statement;
	struct j_statement_block_t    *s_block;
	struct j_statement_list_t     *s_list;
	struct j_statement_decl_t     *s_decl;
	struct j_statement_return_t   *s_return;
	struct j_statement_cond_t     *s_cond;
	struct j_statement_expr_t     *s_expr;
	struct j_statement_iter_t     *s_iter;
	struct j_decl_list_t          *decl_list;
	struct j_decl_t               *decl;
	struct j_expr_t               *expr;
	struct j_idexpr_t             *idexpr;
	struct j_function_t           *function;
	struct j_function_args_t      *function_args;
	struct j_expr_t               *return_expr;

/*- tokeny od leksera -*/
	char                          *bvalue;
	char                          *ivalue;
	char                          *dvalue;
	char                          *svalue;
	char                          *text;
}

/*- Terminale -*/
%token <text>                     J_TEXT              "text"
%token <bvalue>                   J_V_BOOLEAN         "boolean value"
%token <ivalue>                   J_V_INTEGER         "integer value"
%token <dvalue>                   J_V_DOUBLE          "double value"
%token <svalue>                   J_V_STRING          "string value"
%token <btype>                    J_T_BOOLEAN         "boolean"
%token <itype>                    J_T_INTEGER         "integer"
%token <dtype>                    J_T_DOUBLE          "double"
%token <stype>                    J_T_STRING          "string"
%token <vtype>                    J_T_VOID            "void"

/*- Operatory -*/
%token <operator>                 J_OP_ADD           "+"
%token <operator>                 J_OP_SUB           "-"
%token <operator>                 J_OP_MUL           "*"
%token <operator>                 J_OP_DIV           "/"
%token <operator>                 J_OP_MOD           "%"
%token <operator>                 J_OP_POW           "^"
%token <operator>                 J_OP_NOT           "!"
%token <operator>                 J_OP_AND           "&&"
%token <operator>                 J_OP_OR            "||"
%token <operator>                 J_OP_EQ            "=="
%token <operator>                 J_OP_NE            "!="
%token <operator>                 J_OP_GT            ">"
%token <operator>                 J_OP_LT            "<"
%token <operator>                 J_OP_GE            ">="
%token <operator>                 J_OP_LE            "<="
%token <operator>                 J_OP_PLUSPLUS      "++"
%token <operator>                 J_OP_MINUSMINUS    "--"

/*- Slowa kluczowe -*/

%token <keyword>                  J_KW_IF            "if"
%token <keyword>                  J_KW_ELSE          "else"
%token <keyword>                  J_KW_WHILE         "while"
%token <keyword>                  J_KW_FOR           "for"
%token <keyword>                  J_KW_RETURN        "return"

/*- Typy bardziej zlozone -*/

%type <def_fun_list>              def_fun_list
%type <def_fun>                   def_fun
%type <fun_args>                  fun_args
%type <s_block>                   s_block
%type <s_list>                    s_list
%type <s_decl>                    s_decl
%type <s_return>                  s_return
%type <s_cond>                    s_cond
%type <s_expr>                    s_expr
%type <s_iter>                    s_iter
%type <statement>                 statement
%type <decl_list>                 decl_list
%type <decl>                      decl
%type <expr>                      expr
%type <idexpr>                    idexpr
%type <function>                  function
%type <function_args>             function_args
%type <return_expr>               return_expr
%type <value>                     value
%type <type>                      type
%type <id>                        id

/*- Ktoro-stronnie -*/

%left J_OP_AND 
%left J_OP_OR 
%left J_OP_EQ J_OP_NE J_OP_GT J_OP_GE J_OP_LT J_OP_LE
%right '='
%left J_OP_ADD J_OP_SUB 
%left J_OP_MUL J_OP_DIV J_OP_MOD
%right J_OP_POW
%left REVERSE

%expect 1

%%

input: 
	| def_fun_list { j_program->root = $1; }
	; 

def_fun_list: def_fun { 
	$$ = j_new_def_fun_list(NULL, $1);
}
	| def_fun def_fun_list { 
	$$ = j_new_def_fun_list($2, $1); 
}
	;

def_fun: type id '(' ')' s_block { 
	$$ = j_new_def_fun($1, $2, NULL, $5);
}
	| type id '(' fun_args ')' s_block { 
	$$ = j_new_def_fun($1, $2, $4, $6);
}
	;

fun_args: type id { 
	$$ = j_new_fun_args(NULL, $1, $2);
}
	| type id ',' fun_args { 
	$$ = j_new_fun_args($4, $1, $2);
}
	;

s_block: '{' s_list '}' { 
	$$ = j_new_statement_block($2);
}
	;

s_list: statement { 
	$$ = j_new_statement_list(NULL, $1);
}
	| statement s_list { 
	$$ = j_new_statement_list($2, $1);
}
	;

statement: s_block { 
	$$ = j_new_statement(J_ST_BLOCK, $1);
}
	| s_decl {
	$$ = j_new_statement(J_ST_DECL, $1);
} 
	| s_return {
	$$ = j_new_statement(J_ST_RETURN, $1);
}
	| s_cond {
	$$ = j_new_statement(J_ST_COND, $1);
}
	| s_expr {
	$$ = j_new_statement(J_ST_EXPR, $1);
}
	| s_iter {
	$$ = j_new_statement(J_ST_ITER, $1);
} 
	| def_fun {
	$$ = j_new_statement(J_ST_FUNC, $1);
}
	;

s_iter: J_KW_FOR '(' expr ';' expr ';' expr ')' statement {
	$$ = j_new_statement_iter($5, $9, $3, $7);
} 
	| J_KW_WHILE '(' expr ')' statement {
	$$ = j_new_statement_iter($3, $5, 0, 0);
}
	;

s_decl: type decl_list ';' {
	$$ = j_new_statement_decl($1, $2);
}
	;

decl_list: decl {
	$$ = j_new_decl_list($1, NULL);
}
	| decl ',' decl_list {
	$$ = j_new_decl_list($1, $3);
}
	;

decl: id {
	$$ = j_new_decl($1, NULL);
}
	| id '=' expr {
	$$ = j_new_decl($1, $3);
}
	;

s_return: J_KW_RETURN return_expr {
	$$ = j_new_statement_return($2);
} 
	;

return_expr: ';' {
	$$ = NULL;
} | expr ';' {
	$$ = $1;
}

/*
 *- ! Problem przesuniecia-redukcji w 'if'-ie. !
 *-
 *- Do zaakceptowania, gdyz bison bedzie probowal 
 *- jak najwiecej dopasowac, wiec 'else'-a podczepi
 *- pod najblizszego 'if'-a.
 */

s_cond: J_KW_IF '(' expr ')' statement {
	$$ = j_new_statement_cond($3, $5, NULL);
}
	| J_KW_IF '(' expr ')' statement J_KW_ELSE statement {
	$$ = j_new_statement_cond($3, $5, $7);
}
	;

s_expr: expr ';' {
	$$ = j_new_statement_expr($1);
}

expr: value {
	$$ = j_new_value_expr($1);
}
	| idexpr {
	$$ = j_new_id_expr($1);
}
	| function {
	$$ = j_new_function_expr($1);
}
	| expr J_OP_ADD expr {
	$$ = j_new_binary_expr(J_EXPR_ADD, $1, $3);
}
	| expr J_OP_SUB expr {
	$$ = j_new_binary_expr(J_EXPR_SUB, $1, $3);
}
	| expr J_OP_MUL expr {
	$$ = j_new_binary_expr(J_EXPR_MUL, $1, $3);
}
	| expr J_OP_DIV expr {
	$$ = j_new_binary_expr(J_EXPR_DIV, $1, $3);
}
	| expr J_OP_MOD expr {
	$$ = j_new_binary_expr(J_EXPR_MOD, $1, $3);
}
	| expr J_OP_EQ expr {
	$$ = j_new_binary_expr(J_EXPR_EQ, $1, $3);
}
	| expr J_OP_NE expr {
	$$ = j_new_binary_expr(J_EXPR_NE, $1, $3);
}
	| expr J_OP_GT expr {
	$$ = j_new_binary_expr(J_EXPR_GT, $1, $3);
}
	| expr J_OP_LT expr {
	$$ = j_new_binary_expr(J_EXPR_LT, $1, $3);
}
	| expr J_OP_GE expr {
	$$ = j_new_binary_expr(J_EXPR_GE, $1, $3);
}
	| expr J_OP_LE expr {
	$$ = j_new_binary_expr(J_EXPR_LE, $1, $3);
}
	| expr J_OP_OR expr {
	$$ = j_new_binary_expr(J_EXPR_OR, $1, $3);
}
	| expr J_OP_AND expr {
	$$ = j_new_binary_expr(J_EXPR_AND, $1, $3);
}
	| J_OP_NOT expr %prec REVERSE {
	$$ = j_new_unary_expr(J_EXPR_NOT, $2);
}
	| J_OP_SUB expr %prec REVERSE {
	$$ = j_new_unary_expr(J_EXPR_NEG, $2);
} | J_OP_ADD expr %prec REVERSE {
	/*- Plus niczego nie zmienia.
	 */
	$$ = $2;
}
	| '(' expr ')' {
	/*- W drzewie programu nie potrzebujemy onawiasowania,
	 *- wlasciwa kolejnosc wykonywania dzialan juz bedzie 
	 *- ustawiona prawidlowo.
	 */
	$$ = $2; 
}
	| '(' J_T_BOOLEAN ')' expr %prec REVERSE {
	$$ = j_new_casted_expr($4, J_BOOLEAN);
}
	| '(' J_T_INTEGER ')' expr %prec REVERSE {
	$$ = j_new_casted_expr($4, J_INTEGER);
}
	| '(' J_T_DOUBLE ')' expr %prec REVERSE {
	$$ = j_new_casted_expr($4, J_DOUBLE);
}
	| '(' J_T_VOID ')' expr %prec REVERSE {
	$$ = j_new_casted_expr($4, J_VOID);
}
	;

idexpr: id {
	$$ = j_new_idexpr(J_ID_ONLY, $1, 0);
} 
	| id '=' expr {
	$$ = j_new_idexpr(J_ASSIGN, $1, $3);
}
	| id J_OP_PLUSPLUS {
	$$ = j_new_idexpr(J_ASSIGN_INC, $1, 0);
}
	| id J_OP_MINUSMINUS {
	$$ = j_new_idexpr(J_ASSIGN_DEC, $1, 0);
}
	;

value: J_V_BOOLEAN {
	$$ = j_new_value(J_BOOLEAN, $1);
}
	| J_V_INTEGER {
	$$ = j_new_value(J_INTEGER, $1);
}
	| J_V_DOUBLE {
	$$ = j_new_value(J_DOUBLE, $1);
}
	| J_V_STRING {
	$$ = j_new_value(J_STRING, $1);
}

function: id '(' ')' {
	$$ = j_new_function($1, NULL);
}
	| id '(' function_args ')' {
	$$ = j_new_function($1, $3);
}
	;

function_args: expr {
	$$ = j_new_function_args($1, NULL);
}
	| expr ',' function_args {
	$$ = j_new_function_args($1, $3);
}
	;

type: J_T_BOOLEAN {
	$$ = J_BOOLEAN;
}
	| J_T_INTEGER {
	$$ = J_INTEGER;
}
	| J_T_DOUBLE {
	$$ = J_DOUBLE;
}
	| J_T_STRING {
	$$ = J_STRING;
}
	| J_T_VOID {
	$$ = J_VOID;
}
	;

id: J_TEXT {
	j_names_add(j_program->names, $1);
	$$ = j_new_id(j_names_get(j_program->names, $1));
	free($1);
}
	;

%%

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int colnum = 0;
int rownum = 1;
extern FILE *yyin;
FILE *output;

const char *input_file = NULL;
const char *output_file = NULL;
const char *exec_file = NULL;

const char *input_file_def = "<stdin>";
const char *output_file_def = "<stdout>";

int get_params(int argc, char **argv)
{
	int i = 1;

	while (i < argc) {
		if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i])) {
			fprintf(stderr, 
					"Usage: %s [options] <input-file.jl>\n\n"
					"Options:\n"
					"\t-h, --help,\n"
					"\t-o, --output <output-file.asm>\n"
					"\t-e, --exec <exec-file>\n"
					"\n", 
					argv[0]);
			return -1;
		} else if (!strcmp("-o", argv[i]) || !strcmp("--output", argv[i])) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Missing argument for option: '%s', try %s --help\n", argv[i], argv[0]);
				return -1;
			}

			if (output_file) {
				fprintf(stderr, "Already defined output file: '%s', use option '-o/--output' once\n", output_file);
				return -1;
			}

			if (exec_file) {
				fprintf(stderr, "Already defined exec file: '%s', use option '-o/--output' without option '-e/--exec'\n", exec_file);
				return -1;
			}

			output_file = argv[i + 1];

			i += 2;
		} else if (!strcmp("-e", argv[i]) || !strcmp("--exec", argv[i])) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Missing argument for option: '%s', try %s --help\n", argv[i], argv[0]);
				return -1;
			}

			if (exec_file) {
				fprintf(stderr, "Already defined exec file: '%s', use option '-e/--exec' once\n", exec_file);
				return -1;
			}

			if (output_file) {
				fprintf(stderr, "Already defined output file: '%s', use option '-e/--exec' without option '-o/--output'\n", output_file);
				return -1;
			}

			exec_file = argv[i + 1];

			i += 2;			
		} else if (!strncmp("-", argv[i], 1)) {
			fprintf(stderr, "Unknown option: '%s', try %s --help\n", argv[i], argv[0]);
			return -1;
		} else {
			if (input_file) {
				fprintf(stderr, "Already defined input file: '%s', use option '-i/--input' once\n", input_file);
				return -1;
			}

			input_file = argv[i];

			i += 1;
		}
	}
	
	if (!input_file) {
		input_file = input_file_def;
		yyin = stdin;
	} else {
		if (NULL == (yyin = fopen(input_file, "r"))) {
			fprintf(stderr, "Cannot open input file: '%s'\n", input_file);
			return -1;
		}
	}
	
	if (!output_file) {
		output_file = output_file_def;
		output = stdout;
	} else {
		if (NULL == (output = fopen(output_file, "w"))) {
			fprintf(stderr, "Cannot open output file: '%s'\n", output_file);
			return -1;
		}
	}

	if (exec_file) {
		char asm_file[strlen(exec_file) + 4];
		sprintf(asm_file, "%s.asm", exec_file);

		if (NULL == (output = fopen(asm_file, "w"))) {
			fprintf(stderr, "Cannot open output file: '%s'\n", asm_file);
			return -1;
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	/*- wczytujemy parametry -*/

	if (get_params(argc, argv)) return 1;

	/*- tworzymy tablice nazw -*/

	j_program = j_new_program(input_file);

	/*- uruchamiamy proces parsowania -*/
	
	yyparse();

	if (NULL == j_program->root) {
		fprintf(stderr, "%s:1,1: error: parse failed\n", input_file);
		return 1;
	}

	/*- dodajemy wbudowane funkcje -*/

	j_program_attach_std(j_program);

	/*- przeprowadzamy analize kontekstowa -*/

	int analyze = j_analyze_program(j_program);

	if (analyze) {
		/*- Wypisanie programu -*/

		j_program_generate_asm_code(j_program, output);
		//j_print_program(j_program, stderr);

		if (exec_file && output_file) { 
			/*- Budujemy binarke -*/
			char asm_file[strlen(exec_file) + 4];
			char obj_file[strlen(exec_file) + 2];
			sprintf(asm_file, "%s.asm", exec_file);
			sprintf(obj_file, "%s.o", exec_file);
			int status = 0;
			
			fflush(output); //- Sync

			switch (fork()) {
				case -1:
					fprintf(stderr, "fatal: cannot fork: %d, %s\n", errno, strerror(errno));
					exit(1);
				case 0:
					execlp("nasm", "nasm", "-f", "elf", asm_file, "-o", obj_file, NULL);
					fprintf(stderr, "fatal: cannot execlp(\"nasm\", ...): %d, %s\n", errno, strerror(errno));
					exit(1);
				default:
					if (wait(&status) == -1) {
						fprintf(stderr, "fatal: cannot wait for process('nasm'): %d, %s\n", errno, strerror(errno));
						exit(1);
					}
					if (status) {
						fprintf(stderr, "fatal: nasm compilation failure\n");
						exit(1);
					}
			}

			switch (fork()) {
				case -1:
					fprintf(stderr, "fatal: cannot fork: %d, %s\n", errno, strerror(errno));
					exit(1);
				case 0:
					execlp("gcc", "gcc", "-o", exec_file, obj_file, NULL);
					fprintf(stderr, "fatal: cannot execlp(\"gcc\", ...): %d, %s\n", errno, strerror(errno));
					exit(1);
				default:
					if (wait(&status) == -1) {
						fprintf(stderr, "fatal: cannot wait for process('gcc'): %d, %s\n", errno, strerror(errno));
						exit(1);
					}
					if (status) {
						fprintf(stderr, "fatal: gcc linkage failure\n");
						exit(1);				
					}
			}
		}
	}

	/*- zwolnienie zasobow -*/

	j_delete_program(j_program);

	return analyze ? 0:1;
}

