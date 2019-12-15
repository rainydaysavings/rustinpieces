#define BRANCH_SIZE 3

#include <stdlib.h>
#include <stdio.h>
#include "parser.tab.h"
#include "ast.h"

StmtList* ast_stmtList(Stmt* stmt, StmtList* next)
{
  StmtList* s = (StmtList*)malloc(sizeof(StmtList));
  s->stmt = stmt;
  s->next = next;
  return s;
}


Stmt* ast_stmt_attrib(Attrib* attrib)
{
  Stmt* s = (Stmt*)malloc(sizeof(Stmt));
  s->kind = STMT_ATTRIB;
  s->core.attrib = attrib;
  return s;
}

Stmt* ast_stmt_expr(Expr* expr)
{
  Stmt* s = (Stmt*)malloc(sizeof(Stmt));
  s->kind = STMT_EXPR;
  s->core.expr = expr;
  return s;
}

Stmt* ast_stmt_block(StmtList* block)
{
  Stmt* s = (Stmt*)malloc(sizeof(Stmt));
  s->kind = STMT_BLOCK;
  s->core.block = block;
  return s;
}

Stmt* ast_stmt_print(Print* print)
{
  Stmt* s = (Stmt*)malloc(sizeof(Stmt));
  s->kind = STMT_PRINT;
  s->core.print = print;
  return s;
}
Stmt* ast_stmt_read(Read* read)
{
  Stmt* s = (Stmt*)malloc(sizeof(Stmt));
  s->kind = STMT_READ;
  s->core.read = read;
  return s;
}
Stmt* ast_stmt_while(While* whileCmd)
{
  Stmt* s = (Stmt*)malloc(sizeof(Stmt));
  s->kind = STMT_WHILE;
  s->core._while = whileCmd;
  return s;
}

Stmt* ast_stmt_if(If* ifCmd)
{
  Stmt* s = (Stmt*)malloc(sizeof(Stmt));
  s->kind = STMT_IF;
  s->core._if = ifCmd;
  return s;
}


Attrib* ast_attrib_assign(var id, Expr* expr)
{
  Attrib* a = (Attrib*)malloc(sizeof(Attrib));
  a->kind = ATTRIB_ASSIGN;
  a->id = id;
  a->expr = expr;

  return a;
}

Attrib* ast_attrib_decl(var id, Expr* expr)
{
  Attrib* a = (Attrib*)malloc(sizeof(Attrib));
  a->kind = ATTRIB_DECL;
  a->id = id;
  a->expr = expr;
  return a;
}

// Expressions
Expr* ast_expr_primitive_int(primitive val)
{
  Expr* e = (Expr*)malloc(sizeof(Expr));
  e->kind = EXPR_PRIMITIVE;
  e->expr_type = EXPR_TYPE_INT;
  e->core.val = val;
  return e;
}

Expr* ast_expr_primitive_bool(primitive val)
{
  Expr* e = (Expr*)malloc(sizeof(Expr));
  e->kind = EXPR_PRIMITIVE;
  e->expr_type = EXPR_TYPE_BOOL;
  e->core.val = val;
  return e;
}

Expr* ast_expr_identifier_int(var id)
{
  Expr* e = (Expr*)malloc(sizeof(Expr));
  e->kind = EXPR_VAR;
  e->expr_type = EXPR_TYPE_INT;
  e->core.id = id;
  return e;
}

Expr* ast_expr_identifier_bool(var id)
{
  Expr* e = (Expr*)malloc(sizeof(Expr));
  e->kind = EXPR_VAR;
  e->expr_type = EXPR_TYPE_BOOL;
  e->core.id = id;
  return e;
}

Expr* ast_expr_op_int(Expr* l_expr, token op, Expr* r_expr)
{
  Expr* e = (Expr*)malloc(sizeof(Expr));
  e->kind = EXPR_OP;
  e->expr_type = EXPR_TYPE_INT;
  e->core.op.l_expr = l_expr;
  e->core.op.r_expr = r_expr;
  e->core.op.binop = op;
  return e;
}

Expr* ast_expr_op_bool(Expr* l_expr, token op, Expr* r_expr)
{
  Expr* e = (Expr*)malloc(sizeof(Expr));
  e->kind = EXPR_OP;
  e->expr_type = EXPR_TYPE_BOOL;
  e->core.op.l_expr = l_expr;
  e->core.op.r_expr = r_expr;
  e->core.op.binop = op;
  return e;
}

// IO
Print* ast_print_input_word(var id)
{
  Print* f = (Print*)malloc(sizeof(Print));
  f->kind = PRINT_INPUT;
  f->type = WORD;
  f->id = id;
  return f;
}
Read* ast_print_output(var str, var input)
{
  Read* f = (Read*)malloc(sizeof(Read));
  f->kind = print_Output;
  f->in = input;
  f->str = str;
  return f;
}


While* ast_while(Expr* cond, StmtList* block)
{
  While* f = (While*)malloc(sizeof(While));
  f->cond = cond;
  f->block = block;
  return f;
}

If* ast_if(Attrib* attrib, Expr* cond, StmtList* block, StmtList* elseBlock)
{
  If* i = (If*)malloc(sizeof(If));
  i->attrib = attrib;
  i->cond = cond;
  i->block = block;
  i->elseBlock = elseBlock;
  return i;
}


void printAST(StmtList* root, int depth)
{
  printf("main\n");
  printStmtList(root, depth+1);
}

void printStmtList(StmtList* stmtList, int depth)
{
  int empty = 1;
  StmtList* p = stmtList;

  while(p)
  {
    if(p->stmt && !(p->stmt->kind == STMT_ATTRIB && !p->stmt->core.attrib) )
    {
      empty = 0;
      printStmt(p->stmt,depth);
    }

    p = p->next;
  }

  if(empty)
  {
    printTab(depth*BRANCH_SIZE);
    printf("empty\n");
  }
}

void printStmt(Stmt* stmt, int depth)
{
  if(!stmt)
    return;

  switch(stmt->kind)
  {
  case STMT_ATTRIB:
    printAttrib(stmt->core.attrib, depth);  break;
  case STMT_EXPR:
    printExpr(stmt->core.expr, depth);      break;
  case STMT_BLOCK:
    printStmtList(stmt->core.block, depth); break;
  case STMT_IF:
    printIf(stmt->core._if, depth);         break;
  case STMT_WHILE:
    printWhile(stmt->core._while, depth);       break;
  case STMT_PRINT:
    printOutput(stmt->core.print, depth);       break;
  case STMT_READ:
    printRead(stmt->core.read, depth);       break;
  default:
    printPrint(stmt->core.print, depth);
  }

  putchar('\n');
}

void printRead(Read* attrib, int depth){
  if(!attrib)
    return;
  printTab((depth+1)*BRANCH_SIZE);
  printf("read_line\n");

  printTab((depth+2)*BRANCH_SIZE);
  printf("%s\n", attrib->str);
  printTab((depth+3)*BRANCH_SIZE);
  printf("%s\n", attrib->in);
}

void printOutput(Print* attrib, int depth){
  if(!attrib)
    return;
  printTab((depth+1)*BRANCH_SIZE);
  printf("println!\n");

  printTab((depth+2)*BRANCH_SIZE);
  printf("%s\n", attrib->id);
}
void printAttrib(Attrib* attrib, int depth)
{

  if(!attrib)
    return;
  printTab((depth+1)*BRANCH_SIZE);
  printf("%s\n", attrib->id);

  printTab(depth*BRANCH_SIZE);
  printf("=\n");

  printExpr(attrib->expr, depth+1);
}

void printExpr(Expr* expr, int depth)
{
  if(!expr)
    return;

  switch(expr->kind)
  {
  case EXPR_PRIMITIVE:

    switch(expr->expr_type)
    {
    case EXPR_TYPE_INT:
      printTab(depth*BRANCH_SIZE);
      printf("%d\n",expr->core.val);
      break;
    default:
      printTab(depth*BRANCH_SIZE);
      printf("%s\n",(expr->core.val ? "true" : "false"));
    }
    break;

  case EXPR_VAR:

    printTab(depth*BRANCH_SIZE);
    printf("%s\n",expr->core.id);
    break;

  default:

    printExpr(expr->core.op.l_expr, depth+1);
    printOp(expr->core.op.binop, depth);
    printExpr(expr->core.op.r_expr, depth+1);
  }
}

void printIf(If* _if, int depth)
{
  if(!_if)
    return;

  printTab(depth*BRANCH_SIZE);
  printf("if\n");

  printTab((depth+1)*BRANCH_SIZE);
  printf("head\n");
  printAttrib(_if->attrib, depth+2);
  printExpr(_if->cond, depth+2);

  printTab((depth+1)*BRANCH_SIZE);
  printf("body\n");
  printStmtList(_if->block, depth+2);

  printTab((depth+1)*BRANCH_SIZE);
  printf("else\n");
  printStmtList(_if->elseBlock, depth+2);
}

void printWhile(While* _while, int depth)
{
  if(!_while)
    return;

  printTab(depth*BRANCH_SIZE);
  printf("while\n");

  printTab((depth+1)*BRANCH_SIZE);
  printf("head\n");

  if(_while->cond == NULL)
  {
    printTab((depth+2)*BRANCH_SIZE);
    printf("empty\n");
  }
  else
  {
    printExpr(_while->cond, depth+2);
  }

  printTab((depth+1)*BRANCH_SIZE);
  printf("body\n");
  printStmtList(_while->block, depth+2);
}

void printPrint(Print* print, int depth)
{
  if(print == NULL)
    return;

  printTab(depth*BRANCH_SIZE);
  printf("%s %s", (print->kind==PRINT_INPUT ? "scan" : "print"), print->id);
}

void printOp(Binop op, int depth)
{
  printTab(depth*BRANCH_SIZE);

  switch(op)
  {
  case OP_AND:
    printf("and"); break;
  case OP_OR:
    printf("or");  break;
  case OP_EQ:
    printf("==");  break;
  case OP_DIFF:
    printf("!=");  break;
  case OP_LT:
    printf("<");   break;
  case OP_GT:
    printf(">");   break;
  case OP_LTE:
    printf("<=");  break;
  case OP_GTE:
    printf(">=");  break;
  case OP_SUM:
    printf("+");   break;
  case OP_SUB:
    printf("-");   break;
  case OP_MULT:
    printf("*");   break;
  case OP_DIV:
    printf("/");   break;
  default:
    printf("%%");
  }

  putchar('\n');
}

void printTab(int size)
{
  for(int i=0; i!=size; ++i)
    putchar(' ');
}
