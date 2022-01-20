#include "gba.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {
  // TODO: IMPLEMENT
  while(SCANLINECOUNTER > 160) {
            ;
  }
  while(SCANLINECOUNTER < 160) {
            ;
  }
  vBlankCounter++;

  // (1)
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)

  // (2)
  // Write a while loop that keeps going until we're in vBlank:

  // (3)
  // Finally, increment the vBlank counter:
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

void setPixel(int row, int col, u16 color) {
  videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
  // TODO: IMPLEMENT
  for (int x = 0; x < height; x++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + x, col, 240)];
		DMA[3].cnt = (width) | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void drawFullScreenImageDMA(const u16 *image) {
  // TODO: IMPLEMENT
  DMA[3].src = image;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = (WIDTH * HEIGHT) | DMA_ON | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
}

void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
  // TODO: IMPLEMENT
  for (int x = 0; x < height; x++) {
		DMA[3].src = &image[OFFSET(x, 0, width)];
		DMA[3].dst = &videoBuffer[OFFSET(row + x, col, 240)];
		DMA[3].cnt = DMA_ON | width;
	}
}

void fillScreenDMA(volatile u16 color) {
  // TODO: IMPLEMENT
  DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (WIDTH * HEIGHT) | DMA_ON | DMA_SOURCE_FIXED;
}

void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawCenteredString(int row, int col, int width, int height, char *str,
                        u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}
/*
void setPlay(Frisk frisk) {
  fillScreenDMA(BLACK);
  drawRectDMA(20, 60, 121, 121, WHITE);
  drawRectDMA(frisk.row, frisk.col, frisk.length, frisk.height, MAGENTA);
  waitForVBlank();
}
*/
unsigned short colors[] = {RED, BLUE, GREEN, YELLOW, MAGENTA, ORANGE, PURPLE};        
int ncolors = sizeof(colors) / sizeof(colors[0]);

void setBoard(Board board) {
  int x = 20;
  int y = 60;
  for (int j = 0; j < 11; j++) {
    for (int i = 0; i < 11; i++) {
      board.squares[i].color = (colors[rand() % ncolors]);
      drawRectDMA(x, y, 11, 11, board.squares[i].color);
      x += 11;
    }
    y += 11;
    x -= 121;
  }
  waitForVBlank();
  return;
}