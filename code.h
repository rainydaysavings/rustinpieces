typedef enum{ATRIB,PLUS,MINUS,DIV,MULT,LABEL,GOTO,IFE,IFG,IFGE,IFL,IFLE,IFDIF} Opkind;
typedef enum{STRING, INT, EMPTY} AtomKind;
typedef enum{VAR, NUM, ExprOp} ExprKind;

typedef struct{
  AtomKind kind;
  union{
    int value;
    char *name;
  }u;
}Atom;

typedef struct{
  Opkind op;
  Atom el1, el2, el3, el4;
}Instr;

typedef struct list{
  Instr i;
  struct list *next;
}ListInstr;

typedef struct _Expr{
  ExprKind kind;
  Opkind op;
  //struct Expr *e;
  struct _Expr *left;
  struct _Expr *right;
}Expr;

Instr mkInstr(Opkind op, Atom e1, Atom e2, Atom e3, Atom e4);
ListInstr* mkList(Instr i1, Instr i2);
Atom mkVar(char *s);
Atom mkInt(int x);
Atom empty();
Instr getFirst(ListInstr l);
ListInstr* nextInstr(ListInstr* l);
ListInstr* append(ListInstr* l1, ListInstr* l2);
void printInstr(Instr i);
void printInstrList(ListInstr* l);
ListInstr *compileExp(Expr* expr, char *r);

//print functions
void printAtrib(Atom e1,Atom e2);
void printPlus(Atom e1,Atom e2, Atom e3);
void printMinus(Atom e1,Atom e2, Atom e3);
void printMult(Atom e1,Atom e2, Atom e3);
void printDiv(Atom e1,Atom e2, Atom e3);
void printLabel(Atom e1);
void printGoto(Atom e1);
void printIfE(Atom e1,Atom e2);
void printIfG(Atom e1,Atom e2);
void printIfGE(Atom e1,Atom e2);
void printIfL(Atom e1,Atom e2);
void printIfLE(Atom e1,Atom e2);
void printIfDif(Atom e1,Atom e2);
