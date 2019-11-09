%option noyywrap nodefault
%x COMMENT

%{
#include "common.h"
#include "parser.tab.h"

int send_EOL = 1;
%}

%%

"/*"                          { BEGIN COMMENT; }
<COMMENT>"*/\n"               { BEGIN INITIAL; }
<COMMENT>([^*]|\n)+|.
<COMMENT><<EOF>>              { yyerror("Comment not terminated"); }

"//".*\n                      { /* ignore */ }

;/;                           |
^([ \t]*\n)                   |
[ \t]                         { /* ignore */ }

\{/[ \t]*\n                   { send_EOL = 0; return SYM_OB; }

;/[ \t]*\n                    { send_EOL = 0; return SYM_SC; }

\n                            { if(send_EOL) return EOL; else send_EOL = 1; }


"fn"                          { return CMD_FUNC; }

"main"                        { return CMD_MAIN; }

"read_line"                   { return CMD_SCAN; }

"println!"                    { return CMD_PRINT; }

"while"                       { return CMD_WHILE; }

"if"                          { return CMD_IF; }

"else"                        { return CMD_ELSE; }

"let"                         { return CMD_VAR; }

"="                           { return OP_ATRIB; }

"+"                           { return OP_SUM; }

"-"                           { return OP_SUB; }

"*"                           { return OP_MULT; }

"/"                           { return OP_DIV; }

"%"                           { return OP_MOD; }

"=="                          { return OP_EQ; }

"!="                          { return OP_DIFF; }

"<"                           { return OP_LT; }

">"                           { return OP_GT; }

"<="                          { return OP_LTE; }

">="                          { return OP_GTE; }

"&&"                          { return OP_AND; }

"||"                          { return OP_OR; }

"true"                        { yylval.tyInt=1; return TYPE_BOOL; }

"false"                       { yylval.tyInt=0; return TYPE_BOOL; }

[0-9]+(\.[0-9]*)?             { yylval.tyInt = atof(yytext); return TYPE_INT; }

[a-zA-Z]+([a-zA-Z]|[0-9])*    { yylval.tyString = strdup(yytext); return VAR; }

"{"                           { return SYM_OB; }

"}"                           { return SYM_CB; }

"("                           { return SYM_OP; }

")"                           { return SYM_CP; }

";"                           { return SYM_SC; }

<<EOF>> { yyterminate(); }

.                             { yyerror("Unexpected."); }

%%
