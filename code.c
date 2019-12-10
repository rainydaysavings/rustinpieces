#include "code.h"
#include <stdio.h>
#include <stdlib.h>
Instr mkInstr(Opkind op, Atom e1, Atom e2, Atom e3, Atom e4){
  Instr i;
  i.op = op;
  i.el1 = e1;
  i.el2 = e2;
  i.el3 = e3;
  i.el4 = e4;
  return i;
}
ListInstr* mkList(Instr i1,Instr i2){
    ListInstr* new = (ListInstr*)malloc(sizeof(ListInstr));
    new->i=i1;
    //new->next=mkList(i2 ,NULL);
    return new;
}
//função cria var
Atom mkVar(char *s){
  Atom a;
  a.kind = STRING;
  a.u.name = s;
  return a;
}

//funçao cria int
Atom mkInt(int x){
  Atom a;
  a.kind = INT;
  a.u.value = x;
  return a;
}

//cria elem vazio
Atom empty(){
  Atom a;
  a.kind = EMPTY;
  return a;
}
Instr getFirst(ListInstr l){
  return l.i;
}

ListInstr* nextInstr(ListInstr* l){
  return l->next;
}
ListInstr* append(ListInstr* l1, ListInstr* l2){
  l1->next=l2;
  return l1;
}
void printAtrib(Atom e1, Atom e2){
  printf("%s=",e1.u.name);
  if(e2.kind==INT) printf("%d",e2.u.value);
  if(e2.kind==STRING) printf("%s",e2.u.name);
  printf(";\n");
}
void printPlus(Atom e1,Atom e2, Atom e3){
  printf("%s = ",e1.u.name);
  if(e2.kind==INT && e3.kind==INT)
    printf("%d + %d;",e2.u.value,e3.u.value);
  else if(e2.kind==INT && e3.kind==STRING)
    printf("%d + %s;",e2.u.value,e3.u.name);
  else if(e2.kind==STRING && e3.kind==INT)
    printf("%s + %d;",e2.u.name,e3.u.value);
  else if(e2.kind==STRING && e3.kind==STRING)
    printf("%s + %s;",e2.u.name,e3.u.name);
}
void printMinus(Atom e1,Atom e2, Atom e3){
  printf("%s = ",e1.u.name);
  if(e2.kind==INT && e3.kind==INT)
    printf("%d - %d;",e2.u.value,e3.u.value);
  else if(e2.kind==INT && e3.kind==STRING)
    printf("%d - %s;",e2.u.value,e3.u.name);
  else if(e2.kind==STRING && e3.kind==INT)
    printf("%s - %d;",e2.u.name,e3.u.value);
  else if(e2.kind==STRING && e3.kind==STRING)
    printf("%s - %s;",e2.u.name,e3.u.name);
}
void printMult(Atom e1,Atom e2, Atom e3){
  printf("%s = ",e1.u.name);
  if(e2.kind==INT && e3.kind==INT)
    printf("%d * %d;",e2.u.value,e3.u.value);
  else if(e2.kind==INT && e3.kind==STRING)
    printf("%d * %s;",e2.u.value,e3.u.name);
  else if(e2.kind==STRING && e3.kind==INT)
    printf("%s * %d;",e2.u.name,e3.u.value);
  else if(e2.kind==STRING && e3.kind==STRING)
    printf("%s * %s;",e2.u.name,e3.u.name);
}
void printDiv(Atom e1,Atom e2, Atom e3){
  printf("%s = ",e1.u.name);
  if(e2.kind==INT && e3.kind==INT)
    printf("%d / %d;",e2.u.value,e3.u.value);
  else if(e2.kind==INT && e3.kind==STRING)
    printf("%d / %s;",e2.u.value,e3.u.name);
  else if(e2.kind==STRING && e3.kind==INT)
    printf("%s / %d;",e2.u.name,e3.u.value);
  else if(e2.kind==STRING && e3.kind==STRING)
    printf("%s / %s;",e2.u.name,e3.u.name);
}

void printLabel(Atom e1){
  printf("LABEL %s",e1.u.name);
}
void printGoto(Atom e1){
  printf("GOTO %s",e1.u.name);
}
void printIfE(Atom e1,Atom e2){
  printf("if(");
  if(e1.kind==INT && e2.kind==INT)
    printf("%d == %d;",e1.u.value,e2.u.value);
  else if(e1.kind==INT && e2.kind==STRING)
    printf("%d == %s;",e1.u.value,e2.u.name);
  else if(e1.kind==STRING && e2.kind==INT)
    printf("%s == %d;",e1.u.name,e2.u.value);
  else if(e1.kind==STRING && e2.kind==STRING)
    printf("%s == %s;",e1.u.name,e2.u.name);
  printf(");\n");
}
void printIfG(Atom e1,Atom e2){
  printf("if(");
  if(e1.kind==INT && e2.kind==INT)
    printf("%d > %d;",e1.u.value,e2.u.value);
  else if(e1.kind==INT && e2.kind==STRING)
    printf("%d > %s;",e1.u.value,e2.u.name);
  else if(e1.kind==STRING && e2.kind==INT)
    printf("%s > %d;",e1.u.name,e2.u.value);
  else if(e1.kind==STRING && e2.kind==STRING)
    printf("%s > %s;",e1.u.name,e2.u.name);
  printf(");\n");
}
void printIfGE(Atom e1,Atom e2){
  printf("if(");
  if(e1.kind==INT && e2.kind==INT)
    printf("%d >= %d;",e1.u.value,e2.u.value);
  else if(e1.kind==INT && e2.kind==STRING)
    printf("%d >= %s;",e1.u.value,e2.u.name);
  else if(e1.kind==STRING && e2.kind==INT)
    printf("%s >= %d;",e1.u.name,e2.u.value);
  else if(e1.kind==STRING && e2.kind==STRING)
    printf("%s >= %s;",e1.u.name,e2.u.name);
  printf(");\n");
}
void printIfL(Atom e1,Atom e2){
  printf("if(");
  if(e1.kind==INT && e2.kind==INT)
    printf("%d < %d;",e1.u.value,e2.u.value);
  else if(e1.kind==INT && e2.kind==STRING)
    printf("%d < %s;",e1.u.value,e2.u.name);
  else if(e1.kind==STRING && e2.kind==INT)
    printf("%s < %d;",e1.u.name,e2.u.value);
  else if(e1.kind==STRING && e2.kind==STRING)
    printf("%s < %s;",e1.u.name,e2.u.name);
  printf(");\n");
}
void printIfLE(Atom e1,Atom e2){
  printf("if(");
  if(e1.kind==INT && e2.kind==INT)
    printf("%d <= %d;",e1.u.value,e2.u.value);
  else if(e1.kind==INT && e2.kind==STRING)
    printf("%d <= %s;",e1.u.value,e2.u.name);
  else if(e1.kind==STRING && e2.kind==INT)
    printf("%s <= %d;",e1.u.name,e2.u.value);
  else if(e1.kind==STRING && e2.kind==STRING)
    printf("%s <= %s;",e1.u.name,e2.u.name);
  printf(");\n");
}
void printIfDif(Atom e1,Atom e2){
  printf("if(");
  if(e1.kind==INT && e2.kind==INT)
    printf("%d != %d;",e1.u.value,e2.u.value);
  else if(e1.kind==INT && e2.kind==STRING)
    printf("%d != %s;",e1.u.value,e2.u.name);
  else if(e1.kind==STRING && e2.kind==INT)
    printf("%s != %d;",e1.u.name,e2.u.value);
  else if(e1.kind==STRING && e2.kind==STRING)
    printf("%s != %s;",e1.u.name,e2.u.name);
  printf(");\n");
}

void printInstr(Instr i){
  switch(i.op){
    case ATRIB:
      printAtrib(i.el1,i.el2);break;
    case PLUS:
      printPlus(i.el1,i.el2,i.el3);break;
    case MINUS:
      printMinus(i.el1,i.el2,i.el3);break;
    case MULT:
      printMult(i.el1,i.el2,i.el3);break;
    case DIV:
      printDiv(i.el1,i.el2,i.el3);break;
    case LABEL:
      printLabel(i.el1);break;
    case GOTO:
      printGoto(i.el1); break;
    case IFE:
      printIfE(i.el1,i.el2);break;
    case IFG:
      printIfG(i.el1,i.el2);break;
    case IFGE:
      printIfGE(i.el1,i.el2);break;
    case IFL:
      printIfL(i.el1,i.el2);break;
    case IFLE:
      printIfLE(i.el1,i.el2);break;
    case IFDIF:
      printIfDif(i.el1,i.el2);break;
  }
}


void printInstrList(ListInstr* l){
  printInstr(l->i);
  printInstrList(l->next);
}

//prof exemplo
int counter=0;char buffer[20];
char *newVar(){

  sprintf(buffer,"t %d",counter);
  return buffer;
}
Opkind comp(Opkind op){
  return PLUS;
}
ListInstr *compileExp(Expr* expr, char *r){
  ListInstr *l1 = (ListInstr*)malloc(sizeof(ListInstr));
  ListInstr *l2 = (ListInstr*)malloc(sizeof(ListInstr));
  ListInstr *l3 = (ListInstr*)malloc(sizeof(ListInstr));
  ListInstr *l4 = (ListInstr*)malloc(sizeof(ListInstr));
  char *r1;
  char *r2;
  switch(expr->kind){
    case NUM:

    case VAR:

    case ExprOp:
    r1=newVar();
    l1=compileExp(expr->left,r1);

    r2=newVar();
    l2=compileExp(expr->right,r2);
    l3=append(l1,l2);
  }
  l4=append(l3,mkList(mkInstr(comp(expr->op),mkVar(r),mkVar(r1),mkVar(r2),empty()),NULL));
  return l4;
}
/*
ListInstr *compileExp(Expr expr, char *r){
  ListInstr *code = (ListInstr*)malloc(sizeof(ListInstr));
  switch(expr.kind){
    case INT:
      code=mkList(mkInstr(ATRIB,r, expr.u.value,empty(),empty()),NULL);
      return code;
    case STRING:
      code=mkList(mkInstr(IFE,r, expr.u.name,empty(),empty()),NULL);
      return code;
    case EMPTY:
      code=mkList(NULL,NULL);
      return code;
  }
}

ListInstr compileCmd(Cmd c){
  switch(c->kind){
    case WHILE:
      inicio=newLabel();
      l_true=newLabel();
      l-false=newLabel();
      cond=compileBool(c->condition,l-true,l_false);
      body=compileCmd(c->comando);
      code=[label inicio]++cond++[label l_true]++body++[GOTO inicio,LABEL l_false];
  }
}*/
//ate aqui
int main(){}
