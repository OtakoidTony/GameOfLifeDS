// Includes
#include <nds.h>	// Include for NDS_Lib
#include <stdio.h>

#include <nf_lib.h>
#include <unistd.h>

// C++ Includes
#include <iostream>
#include <string>


#define WHITE 0xFFFF
#define BLACK 0x0000
#define SKY 0xEEEF
#define BLUE 0xFC00
#define MAGENTA 0xFC1F
#define CYAN 0xFFE0

void OnKeyPressed(int key) {
   if(key > 0)
      iprintf("%c", key);
}

void draw_shape(int x, int y, uint16* buffer, uint16 color)
{
	buffer += y * SCREEN_WIDTH + x;
	uint16* line = buffer + (SCREEN_WIDTH);
	*line++ = color;
}

void draw_shape(int x, int y, uint16* buffer, uint16 color, int xsize, int ysize){
	int xtemp=0;
	int ytemp=0;

	while(xtemp<xsize){
		while(ytemp<ysize){
			draw_shape(x+xtemp,y+ytemp,buffer,color);

			ytemp=ytemp+1;
		}
		ytemp=0;
		xtemp=xtemp+1;
	}
}
	short screenbuffer_maindisplay[49152];
	short screen_width = 256;
	short screen_height = 192;

	void push_frame() {
		dmaCopy(&screenbuffer_maindisplay, BG_GFX, 49152 * 2);
	}

	void clear_screenbuffer(short colour) {
		for (int x = 0; x < 49152; x += 1) {
			screenbuffer_maindisplay[x] = colour;
		}
	}

	void fill_rectangle(short colour, int w, int h, int dx, int dy) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if ((x + dx) >= 0 && (x + dx) < 256 && (y + dy) >= 0 && (y + dy) < 192) {
					screenbuffer_maindisplay[(dy + y)*screen_width + (x + dx)] = colour;
				}
			}
		}
	}

	void plot_pixel(int x, int y, short colour) {
		if ((x) >= 0 && (x) < 256 && (y) >= 0 && (y) < 192) {
			screenbuffer_maindisplay[y*screen_width + x] = colour;
		}
	}

	void draw_line(int x1, int y1, int x2, int y2, short col) {
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1; y = y1; xe = x2;
			}
			else
			{
				x = x2; y = y2; xe = x1;
			}

			plot_pixel(x, y, col);

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				plot_pixel(x, y, col);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1; y = y1; ye = y2;
			}
			else
			{
				x = x2; y = y2; ye = y1;
			}

			plot_pixel(x, y, col);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				plot_pixel(x, y, col);
			}
		}
	}
}
/*
void draw_linear(int a, int b){
	draw_shape(0,b,
*/

int GetVram() {
	if (videoGetMode() == MODE_FB0) { return VRAM_A; }
	if (videoGetMode() == MODE_FB1) { return VRAM_B; }
	if (videoGetMode() == MODE_FB2) { return VRAM_C; }
	if (videoGetMode() == MODE_FB3) { return VRAM_D; }
}



void WhileTest(int x, int y) {
	int xtemp = 0, ytemp = 0;

	while (xtemp < x) {
		iprintf("First While\n");
		while (ytemp < y) {
			iprintf("    Second While\n");
			++ytemp;
		}
		ytemp = 0;
		iprintf("\n");
		++xtemp;
	}

}

int main(void)
{
	consoleDemoInit();
	//irqInit();
	//irqEnable(IRQ_VBLANK);
	// a vblank interrupt is needed to use swiWaitForVBlank()
	// since the dispatcher handles the flags no handler is required

	// put the main screen on the bottom lcd
	lcdMainOnBottom();

	touchPosition touch;

	/*
	MODE_FIFO
	video display from main memory

	MODE_FB0
	video display directly from VRAM_A in LCD mode

	MODE_FB1
	video display directly from VRAM_B in LCD mode

	MODE_FB2
	video display directly from VRAM_C in LCD mode

	MODE_FB3
	video display directly from VRAM_D in LCD mode
	*/

	//the main 2D engine video mode
	videoSetMode(MODE_FB1);
	vramSetBankB(VRAM_B_LCD);

	// VRAM_A = (u16*)0x6800000
	// VRAM_B = (u16*)0x6820000
	draw_shape(20, 20, (u16*)0x6820000, RGB15(31, 31, 31), 5, 5);
	draw_shape(25, 20, (u16*)0x6820000, RGB15(31, 0, 31), 5, 5);
	draw_shape(30, 20, (u16*)0x6820000, RGB15(31, 31, 31), 5, 5);
	draw_shape(20, 25, (u16*)0x6820000, RGB15(31, 0, 31), 5, 5);
	draw_shape(25, 25, (u16*)0x6820000, RGB15(31, 31, 31), 5, 5);

	int area = 0;
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	consoleDemoInit();

	draw_line(1, 1, 10, 10, WHITE);
	while (1) {
		scanKeys();
		int held = keysHeld();

		draw_shape(touch.px, touch.py, VRAM_A, RGB15(31, 31, 31), 5, 5);

		if (held & KEY_START) break;

		if (held & KEY_TOUCH) {

			touchRead(&touch);

			if (touch.z1 != 0) {
				area = touch.px * touch.z2 / touch.z1 - touch.px;
			}

			iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);

			iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

			iprintf("Touch Area (pressure) %04i\n", area);

		}

		swiWaitForVBlank();
	}
	return 0;
}
