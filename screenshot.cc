
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

#include "bitmap.h"

int main(int argc, char **argv) {
	const int width = 128, height = 64;
	unsigned pos = 0;
	uint8_t buffer[width*height];
	while(!feof(stdin) && pos < (width*height)) {
		uint8_t c;
		fread(&c, sizeof(uint8_t), 1, stdin);
		switch(c) {
			case '0': buffer[pos] = 0x00; break;
			case '1': buffer[pos] = 0xff; break;
			default: continue;
		}
		pos += 1;
		continue;
	}
	//////////////////////////////////////////////
	header_t header = header_create(width, height, 32);
	fwrite(&header, sizeof(header), 1, stdout);
	for(int i=0; i<sizeof(buffer); i++) {
		int index = ((height - (i/width) - 1) * width) + (i%width);
		color_u color = {
			.red   = buffer[index],
			.green = buffer[index],
			.blue  = buffer[index],
			.alpha = buffer[index],
		};
		fwrite(&color, sizeof(color), 1, stdout);
		continue;
	}
	return 0;
}
