/*
The MIT License (MIT)

Copyright (c) 2014 Max Rose

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef _6502E_H
#define _6502E_H

#define CARRY_MASK (1 << 6)
#define ZERO_MASK (1 << 5)
#define INTER_MASK (1 << 4)
#define DEC_MASK (1 << 3)
#define BRK_MASK (1 << 2)
#define OVER_MASK (1 << 1)
#define NEG_MASK (1)

#define CARRY_CLEAR (!(regFlags & CARRY_MASK))
#define CARRY_SET (regFlags & CARRY_MASK)
#define ZERO_SET (regFlags & ZERO_MASK)
#define NEG_SET (regFlags & NEG_MASK)
#define ZERO_CLEAR (!(regFlags & ZERO_MASK))
#define NEG_CLEAR (!(regFlags & NEG_MASK))
#define OVER_CLEAR (!(regFlags & OVER_MASK))
#define OVER_SET (regFlags & OVER_MASK)

typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;

typedef void (*opfunc)();

void cpu6502_step();
void cpu6502_print();
void cpu6502_load(u16 loc, u8 *code, unsigned length);
void cpu6502_loadfile(const char *fln, u16 loc);
void cpu6502_setPC(u16 val);
u8 *cpu6502_getMemPtr();
unsigned cpu6502_getCycles();

void opfunc_0x0();
void opfunc_0x1();
void opfunc_0x2();
void opfunc_0x3();
void opfunc_0x4();
void opfunc_0x5();
void opfunc_0x6();
void opfunc_0x7();
void opfunc_0x8();
void opfunc_0x9();
void opfunc_0xA();
void opfunc_0xB();
void opfunc_0xC();
void opfunc_0xD();
void opfunc_0xE();
void opfunc_0xF();
void opfunc_0x10();
void opfunc_0x11();
void opfunc_0x12();
void opfunc_0x13();
void opfunc_0x14();
void opfunc_0x15();
void opfunc_0x16();
void opfunc_0x17();
void opfunc_0x18();
void opfunc_0x19();
void opfunc_0x1A();
void opfunc_0x1B();
void opfunc_0x1C();
void opfunc_0x1D();
void opfunc_0x1E();
void opfunc_0x1F();
void opfunc_0x20();
void opfunc_0x21();
void opfunc_0x22();
void opfunc_0x23();
void opfunc_0x24();
void opfunc_0x25();
void opfunc_0x26();
void opfunc_0x27();
void opfunc_0x28();
void opfunc_0x29();
void opfunc_0x2A();
void opfunc_0x2B();
void opfunc_0x2C();
void opfunc_0x2D();
void opfunc_0x2E();
void opfunc_0x2F();
void opfunc_0x30();
void opfunc_0x31();
void opfunc_0x32();
void opfunc_0x33();
void opfunc_0x34();
void opfunc_0x35();
void opfunc_0x36();
void opfunc_0x37();
void opfunc_0x38();
void opfunc_0x39();
void opfunc_0x3A();
void opfunc_0x3B();
void opfunc_0x3C();
void opfunc_0x3D();
void opfunc_0x3E();
void opfunc_0x3F();
void opfunc_0x40();
void opfunc_0x41();
void opfunc_0x42();
void opfunc_0x43();
void opfunc_0x44();
void opfunc_0x45();
void opfunc_0x46();
void opfunc_0x47();
void opfunc_0x48();
void opfunc_0x49();
void opfunc_0x4A();
void opfunc_0x4B();
void opfunc_0x4C();
void opfunc_0x4D();
void opfunc_0x4E();
void opfunc_0x4F();
void opfunc_0x50();
void opfunc_0x51();
void opfunc_0x52();
void opfunc_0x53();
void opfunc_0x54();
void opfunc_0x55();
void opfunc_0x56();
void opfunc_0x57();
void opfunc_0x58();
void opfunc_0x59();
void opfunc_0x5A();
void opfunc_0x5B();
void opfunc_0x5C();
void opfunc_0x5D();
void opfunc_0x5E();
void opfunc_0x5F();
void opfunc_0x60();
void opfunc_0x61();
void opfunc_0x62();
void opfunc_0x63();
void opfunc_0x64();
void opfunc_0x65();
void opfunc_0x66();
void opfunc_0x67();
void opfunc_0x68();
void opfunc_0x69();
void opfunc_0x6A();
void opfunc_0x6B();
void opfunc_0x6C();
void opfunc_0x6D();
void opfunc_0x6E();
void opfunc_0x6F();
void opfunc_0x70();
void opfunc_0x71();
void opfunc_0x72();
void opfunc_0x73();
void opfunc_0x74();
void opfunc_0x75();
void opfunc_0x76();
void opfunc_0x77();
void opfunc_0x78();
void opfunc_0x79();
void opfunc_0x7A();
void opfunc_0x7B();
void opfunc_0x7C();
void opfunc_0x7D();
void opfunc_0x7E();
void opfunc_0x7F();
void opfunc_0x80();
void opfunc_0x81();
void opfunc_0x82();
void opfunc_0x83();
void opfunc_0x84();
void opfunc_0x85();
void opfunc_0x86();
void opfunc_0x87();
void opfunc_0x88();
void opfunc_0x89();
void opfunc_0x8A();
void opfunc_0x8B();
void opfunc_0x8C();
void opfunc_0x8D();
void opfunc_0x8E();
void opfunc_0x8F();
void opfunc_0x90();
void opfunc_0x91();
void opfunc_0x92();
void opfunc_0x93();
void opfunc_0x94();
void opfunc_0x95();
void opfunc_0x96();
void opfunc_0x97();
void opfunc_0x98();
void opfunc_0x99();
void opfunc_0x9A();
void opfunc_0x9B();
void opfunc_0x9C();
void opfunc_0x9D();
void opfunc_0x9E();
void opfunc_0x9F();
void opfunc_0xA0();
void opfunc_0xA1();
void opfunc_0xA2();
void opfunc_0xA3();
void opfunc_0xA4();
void opfunc_0xA5();
void opfunc_0xA6();
void opfunc_0xA7();
void opfunc_0xA8();
void opfunc_0xA9();
void opfunc_0xAA();
void opfunc_0xAB();
void opfunc_0xAC();
void opfunc_0xAD();
void opfunc_0xAE();
void opfunc_0xAF();
void opfunc_0xB0();
void opfunc_0xB1();
void opfunc_0xB2();
void opfunc_0xB3();
void opfunc_0xB4();
void opfunc_0xB5();
void opfunc_0xB6();
void opfunc_0xB7();
void opfunc_0xB8();
void opfunc_0xB9();
void opfunc_0xBA();
void opfunc_0xBB();
void opfunc_0xBC();
void opfunc_0xBD();
void opfunc_0xBE();
void opfunc_0xBF();
void opfunc_0xC0();
void opfunc_0xC1();
void opfunc_0xC2();
void opfunc_0xC3();
void opfunc_0xC4();
void opfunc_0xC5();
void opfunc_0xC6();
void opfunc_0xC7();
void opfunc_0xC8();
void opfunc_0xC9();
void opfunc_0xCA();
void opfunc_0xCB();
void opfunc_0xCC();
void opfunc_0xCD();
void opfunc_0xCE();
void opfunc_0xCF();
void opfunc_0xD0();
void opfunc_0xD1();
void opfunc_0xD2();
void opfunc_0xD3();
void opfunc_0xD4();
void opfunc_0xD5();
void opfunc_0xD6();
void opfunc_0xD7();
void opfunc_0xD8();
void opfunc_0xD9();
void opfunc_0xDA();
void opfunc_0xDB();
void opfunc_0xDC();
void opfunc_0xDD();
void opfunc_0xDE();
void opfunc_0xDF();
void opfunc_0xE0();
void opfunc_0xE1();
void opfunc_0xE2();
void opfunc_0xE3();
void opfunc_0xE4();
void opfunc_0xE5();
void opfunc_0xE6();
void opfunc_0xE7();
void opfunc_0xE8();
void opfunc_0xE9();
void opfunc_0xEA();
void opfunc_0xEB();
void opfunc_0xEC();
void opfunc_0xED();
void opfunc_0xEE();
void opfunc_0xEF();
void opfunc_0xF0();
void opfunc_0xF1();
void opfunc_0xF2();
void opfunc_0xF3();
void opfunc_0xF4();
void opfunc_0xF5();
void opfunc_0xF6();
void opfunc_0xF7();
void opfunc_0xF8();
void opfunc_0xF9();
void opfunc_0xFA();
void opfunc_0xFB();
void opfunc_0xFC();
void opfunc_0xFD();
void opfunc_0xFE();
void opfunc_0xFF();

#endif
