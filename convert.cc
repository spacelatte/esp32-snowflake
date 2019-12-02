
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bitmap.h"


int
main(int argc, char **argv) {
	header_file_t file;
	fread(&file, sizeof(file), 1, stdin);
	if(header_file_magic_default.value != file.magic.value) {
		return 1;
	}

	// read metadata
	char meta[file.offset - sizeof(file)];
	fread(&meta, sizeof(meta), 1, stdin);

	// read image
	char pixels[file.size - file.offset];
	fread(&pixels, sizeof(pixels), 1, stdin);

	color_u *colors = (color_u*) pixels;

	// dump image pixels :/
	for(int i=0; i<sizeof(pixels)/sizeof(*colors); i+=8) {
		uint8_t byte = 0;
		for(int j=0; j<8; j++) {
			byte |= (((colors[i+j].uint32 & 0xFF) > 0x80) << j);
			continue;
		}
		fprintf(stdout, "%c%s%02hhx,",
			(i%8 ? ' ' : '\n'), "0x",
			(char) (byte & 0xFF), NULL
		);
		continue;
	}
	fprintf(stdout, "\n");
	header_t header = {
		.file = file,
	};
	memcpy(&header.info, meta, sizeof(header.info));
	char *str = header_serialize(&header);
	fprintf(stderr, "%s\n", str);
	free(str);
	return 0;
}
