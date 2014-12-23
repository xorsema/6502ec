#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <6502e.h>
#include <asmcom.h>

int main(int argc, char **argv){
  char c = 0;

  if(argc > 2){
    int ldloc = strtol(argv[2], NULL, 16);
    int pcloc = (argc > 3) ? strtol(argv[3], NULL, 16) : ldloc;
    cpu6502_loadfile(argv[1], ldloc);
    cpu6502_setPC(pcloc);
  }
  else{
    printf("usage: %s <filename.bin> <load location> <start location>\n", argv[0]);
    return 1;
  }
  
  /*do{
    cpu6502_print();
    c = getchar();
    cpu6502_step();
    }while(c != 'q');*/

  asmcom_init();
  asmcom_loop();
  asmcom_quit();
  return 0;
}
