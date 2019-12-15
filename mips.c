#include "mips.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

RegList* addr_list;
int next_reg = 0, next_temp = 0;

void printMIPS(InstrList* instructions)
{
  write_data();
  write_text(instructions);
}

void write_data()
{
  printf(".data\n");

  RegList* p = reg_list;
  while(p)
  {
    printf("%s: .word 0\n", p->id);
    p->reg_num = next_reg++;
    p = p->next;
  }
}

void write_text(InstrList* instructions)
{
  printf(".text\n");

  write_loads();

  InstrList* p = instructions;
  while(p)
  {
    int iff = p->next && p->next->instr->kind == nuIF_F;

    switch(p->instr->kind)
    {
    case nuADD:
      write_op(p->instr, iff, "add");   break;
    case nuSUB:
      write_op(p->instr, iff, "sub");   break;
    case nuMULT:
      write_op(p->instr, iff, "mul");   break;
    case nuDIV:
      write_op(p->instr, iff, "div");   break;
    case nuMOD:
      write_op(p->instr, iff, "mod");   break;
    case nuAND:
      write_op(p->instr, iff, "and");   break;
    case nuOR:
      write_op(p->instr, iff, "or");    break;
    case nuLT:
      write_op(p->instr, iff, "slt");   break;
    case nuLTE:
      write_op(p->instr, iff, "sle");   break;
    case nuGT:
      write_op(p->instr, iff, "sgt");   break;
    case nuGTE:
      write_op(p->instr, iff, "sge");   break;
    case nuEQ:
      write_op(p->instr, iff, "seq");   break;
    case nuDIFF:
      write_op(p->instr, iff, "sne");   break;
    case nuREAD:
      write_read(p->instr);             break;
    case nuWRITE:
      write_write(p->instr);            break;
    case nuCOPY:
      write_copy(p->instr, iff);        break;
    case nuIF_F:
      write_iff(p->instr);              break;
    case nuLABEL:
      write_label(p->instr);            break;
    case nuGOTO:
      write_goto(p->instr);             break;
    default:
      printf("Error.\n");
    }

    p = p->next;
  }
}

void write_loads()
{
  RegList* p = reg_list;
  while(p)
  {
    printf("la $s%d, %s\n", p->reg_num, p->id);
    p = p->next;
  }
}

regist mips_reg_get(regist addr)
{
  RegList* p = reg_list;
  while(p)
  {
    if(!strcmp(p->reg,addr))
      break;
    p = p->next;
  }

  if(!p)
    return mips_temp_get();

  regist new_reg = (regist)malloc(10*sizeof(char));
  sprintf(new_reg, "$s%d", p->reg_num);
  return new_reg;
}

regist mips_temp_get()
{
  regist new = (regist)malloc(10*sizeof(char));
  sprintf(new, "$t%d", next_temp++);
  if(next_temp>7) next_temp = 0;
  return new;
}

void write_copy(Instr* instr, int iff)
{
  if(instr->addr2->kind == ADDR_CONST)
    printf("li %s, %d\n", mips_reg_get(instr->addr1->core.reg), instr->addr2->core.val);
  else
    printf("move %s, %s\n", mips_reg_get(instr->addr1->core.reg), mips_reg_get(instr->addr2->core.reg));

  if(iff && next_temp)
    next_temp--;
}

void write_label(Instr* instr)
{
  printf("%s:\n", instr->addr1->core.reg);
}

void write_goto(Instr* instr)
{
  printf("j %s\n", instr->addr1->core.reg);
}

void write_read(Instr* instr)
{
  printf("li $v0, 5\n");
  printf("syscall\n");
  printf("add %s, $v0, $zero\n", mips_reg_get(instr->addr1->core.reg));
}

void write_write(Instr* instr)
{
  printf("li $v0, 1\n");
  printf("add $a0, %s, $zero\n", mips_reg_get(instr->addr1->core.reg));
  printf("syscall\n");
}

void write_iff(Instr* instr)
{
  printf("beqz %s, %s\n", mips_reg_get(instr->addr1->core.reg), instr->addr2->core.reg);
}

void write_op(Instr* instr, int iff, char* op)
{
  regist lhs = NULL;
  regist rhs = NULL;

  if(instr->addr2->kind == ADDR_CONST)
  {
    lhs = mips_temp_get();
    printf("li %s, %d\n", lhs, instr->addr2->core.val);
  }
  else
    lhs = mips_reg_get(instr->addr2->core.reg);

  if(instr->addr3->kind == ADDR_CONST)
  {
    rhs = mips_temp_get();
    printf("li %s, %d\n", rhs, instr->addr3->core.val);
  }
  else
    rhs = mips_reg_get(instr->addr3->core.reg);


  if(!strcmp(op,"div"))
  {
    printf("div %s, %s\n", lhs, rhs);
    printf("mflo %s\n", mips_reg_get(instr->addr1->core.reg));
    next_temp--;
  }
  else if(!strcmp(op,"mod"))
  {
    printf("div %s, %s\n", lhs, rhs);
    printf("mfhi %s\n", mips_reg_get(instr->addr1->core.reg));
    next_temp--;
  }
  else
    printf("%s %s, %s, %s\n", op, mips_reg_get(instr->addr1->core.reg), lhs, rhs);

  if(iff && next_temp)
    next_temp--;
}
