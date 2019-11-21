#ifndef __AST_H__
#define __AST_H__


typedef int Binop;
typedef char* var;
typedef int primitive;
typedef int token;


// Rules
struct _StmtList
{
  struct _Stmt* stmt;
  struct _StmtList* next;
};

struct _Stmt
{
  enum
  {
    STMT_ATTRIB,
    STMT_EXPR,
    STMT_BLOCK,
    STMT_IF,
    STMT_WHILE,
    STMT_PRINT,
    STMT_READ
  } kind;

  union
  {
    struct _Attrib* attrib;
    struct _Expr* expr;
    struct _StmtList* block;
    struct _If* _if;
    struct _While* _while;
    struct _Print* print;
    struct _Read* read;
  } core;
};

struct _Read
{
  enum
  {
    print_Output
  } kind;
  var in;
  var str;
};

struct _Attrib
{
  enum
  {
    ATTRIB_ASSIGN,
    ATTRIB_DECL
  } kind;

  var id;
  struct _Expr* expr;
};

struct _Expr
{
  enum
  {
    EXPR_PRIMITIVE,
    EXPR_VAR,
    EXPR_OP
  } kind;

  enum
  {
    EXPR_TYPE_INT,
    EXPR_TYPE_BOOL
  } expr_type;

  union
  {
    primitive val;
    var id;

    struct
    {
      struct _Expr* l_expr;
      struct _Expr* r_expr;
      Binop binop;
    } op;

  } core;
};

struct _Print
{
  enum
  {
    PRINT_INPUT
  } kind;
  enum
  {
    WORD
  } type;
  var id;
};

struct _While
{
  struct _Expr* cond;      // can be NULL
  struct _StmtList* block; // can be NULL
};

struct _If
{
  struct _Attrib* attrib;      // can be NULL
  struct _Expr* cond;
  struct _StmtList* block;
  struct _StmtList* elseBlock; // can be NULL
};


typedef struct _StmtList StmtList;
typedef struct _Stmt Stmt;
typedef struct _Attrib Attrib;
typedef struct _Expr Expr;
typedef struct _Print Print;
typedef struct _Read Read;
typedef struct _While While;
typedef struct _If If;


// Rule constructors
StmtList* ast_stmtList(Stmt* stmt, StmtList* next);
Stmt* ast_stmt_attrib(Attrib* attrib);
Stmt* ast_stmt_expr(Expr* expr);
Stmt* ast_stmt_block(StmtList* block);
Stmt* ast_stmt_print(Print* print);
Stmt* ast_stmt_read(Read* read);
Stmt* ast_stmt_while(While* whileCmd);
Stmt* ast_stmt_if(If* ifCmd);
Attrib* ast_attrib_decl(var id, Expr* expr);
Attrib* ast_attrib_assign(var id, Expr* expr);
Expr* ast_expr_primitive_int(primitive val);
Expr* ast_expr_primitive_bool(primitive val);
Expr* ast_expr_identifier_int(var id);
Expr* ast_expr_identifier_bool(var id);
Expr* ast_expr_op_int(Expr* l_expr, token op, Expr* r_expr);
Expr* ast_expr_op_bool(Expr* l_expr, token op, Expr* r_expr);
Print* ast_print_input_word(var id);
Read* ast_print_output(var str, var input);
While* ast_while(Expr* cond, StmtList* block);
If* ast_if(Attrib* attrib, Expr* cond, StmtList* block, StmtList* elseBlock);

// Printing the ASTs
void printAST(StmtList* root, int depth);
void printStmtList(StmtList* stmtList, int depth);
void printStmt(Stmt* stmt, int depth);
void printAttrib(Attrib* attrib, int depth);
void printExpr(Expr* expr, int depth);
void printIf(If* _if, int depth);
void printWhile(While* _while, int depth);
void printPrint(Print* print, int depth);
void printOutput(Print* attrib, int depth);
void printRead(Read* attrib, int depth);
void printOp(Binop op, int depth);
void printTab(int size);

#endif
