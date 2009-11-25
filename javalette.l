/**
 * @brief Lekser jezyka javalette.
 * @author Piotr Truszkowski
 */

%{

#include <math.h>
#include <assert.h>
#include <string.h>
#include "javalette.tab.h"
#include "javalette.h"

extern int colnum;
extern int rownum;
extern const char *input_file;

%}

%%

\#[^\n]*\n { colnum = 0; ++rownum; }

\/\/[^\n]*\n { colnum = 0; ++rownum; }

\/\*([^\*]|[\r\n]|(\*+([^\*\/]|[\r\n])))*\*+\/ { 
	int i, n = strlen(yytext);
	for (i = 0; i < n; ++i) {
		if ('\n' == yytext[i]) { colnum = 0; ++rownum; }
		else { ++colnum; }
	}
}

[0-9]+\.[0-9]* {
	colnum += strlen(yytext);
	yylval.dvalue = strdup(yytext);
	assert(NULL != yylval.dvalue);
	return J_V_DOUBLE;
}

[0-9]+ {
	colnum += strlen(yytext);
	yylval.ivalue = strdup(yytext);
	assert(NULL != yylval.ivalue);
	return J_V_INTEGER;
}

(true|false) {
	colnum += strlen(yytext);
	yylval.bvalue = (strcmp("true", yytext) == 0) ? strdup("1") : strdup("0");
	assert(NULL != yylval.bvalue);
	return J_V_BOOLEAN;
}

\"[^\"]*\" {
	colnum += strlen(yytext);
	yylval.svalue = strdup(yytext); 
	assert(NULL != yylval.svalue);
	return J_V_STRING;
}

"if"      { colnum += strlen(yytext); return J_KW_IF;     }
"else"    { colnum += strlen(yytext); return J_KW_ELSE;   }
"for"     { colnum += strlen(yytext); return J_KW_FOR;    }
"while"   { colnum += strlen(yytext); return J_KW_WHILE;  }
"return"  { colnum += strlen(yytext); return J_KW_RETURN; }

"boolean" { colnum += strlen(yytext); return J_T_BOOLEAN; }
"double"  { colnum += strlen(yytext); return J_T_DOUBLE;  }
"int"     { colnum += strlen(yytext); return J_T_INTEGER; }
"void"    { colnum += strlen(yytext); return J_T_VOID;    }

[a-zA-Z][a-zA-Z0-9_]* {
	colnum += strlen(yytext);
	yylval.text = strdup(yytext);
	assert(NULL != yylval.text);
	return J_TEXT;
}

\+ { colnum += 1; return J_OP_ADD; }
\- { colnum += 1; return J_OP_SUB; }
\* { colnum += 1; return J_OP_MUL; }
\/ { colnum += 1; return J_OP_DIV; }
\% { colnum += 1; return J_OP_MOD; }
\! { colnum += 1; return J_OP_NOT; }
\< { colnum += 1; return J_OP_LT; }
\> { colnum += 1; return J_OP_GT; }
\=\= { colnum += 2; return J_OP_EQ; }
\!\= { colnum += 2; return J_OP_NE; }
\<\= { colnum += 2; return J_OP_LE; }
\>\= { colnum += 2; return J_OP_GE; }
\|\| { colnum += 2; return J_OP_OR; }
\&\& { colnum += 2; return J_OP_AND; }
\+\+ { colnum += 2; return J_OP_PLUSPLUS; }
\-\- { colnum += 2; return J_OP_MINUSMINUS; }

[\(\)\{\}\[\]\,\;\=] {
	++colnum;
	return *yytext;
}

[ \t] { 
	++colnum; 
}

\n { 
	colnum = 0;
	++rownum; 
}

. { 
	++colnum; 
	fprintf(stderr, "%s:%d,%d: error: unknown symbol: '%s'\n", input_file, rownum, colnum, yytext); 
	return *yytext;
}

%%


