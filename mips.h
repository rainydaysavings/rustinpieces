#ifndef __H_MIPS__
#define __H_MIPS__

#include "symlist.h"
#include "code.h"

extern RegList* addr_list;
extern int next_reg, next_temp;

void printMIPS(InstrList* ic3);
void write_data();
void write_text(InstrList* ic3);
void write_loads();
regist mips_reg_get(regist addr);
regist mips_temp_get();

void write_copy(Instr* instr, int iff);
void write_label(Instr* instr);
void write_goto(Instr* instr);
void write_read(Instr* instr);
void write_write(Instr* instr);
void write_iff(Instr* instr);
void write_op(Instr* instr, int iff, char* op);

#endif
