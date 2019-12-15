#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "parser.tab.h"



RegList* reg_list;
int reg_count=0, label_count=0;



InstrList* genIC(StmtList* ast)
{
  StmtList* p = ast;
  InstrList* ic = NULL;

  while(p)
  {
    InstrList* gen = NULL;

    switch(p->stmt->kind)
    {
    case STMT_ATTRIB:
      gen = compile_attrib(p->stmt->core.attrib);     break;
    case STMT_EXPR:
      gen = compile_expr(p->stmt->core.expr)->second; break;
    case STMT_BLOCK:
      gen = genIC(p->stmt->core.block);               break;
    case STMT_IF:
      gen = compile_if(p->stmt->core._if, 1, NULL);   break;
    case STMT_WHILE:
      gen = compile_while(p->stmt->core._while);          break;
    }

    ic = list_concat(ic,gen);
    p = p->next;
  }

  return ic;
}



InstrList* compile_attrib(Attrib* attrib)
{
  Pair* p = compile_expr(attrib->expr);
  regist r = reg_get(attrib->id);

  if(!p->second)
  {
    if(!r)
    {
      r = reg_alloc();
      reg_assoc(r, attrib->id);
    }

    Address* addr = mk_address_regist(r);
    p->second = mk_instrList( mk_instr(iCOPY,addr,p->first,NULL), NULL );
  }
  else
  {
    if(!r)
      reg_assoc(p->first->core.reg, attrib->id);
    else
    {
      reg_free();

      InstrList* n = p->second;
      while(n->next)
	n = n->next;
      n->instr->addr1->core.reg = r;
    }
  }
  return p->second;
}



Pair* compile_expr(Expr* expr)
{
  if(expr->kind == EXPR_PRIMITIVE)
    return mk_pair(mk_address_const(expr->core.val), NULL);
  else if(expr->kind == EXPR_VAR)
    return mk_pair(mk_address_regist(reg_get(expr->core.id)), NULL);

  Pair* pl = compile_expr(expr->core.op.l_expr);
  Pair* pr = compile_expr(expr->core.op.r_expr);

  Address* addr = NULL;
  if(pl->second) {      addr = pl->first; reg_free(); }
  else if(pr->second)   addr = pr->first;
  else                  addr = mk_address_regist(reg_alloc());

  Instr* instr = mk_instr(get_binop_instr(expr->core.op.binop), addr, pl->first, pr->first);
  InstrList* list = list_append(list_concat(pl->second,pr->second),instr);

  return mk_pair(addr,list);
}



InstrList* compile_if(If* _if, int make_exit, Address* exit_addr)
{
  InstrList* list = NULL;

  if(_if->attrib)
    list = compile_attrib(_if->attrib);

  Pair* p = compile_expr(_if->cond);
  if(!p->second)
  {
    Address* addr = mk_address_regist(reg_alloc());
    p->second = mk_instrList(mk_instr(iCOPY,addr,p->first,NULL), NULL);
    p->first = addr;
  }

  list = list_concat(list,p->second);

  Address* if_label = mk_address_regist(new_label());
  list = list_append(list, mk_instr(iIF_F, p->first, if_label, NULL));
  reg_free();

  list = list_concat(list, genIC(_if->block));

  if(exit_addr)
    list = list_append(list, mk_instr(iGOTO, exit_addr, NULL, NULL));
  else if(make_exit && _if->elseBlock)
  {
    exit_addr = mk_address_regist(new_label());
    list = list_append(list, mk_instr(iGOTO, exit_addr, NULL, NULL));
  }

  list = list_append(list, mk_instr(iLABEL, if_label, NULL, NULL));

  if(_if->elseBlock && !(_if->elseBlock->stmt->kind == STMT_ATTRIB && !_if->elseBlock->stmt->core.attrib))
  {
    if(_if->elseBlock->stmt->kind == STMT_IF)
      list = list_concat(list, compile_if(_if->elseBlock->stmt->core._if, 0, exit_addr));
    else
      list = list_concat(list, genIC(_if->elseBlock));
  }

  if(make_exit && exit_addr)
    list = list_append(list, mk_instr(iLABEL, exit_addr, NULL, NULL));

  return list;
}



InstrList* compile_while(While* _while)
{
  InstrList* list = NULL;

  Address* while_label = mk_address_regist(new_label());
  list = list_append(list, mk_instr(iLABEL, while_label, NULL, NULL));

  Address* exit_addr = NULL;
  if(_while->cond)
  {
    Pair* p = compile_expr(_while->cond);
    if(!p->second)
    {
      Address* addr = mk_address_regist(reg_alloc());
      p->second = mk_instrList(mk_instr(iCOPY, addr, p->first, NULL), NULL);
      p->first = addr;
    }

    list = list_concat(list,p->second);

    exit_addr = mk_address_regist(new_label());
    list = list_append(list, mk_instr(iIF_F, p->first, exit_addr, NULL));
    reg_free();
  }

  if(_while->block && !(_while->block->stmt->kind == STMT_ATTRIB && !_while->block->stmt->core.attrib))
    list = list_concat(list, genIC(_while->block));

  list = list_append(list, mk_instr(iGOTO,while_label,NULL,NULL));

  if(exit_addr)
    list = list_append(list, mk_instr(iLABEL, exit_addr, NULL, NULL));

  return list;
}



Pair* mk_pair(Address* first, InstrList* second)
{
  Pair* p = (Pair*)malloc(sizeof(Pair));
  p->first = first;
  p->second = second;
  return p;
}



Address* mk_address_const(int val)
{
  Address* a = (Address*)malloc(sizeof(Address));
  a->kind = ADDR_CONST;
  a->core.val = val;
  return a;
}



Address* mk_address_regist(regist reg)
{
  Address* a = (Address*)malloc(sizeof(Address));
  a->kind = ADDR_REGIST;
  a->core.reg = reg;
  return a;
}



Instr* mk_instr(instr_kind kind, Address* addr1, Address* addr2, Address* addr3)
{
  Instr* i = (Instr*)malloc(sizeof(Instr));
  i->kind = kind;
  i->addr1 = addr1;
  i->addr2 = addr2;
  i->addr3 = addr3;
  return i;
}



instr_kind get_binop_instr(Binop op)
{
  switch(op)
  {
  case OP_AND:
    return iAND;  break;
  case OP_OR:
    return iOR;   break;
  case OP_EQ:
    return iEQ;   break;
  case OP_DIFF:
    return iDIFF; break;
  case OP_LT:
    return iLT;   break;
  case OP_GT:
    return iGT;   break;
  case OP_LTE:
    return iLTE;  break;
  case OP_GTE:
    return iGTE;  break;
  case OP_SUM:
    return iADD;  break;
  case OP_SUB:
    return iSUB;  break;
  case OP_MULT:
    return iMULT; break;
  case OP_DIV:
    return iDIV;  break;
  default:
    return iMOD;
  }
}



InstrList* mk_instrList(Instr* instr, InstrList* next)
{
  InstrList* l = (InstrList*)malloc(sizeof(InstrList));
  l->instr = instr;
  l->next = next;
  return l;
}



InstrList* list_concat(InstrList* lhs, InstrList* rhs)
{
  if(!lhs && !rhs)
    return NULL;
  else if(!lhs)
    return rhs;
  else if(!rhs)
    return lhs;

  InstrList* p = lhs;

  while(p->next)
    p = p->next;

  p->next = rhs;
  return lhs;
}



InstrList* list_append(InstrList* list, Instr* instr)
{
  if(!list)
    return mk_instrList(instr,NULL);

  InstrList* p = list;
  while(p->next)
    p = p->next;

  p->next = mk_instrList(instr,NULL);
  return list;
}



void printIC(InstrList* ic)
{
  while(ic)
  {
    switch(ic->instr->kind)
    {
    case iADD:
      print_operation(ic->instr,"+");                   break;
    case iSUB:
      print_operation(ic->instr,"-");                   break;
    case iMULT:
      print_operation(ic->instr,"*");                   break;
    case iDIV:
      print_operation(ic->instr,"/");                   break;
    case iMOD:
      print_operation(ic->instr,"%");                   break;
    case iAND:
      print_operation(ic->instr,"&&");                  break;
    case iOR:
      print_operation(ic->instr,"||");                  break;
    case iLT:
      print_operation(ic->instr,"<");                   break;
    case iLTE:
      print_operation(ic->instr,"<=");                  break;
    case iGT:
      print_operation(ic->instr,">");                   break;
    case iGTE:
      print_operation(ic->instr,">=");                  break;
    case iEQ:
      print_operation(ic->instr,"==");                  break;
    case iDIFF:
      print_operation(ic->instr,"!=");                  break;
    case iRD:
      printf("READ %s\n", ic->instr->addr1->core.reg);  break;
    case iWR:
      printf("WRITE %s\n", ic->instr->addr1->core.reg); break;
    case iCOPY:
      printf("COPY %s ", ic->instr->addr1->core.reg);
      print_address(ic->instr->addr2,1);
      break;
    case iIF_F:
      printf("IF_FALSE %s GOTO %s\n", ic->instr->addr1->core.reg, ic->instr->addr2->core.reg);
      break;
    case iLABEL:
      printf("LABEL %s\n", ic->instr->addr1->core.reg);
      break;
    case iGOTO:
      printf("GOTO %s\n", ic->instr->addr1->core.reg);
      break;
    default:
      printf("Other stuff I cant deal with right now...\n");
    }
    ic = ic->next;
  }
}



void print_operation(Instr* instr, char* op)
{
  printf("%s = ", instr->addr1->core.reg);
  print_address(instr->addr2,0);
  printf(" %s ", op);
  print_address(instr->addr3,1);
}



void print_address(Address* addr, int eol)
{
  if(addr->kind == ADDR_CONST)
    printf("%d", addr->core.val);
  else
    printf("%s", addr->core.reg);

  if(eol) putchar('\n');
}



label new_label()
{
  label l = (label)malloc(10*sizeof(char));
  sprintf(l, "L%d", label_count++);
  return l;
}



regist reg_get(var id)
{
  RegList* p = reg_list;

  while(p)
  {
    if(!strcmp(id,p->id))
      return p->reg;
    p = p->next;
  }

  return NULL;
}



void reg_free()
{
  reg_count--;
}



regist reg_alloc()
{
  regist r = (regist)malloc(10*sizeof(char));
  sprintf(r, "R%d", reg_count++);
  return r;
}



RegList* mk_regList(regist reg, var id, RegList* next)
{
  RegList* r = (RegList*)malloc(sizeof(RegList));
  r->reg = reg;
  r->id = id;
  r->reg_num = -1;
  r->next = next;
  return r;
}



void reg_assoc(regist reg, var id)
{
  if(!reg_list)
  {
    reg_list = mk_regList(reg,id,NULL);
    return;
  }

  RegList* p = reg_list;
  while(p->next)
    p = p->next;
  p->next = mk_regList(reg,id,NULL);
}
