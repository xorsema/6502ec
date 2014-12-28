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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <6502e.h>
#include <asmcom.h>

int main(int argc, char **argv){
  char c = 0;
  int debug = 0;
  if(argc > 1 && !strcmp("-d", argv[1])){
    debug = 1;
  }
  if(argc > 2){
    int ldloc = strtol(argv[2+debug], NULL, 16);
    int pcloc = (argc > 3+debug) ? strtol(argv[3+debug], NULL, 16) : ldloc;
    cpu6502_loadfile(argv[1+debug], ldloc);
    cpu6502_setPC(pcloc);
  }
  else{
    printf("usage: %s <filename.bin> <load location> <start location>\n", argv[0]);
    return 1;
  }
  
  if(debug){
    asmcom_init();
    do{
      cpu6502_print();
      c = getchar();
      cpu6502_step();
      asmcom_renderframe();
    }while(c != 'q');
    asmcom_quit();
  }
  else{
    asmcom_init();
    asmcom_loop();
    asmcom_quit();
  }
  return 0;
}
