#ifndef __common_h__
#define __common_h__

extern int yylex();
extern char* yytext;
extern FILE* yyin;

void yyerror(const char *msg);

#endif
