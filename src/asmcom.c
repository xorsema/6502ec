#include <SDL2/SDL.h>
#include <6502e.h>
#include <asmcom.h>
#include <time.h>
#include <stdlib.h>

SDL_Window	*g_window;
SDL_Renderer	*g_renderer;
int		 quit = 0;

const char	*WIN_TITLE     = "6502asm.com";
const unsigned	 SCREEN_WIDTH  = 32*PIXEL_SIZE;
const unsigned	 SCREEN_HEIGHT = 32*PIXEL_SIZE;
const SDL_Color COLOR_PALETTE[0x10] = {
  {0, 0, 0, 0xff},
  {0xff, 0xff, 0xff, 0xff},
  {0x88, 0, 0, 0xff},
  {0xaa, 0xff, 0xee, 0xff},
  {0xcc, 0x44, 0xcc, 0xff},
  {0, 0xcc, 0x55, 0xff},
  {0, 0, 0xaa, 0xff},
  {0xee, 0xee, 0x77, 0xff},
  {0xdd, 0x88, 0x55, 0xff},
  {0x66, 0x44, 0, 0xff},
  {0xff, 0x77, 0x77, 0xff},
  {0x33, 0x33, 0x33, 0xff},
  {0x77, 0x77, 0x77, 0xff},
  {0xaa, 0xff, 0x66, 0xff},
  {0, 0x88, 0xff, 0xff},
  {0xbb, 0xbb, 0xbb, 0xff}
};
  
static int asmcom_CpuThread(void *ptr);

void asmcom_init(){
  srand(time(NULL));
  g_window = NULL;
  g_renderer = NULL;
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
    die(SDL_GetError());
  }

  if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH,
				 SCREEN_HEIGHT,
				 SDL_WINDOW_SHOWN,
				 &g_window,
				 &g_renderer) < 0){
    die(SDL_GetError());
  }

  SDL_SetWindowTitle(g_window, WIN_TITLE);
}

void asmcom_quit(){
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}

/* Used for debug mode only */
void asmcom_renderframe(){
  SDL_RenderClear(g_renderer);
  asmcom_drawscreen(cpu6502_getMemPtr()+0x200);
  SDL_RenderPresent(g_renderer);
}

void asmcom_loop(){
  SDL_Event event;
  u8 *mem = cpu6502_getMemPtr();
  u8 *ptr = mem+0x200;
  SDL_CreateThread(asmcom_CpuThread, "CpuThread",  (void*)NULL);
  while(!quit){
    while(SDL_PollEvent(&event)){
      switch(event.type){
      case SDL_QUIT:
	quit = 1;
	break;
      case SDL_KEYDOWN:
	*(mem + 0xFF) = event.key.keysym.sym;
	break;
      }
    }

    SDL_RenderClear(g_renderer);
    asmcom_drawscreen(ptr);
    SDL_RenderPresent(g_renderer);
  }
}

void asmcom_drawscreen(u8 *screenmem){
  unsigned i, x = 0, y = 0;
  SDL_Rect r = {0, 0, PIXEL_SIZE, PIXEL_SIZE};
  SDL_Color c;
  u8 b;
  
  for(i = 0; i < 0x400; i++, x = (x+1) % 32, y += (i % 32 == 0) ? 1 : 0){
    b = screenmem[i];
    c = COLOR_PALETTE[b & 0xf];
    r.x = x*PIXEL_SIZE;
    r.y = y*PIXEL_SIZE;
    SDL_SetRenderDrawColor(g_renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(g_renderer, &r);
  }
}

static int asmcom_CpuThread(void *ptr){
  while(!quit){
    *(cpu6502_getMemPtr()+0xFE) = rand() % 0xFF;
    cpu6502_step();
  }
  return 0;
}

void die(const char *msg){
  fprintf(stderr, "Fatal error: %s\n", msg);
  exit(EXIT_FAILURE);
}
