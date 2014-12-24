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
