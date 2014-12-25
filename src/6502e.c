#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <6502e.h>

u16 regPC, regSP = 0x1FF;
u8 regA, regX, regY, regFlags;
u8 cpuMem[65536];

const u8 opcode_sztable[256] = {
  0, 2, 0, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3,
  0, 2, 0, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3,
  0, 2, 0, 2, 2, 2, 2, 2, 1, 2, 1, 2, 0, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3,
  0, 2, 0, 2, 2, 2, 2, 2, 1, 2, 1, 2, 0, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3,
  2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 0, 0, 3, 3, 3, 3,
  2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3,
  2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3,
  2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 3, 3, 3, 3,
  2, 2, 0, 2, 2, 2, 2, 2, 1, 3, 1, 3, 3, 3, 3, 3,
};

const u8 opcode_cycletable[256] = {
  0, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
  3, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  6, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
  3, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
  3, 6, 0, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
  2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
  2, 5, 0, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
  2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
  3, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
};

const opfunc opcode_table[256] = {
  opfunc_0x0, opfunc_0x1, 
  opfunc_0x2, opfunc_0x3, 
  opfunc_0x4, opfunc_0x5, 
  opfunc_0x6, opfunc_0x7, 
  opfunc_0x8, opfunc_0x9, 
  opfunc_0xA, opfunc_0xB, 
  opfunc_0xC, opfunc_0xD, 
  opfunc_0xE, opfunc_0xF, 
  opfunc_0x10, opfunc_0x11, 
  opfunc_0x12, opfunc_0x13, 
  opfunc_0x14, opfunc_0x15, 
  opfunc_0x16, opfunc_0x17, 
  opfunc_0x18, opfunc_0x19, 
  opfunc_0x1A, opfunc_0x1B, 
  opfunc_0x1C, opfunc_0x1D, 
  opfunc_0x1E, opfunc_0x1F, 
  opfunc_0x20, opfunc_0x21, 
  opfunc_0x22, opfunc_0x23, 
  opfunc_0x24, opfunc_0x25, 
  opfunc_0x26, opfunc_0x27, 
  opfunc_0x28, opfunc_0x29, 
  opfunc_0x2A, opfunc_0x2B, 
  opfunc_0x2C, opfunc_0x2D, 
  opfunc_0x2E, opfunc_0x2F, 
  opfunc_0x30, opfunc_0x31, 
  opfunc_0x32, opfunc_0x33, 
  opfunc_0x34, opfunc_0x35, 
  opfunc_0x36, opfunc_0x37, 
  opfunc_0x38, opfunc_0x39, 
  opfunc_0x3A, opfunc_0x3B, 
  opfunc_0x3C, opfunc_0x3D, 
  opfunc_0x3E, opfunc_0x3F, 
  opfunc_0x40, opfunc_0x41, 
  opfunc_0x42, opfunc_0x43, 
  opfunc_0x44, opfunc_0x45, 
  opfunc_0x46, opfunc_0x47, 
  opfunc_0x48, opfunc_0x49, 
  opfunc_0x4A, opfunc_0x4B, 
  opfunc_0x4C, opfunc_0x4D, 
  opfunc_0x4E, opfunc_0x4F, 
  opfunc_0x50, opfunc_0x51, 
  opfunc_0x52, opfunc_0x53, 
  opfunc_0x54, opfunc_0x55, 
  opfunc_0x56, opfunc_0x57, 
  opfunc_0x58, opfunc_0x59, 
  opfunc_0x5A, opfunc_0x5B, 
  opfunc_0x5C, opfunc_0x5D, 
  opfunc_0x5E, opfunc_0x5F, 
  opfunc_0x60, opfunc_0x61, 
  opfunc_0x62, opfunc_0x63, 
  opfunc_0x64, opfunc_0x65, 
  opfunc_0x66, opfunc_0x67, 
  opfunc_0x68, opfunc_0x69, 
  opfunc_0x6A, opfunc_0x6B, 
  opfunc_0x6C, opfunc_0x6D, 
  opfunc_0x6E, opfunc_0x6F, 
  opfunc_0x70, opfunc_0x71, 
  opfunc_0x72, opfunc_0x73, 
  opfunc_0x74, opfunc_0x75, 
  opfunc_0x76, opfunc_0x77, 
  opfunc_0x78, opfunc_0x79, 
  opfunc_0x7A, opfunc_0x7B, 
  opfunc_0x7C, opfunc_0x7D, 
  opfunc_0x7E, opfunc_0x7F, 
  opfunc_0x80, opfunc_0x81, 
  opfunc_0x82, opfunc_0x83, 
  opfunc_0x84, opfunc_0x85, 
  opfunc_0x86, opfunc_0x87, 
  opfunc_0x88, opfunc_0x89, 
  opfunc_0x8A, opfunc_0x8B, 
  opfunc_0x8C, opfunc_0x8D, 
  opfunc_0x8E, opfunc_0x8F, 
  opfunc_0x90, opfunc_0x91, 
  opfunc_0x92, opfunc_0x93, 
  opfunc_0x94, opfunc_0x95, 
  opfunc_0x96, opfunc_0x97, 
  opfunc_0x98, opfunc_0x99, 
  opfunc_0x9A, opfunc_0x9B, 
  opfunc_0x9C, opfunc_0x9D, 
  opfunc_0x9E, opfunc_0x9F, 
  opfunc_0xA0, opfunc_0xA1, 
  opfunc_0xA2, opfunc_0xA3, 
  opfunc_0xA4, opfunc_0xA5, 
  opfunc_0xA6, opfunc_0xA7, 
  opfunc_0xA8, opfunc_0xA9, 
  opfunc_0xAA, opfunc_0xAB, 
  opfunc_0xAC, opfunc_0xAD, 
  opfunc_0xAE, opfunc_0xAF, 
  opfunc_0xB0, opfunc_0xB1, 
  opfunc_0xB2, opfunc_0xB3, 
  opfunc_0xB4, opfunc_0xB5, 
  opfunc_0xB6, opfunc_0xB7, 
  opfunc_0xB8, opfunc_0xB9, 
  opfunc_0xBA, opfunc_0xBB, 
  opfunc_0xBC, opfunc_0xBD, 
  opfunc_0xBE, opfunc_0xBF, 
  opfunc_0xC0, opfunc_0xC1, 
  opfunc_0xC2, opfunc_0xC3, 
  opfunc_0xC4, opfunc_0xC5, 
  opfunc_0xC6, opfunc_0xC7, 
  opfunc_0xC8, opfunc_0xC9, 
  opfunc_0xCA, opfunc_0xCB, 
  opfunc_0xCC, opfunc_0xCD, 
  opfunc_0xCE, opfunc_0xCF, 
  opfunc_0xD0, opfunc_0xD1, 
  opfunc_0xD2, opfunc_0xD3, 
  opfunc_0xD4, opfunc_0xD5, 
  opfunc_0xD6, opfunc_0xD7, 
  opfunc_0xD8, opfunc_0xD9, 
  opfunc_0xDA, opfunc_0xDB, 
  opfunc_0xDC, opfunc_0xDD, 
  opfunc_0xDE, opfunc_0xDF, 
  opfunc_0xE0, opfunc_0xE1, 
  opfunc_0xE2, opfunc_0xE3, 
  opfunc_0xE4, opfunc_0xE5, 
  opfunc_0xE6, opfunc_0xE7, 
  opfunc_0xE8, opfunc_0xE9, 
  opfunc_0xEA, opfunc_0xEB, 
  opfunc_0xEC, opfunc_0xED, 
  opfunc_0xEE, opfunc_0xEF, 
  opfunc_0xF0, opfunc_0xF1, 
  opfunc_0xF2, opfunc_0xF3, 
  opfunc_0xF4, opfunc_0xF5, 
  opfunc_0xF6, opfunc_0xF7, 
  opfunc_0xF8, opfunc_0xF9, 
  opfunc_0xFA, opfunc_0xFB, 
  opfunc_0xFC, opfunc_0xFD, 
  opfunc_0xFE, opfunc_0xFF, 
};

void cpu6502_step(){
  u8 op = cpuMem[regPC];
  opfunc f = opcode_table[op];
  f();
  regPC += opcode_sztable[op];
  regSP &= 0x01FF;
}

void cpu6502_print(){
  printf("PC: %X SP: %X\nA: %X X: %X Y: %X\nFlags: ", regPC, regSP, regA, regX, regY);
  printf("%u%u%u%u%u%u%u\n", regFlags & CARRY_MASK ? 1 : 0, regFlags & ZERO_MASK ? 1 : 0, regFlags & INTER_MASK ? 1 : 0, regFlags & DEC_MASK ? 1 : 0, regFlags & BRK_MASK ? 1 : 0, regFlags & OVER_MASK ? 1 : 0, regFlags & NEG_MASK ? 1 : 0);
}

void cpu6502_load(u16 loc, u8 *code, unsigned length){
  memcpy(cpuMem+loc, code, length);
}

void cpu6502_loadfile(const char *fln, u16 loc){
  FILE *fp = fopen(fln, "rb");
  unsigned len = 0;
  u8 *p = NULL;
  
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  p = (u8*)malloc(len);
  fread(p, 1, len, fp);
  cpu6502_load(loc, p, len);
  free(p);
  fclose(fp);
}

void cpu6502_setPC(u16 val){
  regPC = val;
}

u8 *cpu6502_getMemPtr(){
  return cpuMem;
}

/* Get immediate 8 bit constant */
u8 imm(){
  return cpuMem[regPC+1];
}

/* Get a ptr to byte at address in zero page */
u8 *zp(){
  return &cpuMem[cpuMem[regPC+1]];
}

u8 *zpx(){
  return &cpuMem[(u8)(cpuMem[regPC+1]+regX)];
}

u8 *zpy(){
  return &cpuMem[(u8)(cpuMem[regPC+1]+regY)];
}

u8 *abs_(){
  return &cpuMem[*(u16*)&cpuMem[regPC+1]];
}

u8 *absx(){
  return &cpuMem[(u16)(regX+*(u16*)&cpuMem[regPC+1])];
}

u8 *absy(){
  return &cpuMem[(u16)(regY+*(u16*)&cpuMem[regPC+1])];
}

u8 *iix(){
  return &cpuMem[*(u16*)&cpuMem[(u8)(regX+cpuMem[regPC+1])]];
}

u8 *iiy(){
  return &cpuMem[(u16)(regY+*(u16*)&cpuMem[cpuMem[regPC+1]])];
}

void adc(u8 val){
  int r = regA + val + (CARRY_SET ? 1 : 0);
  regFlags &= ~(OVER_MASK | CARRY_MASK | NEG_MASK | ZERO_MASK);
  if(r > 127 || r < -128){
    regFlags |= OVER_MASK;
  }
  if(((unsigned)r) > 255){
    regFlags |= CARRY_MASK;
  }
  if(r & 0x80){
    regFlags |= NEG_MASK;
  }
  if(r == 0){
    regFlags |= ZERO_MASK;
  }
  regA = (u8)r;
}

void and(u8 val){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  regA &= val;
  if(regA == 0){
    regFlags |= ZERO_MASK;
  }
  if(regA & 0x80){
    regFlags |= NEG_MASK;
  }
}

void asl(u8 *target){
  regFlags &= ~(CARRY_MASK | ZERO_MASK | NEG_MASK);
  regFlags |= *target & CARRY_MASK;

  *target = *target << 1;
  if(*target == 0){
    regFlags |= ZERO_MASK;
  }
  if(*target & 0x80){
    regFlags |= NEG_MASK;
  }
}

void bxx(int cond){
  if(cond){
    regPC += (s8)cpuMem[regPC+1];
  }
}

void bit(u8 val){
  regFlags |= ~(ZERO_MASK | OVER_MASK | NEG_MASK);
  if((val & regA) == 0){
    regFlags |= ZERO_MASK;
  }
  regFlags |= (val & (1 << 5)) ? OVER_MASK : 0;
  regFlags |= (val & (1 << 6)) ? NEG_MASK : 0;
}

void clx(u8 val){
  regFlags &= ~val;
}

void sex(u8 val){
  regFlags |= val;
}

void cmp(u8 reg, u8 mem){
  regFlags &= ~(CARRY_MASK | ZERO_MASK | NEG_MASK);
  if(reg >= mem){
    regFlags |= CARRY_MASK;
  }
  if(reg == mem){
    regFlags |= ZERO_MASK;
  }
  if((reg - mem) & 0x80){
    regFlags |= NEG_MASK;
  }
}

void dec(u8 *target){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  (*target)--;
  if(*target == 0){
    regFlags |= ZERO_MASK;
  }
  if(*target & 0x80){
    regFlags |= NEG_MASK;
  }
}

void eor(u8 val){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  regA ^= val;
  if(regA == 0){
    regFlags |= ZERO_MASK;
  }
  if(regA & 0x80){
    regFlags |= NEG_MASK;
  }
}

void inc(u8 *target){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  (*target)++;
  if(*target == 0){
    regFlags |= ZERO_MASK;
  }
  if(*target & 0x80){
    regFlags |= NEG_MASK;
  }
}

void ld(u8 *dst, u8 val){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  *dst = val;
  if(*dst == 0){
    regFlags |= ZERO_MASK;
  }
  if(*dst & 0x80){
    regFlags |= NEG_MASK;
  }
}

void lsr(u8 *target){
  regFlags &= ~(CARRY_MASK | ZERO_MASK | NEG_MASK);
  regFlags |= (*target & 1) ? CARRY_MASK : 0;
  *target = *target >> 1;
  if(*target == 0){
    regFlags |= ZERO_MASK;
  }
  if(*target & 0x80){
    regFlags |= NEG_MASK;
  }
}

void ora(u8 val){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  regA |= val;
  if(regA == 0){
    regFlags |= ZERO_MASK;
  }
  if(regA & 0x80){
    regFlags |= NEG_MASK;
  }
}

void rol(u8 *target){
  u8 b7 = (*target & 0x80) ? 1 : 0;
  u8 c = (regFlags & CARRY_MASK) ? 1 : 0;
  regFlags &= ~(CARRY_MASK | ZERO_MASK | NEG_MASK);
  *target = *target << 1;
  *target |= c;
  regFlags |= (b7) ? CARRY_MASK : 0;
  if(*target == 0){
    regFlags |= ZERO_MASK;
  }
  if(*target & 0x80){
    regFlags |= NEG_MASK;
  }
}

void ror(u8 *target){
  u8 b0 = *target & 1;
  u8 c = (regFlags & CARRY_MASK) ? 1 : 0;
  regFlags &= ~(CARRY_MASK | ZERO_MASK | NEG_MASK);
  *target = *target >> 1;
  *target |= c ? 0x80 : 0;
  regFlags |= (b0) ? CARRY_MASK : 0;
  if(*target == 0){
    regFlags |= ZERO_MASK;
  }
  if(*target & 0x80){
    regFlags |= NEG_MASK;
  }
}

void sbc(u8 val){
  int r = regA + ~val + (CARRY_SET ? 1 : 0);
  regFlags &= ~(OVER_MASK | CARRY_MASK | NEG_MASK | ZERO_MASK);
  if(r > 127 || r < -128){
    regFlags |= OVER_MASK;
  }
  if(!(((unsigned)r) > 255)){
    regFlags |= CARRY_MASK;
  }
  if(r & 0x80){
    regFlags |= NEG_MASK;
  }
  if(r == 0){
    regFlags |= ZERO_MASK;
  }
  regA = (u8)r;
}

void st(u8 src, u8 *dst){
  *dst = src;
}

void trs(u8 src, u8 *dst){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  *dst = src;
  if(*dst == 0){
    regFlags |= ZERO_MASK;
  }
  if(*dst & 0x80){
    regFlags |= NEG_MASK;
  }
}

void opfunc_0x0(){

}

void opfunc_0x1(){
  ora(*iix());
}

void opfunc_0x2(){

}

void opfunc_0x3(){

}

void opfunc_0x4(){

}

void opfunc_0x5(){
  ora(*zp());
}

void opfunc_0x6(){
  asl(zp());
}

void opfunc_0x7(){

}

void opfunc_0x8(){
  cpuMem[regSP--] = regFlags;
}

void opfunc_0x9(){
  ora(imm());
}

void opfunc_0xA(){
  asl(&regA);
}

void opfunc_0xB(){

}

void opfunc_0xC(){

}

void opfunc_0xD(){
  ora(*abs_());
}

void opfunc_0xE(){
  asl(abs_());
}

void opfunc_0xF(){

}

void opfunc_0x10(){
  bxx(NEG_CLEAR);
}

void opfunc_0x11(){
  ora(*iiy());
}

void opfunc_0x12(){

}

void opfunc_0x13(){

}

void opfunc_0x14(){

}

void opfunc_0x15(){
  ora(*zpx());
}

void opfunc_0x16(){
  asl(zpx());
}

void opfunc_0x17(){

}

void opfunc_0x18(){
  clx(CARRY_MASK);
}

void opfunc_0x19(){
  ora(*absy());
}

void opfunc_0x1A(){

}

void opfunc_0x1B(){

}

void opfunc_0x1C(){

}

void opfunc_0x1D(){
  ora(*absx());
}

void opfunc_0x1E(){
  asl(absx());
}

void opfunc_0x1F(){

}

void opfunc_0x20(){
  regSP -= 2;
  *(u16*)&cpuMem[regSP] = regPC + 3;
  regPC = *(u16*)&cpuMem[regPC+1];
}

void opfunc_0x21(){
  and(*iix());
}

void opfunc_0x22(){

}

void opfunc_0x23(){

}

void opfunc_0x24(){
  bit(*zp());
}

void opfunc_0x25(){
  and(*zp());
}

void opfunc_0x26(){
  rol(zp());
}

void opfunc_0x27(){

}

void opfunc_0x28(){
  regFlags = cpuMem[++regSP];
}

void opfunc_0x29(){
  and(imm());
}

void opfunc_0x2A(){
  rol(&regA);
}

void opfunc_0x2B(){
  
}

void opfunc_0x2C(){
  bit(*abs_());
}

void opfunc_0x2D(){
  and(*abs_());
}

void opfunc_0x2E(){
  rol(abs_());
}

void opfunc_0x2F(){

}

void opfunc_0x30(){
  bxx(NEG_SET);
}

void opfunc_0x31(){
  and(*iiy());
}

void opfunc_0x32(){

}

void opfunc_0x33(){

}

void opfunc_0x34(){

}

void opfunc_0x35(){
  and(*zpx());
}

void opfunc_0x36(){
  rol(zpx());
}

void opfunc_0x37(){

}

void opfunc_0x38(){
  sex(CARRY_MASK);
}

void opfunc_0x39(){
  and(*absy());
}

void opfunc_0x3A(){

}

void opfunc_0x3B(){

}

void opfunc_0x3C(){

}

void opfunc_0x3D(){
  and(*absx());
}

void opfunc_0x3E(){
  rol(absx());
}

void opfunc_0x3F(){

}

void opfunc_0x40(){
  regFlags = cpuMem[++regSP];
  regSP += 2;
  regPC = *(u16*)&cpuMem[regSP];
}

void opfunc_0x41(){
  eor(*iix());
}

void opfunc_0x42(){

}

void opfunc_0x43(){

}

void opfunc_0x44(){

}

void opfunc_0x45(){
  eor(*zp());
}

void opfunc_0x46(){
  lsr(zp());
}

void opfunc_0x47(){

}

void opfunc_0x48(){
  cpuMem[regSP--] = regA;
}

void opfunc_0x49(){
  eor(imm());
}

void opfunc_0x4A(){
  lsr(&regA);
}

void opfunc_0x4B(){

}

void opfunc_0x4C(){
  regPC = *(u16*)&cpuMem[regPC+1];
}

void opfunc_0x4D(){
  eor(*abs_());
}

void opfunc_0x4E(){
  lsr(abs_());
}

void opfunc_0x4F(){

}

void opfunc_0x50(){
  bxx(OVER_CLEAR);
}

void opfunc_0x51(){
  eor(*iiy());
}

void opfunc_0x52(){

}

void opfunc_0x53(){

}

void opfunc_0x54(){

}

void opfunc_0x55(){
  eor(*zpx());
}

void opfunc_0x56(){
  lsr(zpx());
}

void opfunc_0x57(){

}

void opfunc_0x58(){
  clx(INTER_MASK);
}

void opfunc_0x59(){
  eor(*absy());
}

void opfunc_0x5A(){

}

void opfunc_0x5B(){

}

void opfunc_0x5C(){

}

void opfunc_0x5D(){
  eor(*absx());
}

void opfunc_0x5E(){
  lsr(absx());
}

void opfunc_0x5F(){

}

void opfunc_0x60(){
  regPC = *(u16*)&cpuMem[regSP];
  regSP += 2;
}

void opfunc_0x61(){

}

void opfunc_0x62(){

}

void opfunc_0x63(){

}

void opfunc_0x64(){

}

void opfunc_0x65(){
  adc(*zp());
}

void opfunc_0x66(){
  ror(zp());
}

void opfunc_0x67(){

}

void opfunc_0x68(){
  regFlags &= ~(ZERO_MASK | NEG_MASK);
  regA = cpuMem[++regSP];
  if(regA == 0){
    regFlags |= ZERO_MASK;
  }
  if(regA & 0x80){
    regFlags |= NEG_MASK;
  }
}

void opfunc_0x69(){
  adc(imm());
}

void opfunc_0x6A(){
  ror(&regA);
}

void opfunc_0x6B(){

}

void opfunc_0x6C(){
  regPC = *(u16*)&cpuMem[*(u16*)&cpuMem[regPC+1]];
}

void opfunc_0x6D(){

}

void opfunc_0x6E(){
  ror(abs_());
}

void opfunc_0x6F(){

}

void opfunc_0x70(){
  bxx(OVER_SET);
}

void opfunc_0x71(){

}

void opfunc_0x72(){

}

void opfunc_0x73(){

}

void opfunc_0x74(){

}

void opfunc_0x75(){
  adc(*zpx());
}

void opfunc_0x76(){
  ror(zpx());
}

void opfunc_0x77(){

}

void opfunc_0x78(){
  sex(INTER_MASK);
}

void opfunc_0x79(){

}

void opfunc_0x7A(){

}

void opfunc_0x7B(){

}

void opfunc_0x7C(){

}

void opfunc_0x7D(){

}

void opfunc_0x7E(){
  ror(absx());
}

void opfunc_0x7F(){

}

void opfunc_0x80(){

}

void opfunc_0x81(){
  st(regA, iix());
}

void opfunc_0x82(){

}

void opfunc_0x83(){

}

void opfunc_0x84(){
  st(regY, zp());
}

void opfunc_0x85(){
  st(regA, zp());
}

void opfunc_0x86(){
  st(regX, zp());
}

void opfunc_0x87(){

}

void opfunc_0x88(){
  dec(&regY);
}

void opfunc_0x89(){

}

void opfunc_0x8A(){
  trs(regX, &regA);
}

void opfunc_0x8B(){

}

void opfunc_0x8C(){
  st(regY, abs_());
}

void opfunc_0x8D(){
  st(regA, abs_());
}

void opfunc_0x8E(){
  st(regX, abs_());
}

void opfunc_0x8F(){

}

void opfunc_0x90(){
  bxx(CARRY_CLEAR);
}

void opfunc_0x91(){
  st(regA, iiy());
}

void opfunc_0x92(){

}

void opfunc_0x93(){

}

void opfunc_0x94(){
  st(regY, zpx());
}

void opfunc_0x95(){
  st(regA, zpx());
}

void opfunc_0x96(){
  st(regX, zpy());
}

void opfunc_0x97(){

}

void opfunc_0x98(){
  trs(regY, &regA);
}

void opfunc_0x99(){
  st(regA, absy());
}

void opfunc_0x9A(){
  ((u8*)&regSP)[1] = regX;
}

void opfunc_0x9B(){

}

void opfunc_0x9C(){

}

void opfunc_0x9D(){
  st(regA, absx());
}

void opfunc_0x9E(){

}

void opfunc_0x9F(){

}

void opfunc_0xA0(){
  ld(&regY, imm());
}

void opfunc_0xA1(){
  ld(&regA, *iix());
}

void opfunc_0xA2(){
  ld(&regX, imm());
}

void opfunc_0xA3(){

}

void opfunc_0xA4(){
  ld(&regY, *zp());
}

void opfunc_0xA5(){
  ld(&regA, *zp());
}

void opfunc_0xA6(){
  ld(&regX, *zp());
}

void opfunc_0xA7(){

}

void opfunc_0xA8(){
  trs(regA, &regY);
}

void opfunc_0xA9(){
  ld(&regA, imm());
}

void opfunc_0xAA(){
  trs(regA, &regX);
}

void opfunc_0xAB(){
  
}

void opfunc_0xAC(){
  ld(&regY, *abs_());
}

void opfunc_0xAD(){
  ld(&regA, *abs_());
}

void opfunc_0xAE(){
  ld(&regX, *abs_());
}

void opfunc_0xAF(){

}

void opfunc_0xB0(){
  bxx(CARRY_SET);
}

void opfunc_0xB1(){
  ld(&regA, *iiy());
}

void opfunc_0xB2(){

}

void opfunc_0xB3(){

}

void opfunc_0xB4(){
  ld(&regY, *zpx());
}

void opfunc_0xB5(){
  ld(&regA, *zpx());
}

void opfunc_0xB6(){
  ld(&regX, *zpy());
}

void opfunc_0xB7(){

}

void opfunc_0xB8(){
  clx(OVER_MASK);
}

void opfunc_0xB9(){
  ld(&regA, *absy());
}

void opfunc_0xBA(){
  trs(((u8*)&regSP)[1], &regX);
}

void opfunc_0xBB(){

}

void opfunc_0xBC(){
  ld(&regY, *absx());
}

void opfunc_0xBD(){
  ld(&regA, *absx());
}

void opfunc_0xBE(){
  ld(&regX, *absy());
}

void opfunc_0xBF(){

}

void opfunc_0xC0(){
  cmp(regY, imm());
}

void opfunc_0xC1(){
  cmp(regA, *iix());
}

void opfunc_0xC2(){

}

void opfunc_0xC3(){

}

void opfunc_0xC4(){
  cmp(regY, *zp());
}

void opfunc_0xC5(){
  cmp(regA, *zp());
}

void opfunc_0xC6(){
  dec(zp());
}

void opfunc_0xC7(){

}

void opfunc_0xC8(){
  inc(&regY);
}

void opfunc_0xC9(){
  cmp(regA, imm());
}

void opfunc_0xCA(){
  dec(&regX);
}

void opfunc_0xCB(){

}

void opfunc_0xCC(){
  cmp(regY, *abs_());
}

void opfunc_0xCD(){
  cmp(regA, *abs_());
}

void opfunc_0xCE(){
  dec(abs_());
}

void opfunc_0xCF(){

}

void opfunc_0xD0(){
  bxx(ZERO_CLEAR);
}

void opfunc_0xD1(){
  cmp(regA, *iiy());
}

void opfunc_0xD2(){

}

void opfunc_0xD3(){

}

void opfunc_0xD4(){

}

void opfunc_0xD5(){
  cmp(regA, *zpx());
}

void opfunc_0xD6(){
  dec(zpx());
}

void opfunc_0xD7(){

}

void opfunc_0xD8(){
  clx(DEC_MASK);
}

void opfunc_0xD9(){
  cmp(regA, *absy());
}

void opfunc_0xDA(){

}

void opfunc_0xDB(){

}

void opfunc_0xDC(){
  
}

void opfunc_0xDD(){
  cmp(regA, *absx());
}

void opfunc_0xDE(){
  dec(absx());
}

void opfunc_0xDF(){

}

void opfunc_0xE0(){
  cmp(regX, imm());
}

void opfunc_0xE1(){
  sbc(*iix());
}

void opfunc_0xE2(){

}

void opfunc_0xE3(){

}

void opfunc_0xE4(){
  cmp(regX, *zp());
}

void opfunc_0xE5(){
  sbc(*zp());
}

void opfunc_0xE6(){
  inc(zp());
}

void opfunc_0xE7(){

}

void opfunc_0xE8(){
  inc(&regX);
}

void opfunc_0xE9(){
  sbc(imm());
}

void opfunc_0xEA(){

}

void opfunc_0xEB(){

}

void opfunc_0xEC(){
  cmp(regX, *abs_());
}

void opfunc_0xED(){
  sbc(*abs_());
}

void opfunc_0xEE(){
  inc(abs_());
}

void opfunc_0xEF(){

}

void opfunc_0xF0(){
  bxx(ZERO_SET);
}

void opfunc_0xF1(){
  sbc(*iiy());
}

void opfunc_0xF2(){

}

void opfunc_0xF3(){

}

void opfunc_0xF4(){

}

void opfunc_0xF5(){
  sbc(*zpx());
}

void opfunc_0xF6(){
  inc(zpx());
}

void opfunc_0xF7(){

}

void opfunc_0xF8(){
  sex(DEC_MASK);
}

void opfunc_0xF9(){
  sbc(*absy());
}

void opfunc_0xFA(){

}

void opfunc_0xFB(){

}

void opfunc_0xFC(){

}

void opfunc_0xFD(){

}

void opfunc_0xFE(){
  inc(absx());
}

void opfunc_0xFF(){

}
