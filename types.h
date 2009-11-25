/**
 * @brief Typy danych
 * @author Piotr Truszkowski
 */

#ifndef _J_TYPES_H_
#define _J_TYPES_H_

#include <stdlib.h>

/*- Mozliwe typy wartosci -*/

typedef enum {
	J_BOOLEAN,
	J_INTEGER,
	J_DOUBLE,
	J_STRING,
	J_VOID,
	J_BAD_TYPE
} j_type_t;

/*- Struktura kontekstu -*/

struct j_context_t;

/*- Deklaracje pozostalych struktur -*/

struct j_def_fun_list_t;
struct j_def_fun_t;
struct j_fun_args_t;
struct j_statement_block_t;
struct j_statement_list_t;
struct j_statement_t;
struct j_statement_decl_t;
struct j_statement_assign_t;
struct j_statement_cond_t;
struct j_statement_iter_t;
struct j_statement_return_t;
struct j_statement_expr_t;
struct j_decl_list_t;
struct j_decl_t;
struct j_expr_t;
struct j_function_t;
struct j_function_args_t;
struct j_idexpr_t;

/*- definicje struktur -*/

typedef struct j_id_t {
	size_t                         number;      //- unikalny ID dla nazwy identyfikatora
	int                            row, col;    //- pozycja w pliku zrodlowym
	struct j_context_t            *ctx;         //- kontekst deklaracji zmiennej
} j_id_t;

typedef struct j_value_t {
	j_type_t                       type;        //- typ wartosci
	char                          *value;       //- wartosc zapisana jako string
	int                            row, col;    //- pozycja w pliku zrodlowym
	size_t                         idnumber;    //- unikalny ID dla wartosci
	struct j_value_t              *list;        //- lista wszystkich stalych wartosci
} j_value_t;

typedef struct j_def_fun_list_t {
	struct j_def_fun_t            *def_fun;     //- definicja funkcji
	struct j_def_fun_list_t       *def_fun_list;//- nastepna definicja funkcji na liscie
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_def_fun_list_t;

typedef struct j_def_fun_t {
	j_type_t                       type;        //- typ zwracanej wartosci przez funkcje
	struct j_id_t                 *id;          //- identyfikator funkcji
	struct j_fun_args_t           *fun_args;    //- lista argumentow funkcji
	struct j_statement_block_t    *block;       //- blok instrukcji dla funkcji
	int                            row, col;    //- pozycja w pliku zrodlowym
	const char                    *builtin;     //- bezposredni kod asemblera (dla f predefiniowych/wbudowanych)
	struct j_context_t            *ctx;         //- kontekst jaki tworzy funkcja
	struct j_def_fun_t            *list;        //- lista wszystkich funkcji (w tym zagniezdzonych)
	size_t                         idnumber;    //- unikalny ID dla funkcji 
} j_def_fun_t;

typedef struct j_fun_args_t {
	j_type_t                       type;        //- typ argumentu
	struct j_id_t                 *id;          //- identyfikator argumentu
	struct j_fun_args_t           *fun_args;    //- nastepny argument na liscie
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_fun_args_t;

typedef struct j_statement_block_t {
	struct j_statement_list_t     *list;        //- lista instrukcji w bloku
	int                            row, col;    //- pozycja w pliku zrodlowym
	struct j_context_t            *ctx;         //- kontekst jaki tworzy blok (moze byc dziedziczony z funkcji)
} j_statement_block_t;

typedef struct j_statement_list_t {
	struct j_statement_t          *statement;   //- instrukcja
	struct j_statement_list_t     *list;        //- nastepna instrukcja na liscie
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_statement_list_t;

typedef enum {
	J_ST_BLOCK, 
	J_ST_DECL, 
	J_ST_COND, 
	J_ST_ITER, 
	J_ST_RETURN, 
	J_ST_EXPR,
	J_ST_FUNC
} j_statement_type_t;

typedef union {
	struct j_statement_block_t    *block;       //- instrukcja blokowa
	struct j_statement_decl_t     *decl;        //- instrukcja deklaracji
	struct j_statement_cond_t     *cond;        //- instrukcja warunkowa
	struct j_statement_iter_t     *iter;        //- instrukcja iteracji
	struct j_statement_return_t   *ret;         //- instrukcja powrotu
	struct j_statement_expr_t     *expr;        //- instrukcja wyrazenia
	struct j_def_fun_t            *func;        //- definicja funkcji zagniezdzonej
	void                          *ptr;         //- wskaznik typu void
} j_statement_union_t;

typedef struct j_statement_t {
	j_statement_type_t             type;        //- typ instrukcji (j.w.)
	j_statement_union_t            statement;   //- instrukcja (j.w.)
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_statement_t;

typedef struct j_statement_decl_t {
	j_type_t                       type;        //- typ deklarowanej zmiennej
	struct j_decl_list_t          *list;        //- lista zmiennych
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_statement_decl_t;

typedef struct j_decl_list_t {
	struct j_decl_t               *decl;        //- deklaracja
	struct j_decl_list_t          *list;        //- nastepna deklaracja na liscie
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_decl_list_t;

typedef struct j_decl_t {
	struct j_id_t                 *id;          //- identyfikator zmiennej
	struct j_expr_t               *expr;        //- wartosc zmiennej
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_decl_t;

typedef enum {
	J_ID_ONLY,                                  //- jedynie id
	J_ASSIGN,                                   //- przypisanie przez '='
	J_ASSIGN_DEC,                               //- dekrementacja
	J_ASSIGN_INC                                //- inkrementacja
} j_idexpr_type_t;

typedef struct j_idexpr_t {
	j_idexpr_type_t                type;        //- typ przypisania
	struct j_id_t                 *id;          //- identyfikator zmiennej
	struct j_expr_t               *expr;        //- przypisywana wartosci (jesli type = J_ASSIGN)
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_idexpr_t;

typedef struct j_statement_cond_t {
	struct j_expr_t               *expr;        //- wyrazenie bedace warunkiem
	struct j_statement_t          *iftrue;      //- instrukcja do wykonania gdy warunek jest prawdziwy
	struct j_statement_t          *iffalse;     //- instrukcja do wykonania gdy warunek jest falszywy
	int                            row, col;    //- pozycja w pliku zrodlowym
	size_t                         idnumber;    //- unikalny ID instrukcji warunkowej
} j_statement_cond_t;

typedef struct j_statement_iter_t {
	struct j_expr_t               *expr;        //- wyrazenie bedace warunkiem
	struct j_expr_t               *begin;       //- przypisanie startowe
	struct j_expr_t               *step;        //- przypisanie po kazdej iteracji
	struct j_statement_t          *statement;   //- instrukcja do wykonania w iteracji
	int                            row, col;    //- pozycja w pliku zrodlowym
	size_t                         idnumber;    //- unikalny ID instrukcji iteracji
} j_statement_iter_t;

typedef struct j_statement_expr_t { 
	struct j_expr_t               *expr;        //- wyrazenie
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_statement_expr_t;

typedef struct j_statement_return_t {
	struct j_expr_t               *expr;        //- zwracane wyrazenie
	int                            row, col;    //- pozycj w pliku zrodlowym
	int                            stack;       //- rozmiar stosu jaki trzeba zwinac przed powrotem
} j_statement_return_t;

typedef enum {
	J_EXPR_VALUE,                               //- wyrazenie jest wartoscia
	J_EXPR_FUNCTION,                            //- wyrazenie jest wolaniem funkcji
	J_EXPR_ID,                                  //- wyrazenie jest zmienna
	J_EXPR_NOT,                                 //- unarna operacja: '!'
	J_EXPR_NEG,                                 //- unarna operacja: '-'
	J_EXPR_ADD,                                 //- binarna operacja: '+'
	J_EXPR_SUB,                                 //- binarna operacja: '-'
	J_EXPR_MUL,                                 //- binarna operacja: '*'
	J_EXPR_DIV,                                 //- binarna operacja: '/'
	J_EXPR_MOD,                                 //- binarna operacja: '%'
	J_EXPR_AND,                                 //- binarna operacja: '&&'
	J_EXPR_OR,                                  //- binarna operacja: '||'
	J_EXPR_EQ,                                  //- binarna operacja: '=='
	J_EXPR_NE,                                  //- binarna operacja: '!='
	J_EXPR_LT,                                  //- binarna operacja: '<'
	J_EXPR_GT,                                  //- binarna operacja: '>'
	J_EXPR_LE,                                  //- binarna operacja: '<='
	J_EXPR_GE,                                  //- binarna operacja: '>='
	J_EXPR_CAST                                 //- operacja rzutowania
} j_expr_type_t;

typedef struct j_expr_t {
	j_expr_type_t                  etype;       //- typ wyrazenia
	struct j_expr_t               *left;        //- lewy argument operacji (jesli jest)
	struct j_expr_t               *right;       //- prawy argument operacji (jesli jest)
	struct j_value_t              *value;       //- wartosc (jesli jest)
	struct j_function_t           *function;    //- wolana funkcja (jesli jest)
	struct j_idexpr_t             *id;          //- identykikator zmiennej (jesli jest)
	j_type_t                       type;        //- typ wyrazenia
	int                            row, col;    //- pozycja w pliku zrodlowym
	size_t                         idnumber;    //- unikalny ID wyrazenia
} j_expr_t;

typedef struct j_function_t {
	struct j_id_t                 *id;          //- identyfikator wolanej funkcji
	struct j_function_args_t      *args;        //- lista argumentow
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_function_t;

typedef struct j_function_args_t {
	struct j_expr_t               *expr;        //- wartosc argumentu
	struct j_function_args_t      *args;        //- nastepny argument na liscie
	int                            row, col;    //- pozycja w pliku zrodlowym
} j_function_args_t;

// Makro sprawdzajace czy t jest liczba
#define j_type_is_numeric(t) \
	( (t) == J_BOOLEAN || \
		(t) == J_INTEGER || \
		(t) == J_DOUBLE )

// Makro zwracajace opis typy na string
#define j_type_to_string(t) \
	( ((t) == J_BOOLEAN) ? "boolean" : \
		((t) == J_INTEGER) ? "integer" : \
		((t) == J_DOUBLE ) ? "double"  : \
		((t) == J_VOID   ) ? "void"    : \
		((t) == J_STRING ) ? "string"  : \
		"<<unknown>>" )

/**
 * @brief Ktory liczbowy typ jest szerszy?
 * 'boolean' < 'int' < 'double'.
 *  = 0 - te same typy
 *  > 0 - t2 ma szerszy typ
 *  < 0 - t1 ma szerszy typ
 */
#define j_type_compare(t1, t2) \
	( ((int)(t2)) - ((int)(t1)) )

// Makro sprawdzajace czy wyrazenie jest jedno argumentowe
#define j_expr_is_unary(e) \
	( J_EXPR_NOT  == (e)->etype || \
		J_EXPR_NEG  == (e)->etype || \
		J_EXPR_CAST == (e)->etype )

// Makro sprawdzajace czy wyrazenie jest dwu argumentowe
#define j_expr_is_binary(e) \
	( J_EXPR_ADD == (e)->etype || \
		J_EXPR_SUB == (e)->etype || \
		J_EXPR_MUL == (e)->etype || \
		J_EXPR_DIV == (e)->etype || \
		J_EXPR_MOD == (e)->etype || \
		J_EXPR_AND == (e)->etype || \
		J_EXPR_OR  == (e)->etype || \
		J_EXPR_EQ  == (e)->etype || \
		J_EXPR_NE  == (e)->etype || \
		J_EXPR_LT  == (e)->etype || \
		J_EXPR_GT  == (e)->etype || \
		J_EXPR_LE  == (e)->etype || \
		J_EXPR_GE  == (e)->etype )

#endif

