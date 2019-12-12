%{
#define YYDEBUG 1
#define ANY -1
#define INT 0
#define BOOL 1

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"
%}



%code requires {
  #include "ast.h"
  #include "symlist.h"
  #include "code.h"
  #include "mips.h"

  StmtList* root;
  InstrList* ic3;
  List* symList;

  int validate_expression(Expr* expr, int type);
  int validate_operation(Expr* lhs, Expr* rhs, int type);
  int validate_var(string var, int type);
  void generateASM();
  void compiler();
}



%nonassoc
CMD_FUNC
CMD_MAIN
CMD_IF
CMD_ELSE
CMD_WHILE
VAR
VARP
TYPE_INT
TYPE_BOOL
EOL
ASPA
COMMA
SYM_OB
SYM_CB
SYM_OP
SYM_CP
OP_ATRIB
CMD_VAR
CMD_SCAN
CMD_PRINT

%left
OP_OR
OP_AND
OP_EQ OP_DIFF OP_LT OP_LTE OP_GT OP_GTE
OP_SUM OP_SUB
OP_MULT OP_DIV OP_MOD
SYM_SC

%union
{
  char* tyString;
  int tyInt;
  StmtList* tyStmtList;
  Stmt* tyStmt;
  Attrib* tyAttrib;
  Expr* tyExpr;
  Print* tyPrint;
  Read* tyRead;
  While* tyWhile;
  If* tyIf;
}

%type <tyString> VAR
%type <tyString> VARP
%type <tyInt> TYPE_BOOL
%type <tyInt> TYPE_INT
%type <tyStmtList> Program
%type <tyStmtList> Main
%type <tyStmtList> Block
%type <tyStmtList> StatementList
%type <tyExpr> Expression
%type <tyExpr> Term
%type <tyExpr> Factor
%type <tyAttrib> Assignment
%type <tyAttrib> Declaration
%type <tyAttrib> AttribStmt
%type <tyWhile> WhileStmt
%type <tyIf> IfStmt
%type <tyPrint> InputStmt
%type <tyRead> OutputStmt
%type <tyStmt> Statement



%%
Program:
/* nothing */                                                   { root = NULL; }
| Main
{
  root = $1;
  ic3 = genIC(root);
  compiler();
}
;


Separator:
SYM_SC
| EOL
;


Main:
CMD_FUNC CMD_MAIN SYM_OP SYM_CP Block                           { $$ = $5; }
| CMD_FUNC CMD_MAIN SYM_OP SYM_CP Block Separator               { $$ = $5; }
;


Block:
SYM_OB StatementList SYM_CB                                     { $$ = $2; }
;


StatementList:
/* nothing */                                                   { $$ = NULL; }
| Statement Separator StatementList                             { $$ = ast_stmtList($1,$3); }
;


Statement:
AttribStmt                                                      { $$ = ast_stmt_attrib($1); }
| Expression                                                    { $$ = ast_stmt_expr($1); }
| Block                                                         { $$ = ast_stmt_block($1); }
| IfStmt                                                        { $$ = ast_stmt_if($1); }
| WhileStmt                                                     { $$ = ast_stmt_while($1); }
| InputStmt                                                     { $$ = ast_stmt_print($1); }
| OutputStmt                                                    { $$ = ast_stmt_read($1); }
;

InputStmt:
CMD_PRINT SYM_OP VARP SYM_CP                                    { $$ = ast_print_input_word($3); }
;


OutputStmt:
CMD_SCAN SYM_OP VAR COMMA VAR SYM_CP                            { $$ = ast_print_output($3,$5); }
;


IfStmt:
CMD_IF Expression Block
                                                                {
                                                                  validate_expression($2,BOOL);
                                                                  $$ = ast_if(NULL,$2,$3,NULL);
                                                                }
| CMD_IF AttribStmt SYM_SC Expression Block
                                                                {
                                                                  validate_expression($4,BOOL);
                                                                  $$ = ast_if($2,$4,$5,NULL);
                                                                }
| CMD_IF Expression Block CMD_ELSE IfStmt
                                                                {
                                                                  validate_expression($2,BOOL);
                                                                  $$ = ast_if(NULL,$2,$3,ast_stmtList(ast_stmt_if($5),NULL));
                                                                }
| CMD_IF Expression Block CMD_ELSE Block
                                                                {
                                                                  validate_expression($2,BOOL);
                                                                  $$ = ast_if(NULL,$2,$3,$5);
                                                                }
| CMD_IF AttribStmt SYM_SC Expression Block CMD_ELSE IfStmt
                                                                {
                                                                  validate_expression($4,BOOL);
                                                                  $$ = ast_if($2,$4,$5,ast_stmtList(ast_stmt_if($7),NULL));
                                                                }
| CMD_IF AttribStmt SYM_SC Expression Block CMD_ELSE Block
                                                                {
                                                                  validate_expression($4,BOOL);
                                                                  $$ = ast_if($2,$4,$5,$7);
                                                                }
;

WhileStmt:
CMD_WHILE Block                                                 { $$ = ast_while(NULL,$2); }
| CMD_WHILE Expression Block
                                                                {
                                                                  validate_expression($2,BOOL);
                                                                  $$ = ast_while($2,$3);
                                                                }
;


AttribStmt:
/* nothing */                                                   { $$ = NULL; }
| Declaration                                                   { $$ = $1; }
| Assignment                                                    { $$ = $1; }
;

Assignment:
VAR OP_ATRIB Expression
                                                                {
                                                                  validate_var($1,$3->expr_type);
                                                                  $$ = ast_attrib_assign($1,$3);
                                                                }
;

Declaration:
CMD_VAR VAR OP_ATRIB Expression
                                                                {
                                                                  if(list_get(symList,$2) != -1)
                                                                    yyerror("Already defined.");

                                                                  symList = list_store(symList,$2,$4->expr_type);
                                                                  $$ = ast_attrib_decl($2,$4);
                                                                }
;

Expression:
Expression OP_EQ Expression
                                                                {
                                                                  validate_operation($1,$3,ANY);
                                                                  $$ = ast_expr_op_bool($1,OP_EQ,$3);
                                                                }
| Expression OP_DIFF Expression
                                                                {
                                                                  validate_operation($1,$3,ANY);
                                                                  $$ = ast_expr_op_bool($1,OP_DIFF,$3);
                                                                }
| Expression OP_LT Expression
                                                                {
                                                                  validate_operation($1,$3,ANY);
                                                                  $$ = ast_expr_op_bool($1,OP_LT,$3);
                                                                }
| Expression OP_LTE Expression
                                                                {
                                                                  validate_operation($1,$3,ANY);
                                                                  $$ = ast_expr_op_bool($1,OP_LTE,$3);
                                                                }
| Expression OP_GT Expression
                                                                {
                                                                  validate_operation($1,$3,ANY);
                                                                  $$ = ast_expr_op_bool($1,OP_GT,$3);
                                                                }
| Expression OP_GTE Expression
                                                                {
                                                                  validate_operation($1,$3,ANY);
                                                                  $$ = ast_expr_op_bool($1,OP_GTE,$3);
                                                                }
| Expression OP_AND Expression
                                                                {
                                                                  validate_operation($1,$3,BOOL);
                                                                  $$ = ast_expr_op_bool($1,OP_AND,$3);
                                                                }
| Expression OP_OR Expression
                                                                {
                                                                  validate_operation($1,$3,BOOL);
                                                                  $$ = ast_expr_op_bool($1,OP_OR,$3);
                                                                }
| Expression OP_SUM Term
                                                                {
                                                                  validate_operation($1,$3,INT);
                                                                  $$ = ast_expr_op_int($1,OP_SUM,$3);
                                                                }
| Expression OP_SUB Term
                                                                {
                                                                  validate_operation($1,$3,INT);
                                                                  $$ = ast_expr_op_int($1,OP_SUB,$3);
                                                                }
| Term
                                                                {
                                                                  $$ = $1;
                                                                }
;

Term:
Term OP_MULT Factor
                                                                {
                                                                  validate_operation($1,$3,INT);
                                                                  $$ = ast_expr_op_int($1,OP_MULT,$3);
                                                                }
| Term OP_DIV Factor
                                                                {
                                                                  validate_operation($1,$3,INT);
                                                                  $$ = ast_expr_op_int($1,OP_DIV,$3);
                                                                }
| Term OP_MOD Factor
                                                                {
                                                                  validate_operation($1,$3,INT);
                                                                  $$ = ast_expr_op_int($1,OP_MOD,$3);
                                                                }
| Factor
                                                                {
                                                                  $$ = $1;
                                                                }
;

Factor:
SYM_OP Expression SYM_CP
                                                                {
                                                                  $$ = $2;
                                                                }
| TYPE_INT
                                                                {
                                                                  $$ = ast_expr_primitive_int($1);
                                                                }
| TYPE_BOOL
                                                                {
                                                                  $$ = ast_expr_primitive_bool($1);
                                                                }
| VAR
                                                                {
                                                                  int t = validate_var($1,ANY);

                                                                  if(t == 0)
                                                                    $$ = ast_expr_identifier_int($1);
                                                                    else
                                                                      $$ = ast_expr_identifier_bool($1);
                                                                }
;
%%



int validate_expression(Expr* expr, int type)
{
  int t = expr->expr_type;

  if(type != -1 && t != type)
    yyerror("Expression with wrong type!");

  return t;
}



int validate_operation(Expr* lhs, Expr* rhs, int type)
{
  int tl = lhs->expr_type;
  int tr = rhs->expr_type;

  if(tl != tr)
    yyerror("Operation with different types!");

  validate_expression(lhs,type);
  return tl;
}



int validate_var(string var, int type)
{
  int t = list_get(symList,var);

  if(t == -1)
    yyerror("Variable not defined!");
  else if(type != -1 && t != type)
    yyerror("Wrong type!");

  return t;
}


void compiler()
{
  printAST(root,0);
  printMIPS(ic3);
}



void yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
    exit(1);
}



int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Try: ./rust_in_pieces prog.rs\n");
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if(!yyin)
    {
        perror("Error");
        return 1;
    }

    yydebug = 0;
    return yyparse();
}
