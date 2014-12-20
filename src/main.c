#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <6502e.h>

int main(int argc, char **argv){
  char c = 0;

  if(argc > 1){
    cpu6502_loadfile(argv[1], 0);
    cpu6502_setPC(0);
  }
  else{
    printf("usage: %s <filename.bin>\n", argv[0]);
    return 1;
  }
  
  do{
    cpu6502_print();
    c = getchar();
    cpu6502_step();
  }while(c != 'q');
  return 0;
}
