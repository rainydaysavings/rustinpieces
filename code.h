#ifndef __IC_HEADER__
#define __IC_HEADER__

#include "ast.h"



typedef char* var;
typedef char* label;
typedef char* regist;



typedef enum { iCOPY, iADD, iSUB, iMULT, iDIV, iMOD, iAND, iOR, iGOTO, iLT, iLTE, iGT, iGTE, iEQ, iDIFF, iIF_F, iLABEL, iRD, iWR } instr_kind;



struct _InstrList
{
  struct _Instr *instr;
  struct _InstrList *next;
};



struct _Instr
{
  instr_kind kind;
  struct _Address* addr1;
  struct _Address* addr2;
  struct _Address* addr3;
};



struct _Address
{
  enum
  {
    ADDR_CONST,
    ADDR_REGIST
  } kind;

  union
  {
    int val;
    regist reg;
  } core;
};



struct _Pair
{
  struct _Address* first;
  struct _InstrList* second;
};



struct _RegList
{
  regist reg;
  var id;
  int reg_num;
  struct _RegList* next;
};



typedef struct _Address Address;
typedef struct _Instr Instr;
typedef struct _InstrList InstrList;
typedef struct _Pair Pair;
typedef struct _RegList RegList;



extern RegList* reg_list;
extern int reg_count, label_count;



InstrList* genIC(StmtList* ast);
InstrList* compile_attrib(Attrib* attrib);
Pair* compile_expr(Expr* expr);
InstrList* compile_if(If* _if, int make_exit, Address* exit_addr);
InstrList* compile_while(While* _while);
Pair* mk_pair(Address* first, InstrList* second);
InstrList* mk_instrList(Instr* instr, InstrList* next);
Instr* mk_instr(instr_kind kind, Address* addr1, Address* addr2, Address* addr3);
Address* mk_address_const(int val);
Address* mk_address_regist(regist reg);
instr_kind get_binop_instr(Binop op);
void printIC(InstrList* ic);
void print_operation(Instr* instr, char* op);
void print_address(Address* addr, int eol);
InstrList* list_concat(InstrList* lhs, InstrList* rhs);
InstrList* list_append(InstrList* list, Instr* instr);



label new_label();
regist reg_get(var id);
void reg_free();
regist reg_alloc();
RegList* mk_regList(regist reg, var id, RegList* next);
void reg_assoc(regist reg, var id);

#endif
