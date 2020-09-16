/*---------------------------------------------------------------------------------------------------
|   SCREEN_DRIVER.C    |                                                                            |
|-----------------------                                                                            |
| These functions provide an interface to operate the screen in video mode (VESA).        	        |
---------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <screen_driver.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                            HARDWARE SETUP
-------------------------------------------------------------------------------------------------------------------------- */

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));


// Remember to enable cfg_vesa at sysvar.asm
// Screen info address taken from VBEModeInfoBlock at sysvar.asm
struct vbe_mode_info_structure * screenInfo = (struct vbe_mode_info_structure *) 0x5C00;

// Default resolution of VESA
const uint16_t * WIDTH = (uint16_t *) 0x5084;
const uint16_t * HEIGHT = (uint16_t *) 0x5086;


/* --------------------------------------------------------------------------------------------------------------------------
                                            DRAW METHODS
-------------------------------------------------------------------------------------------------------------------------- */

int draw(int x, int y, int rgb){

	if(x<0 || y<0 || x>*WIDTH || y>*HEIGHT)
		return -1;

	if(rgb<0)
		return -2;

    char * pos = (char *) ((uint64_t) screenInfo->framebuffer + (x + y * *WIDTH)*3);

    pos[0] = (rgb & 0x0000FF);
    pos[1] = (rgb & 0x00FF00) >> 8;
    pos[2] = (rgb & 0xFF0000) >> 16;

	return 0;

}


int getResWidth(){
	
	return *WIDTH;

}


int getResHeight(){

	return *HEIGHT;

}