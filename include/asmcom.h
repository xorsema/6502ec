#ifndef ASMCOM_H
#define ASMCOM_H

void asmcom_init();
void asmcom_quit();
void asmcom_loop();
void asmcom_drawscreen(u8 *screenmem);
void asmcom_renderframe();
void die(const char *msg);

#define PIXEL_SIZE 8

#endif
