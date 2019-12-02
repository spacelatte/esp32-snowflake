
#ifndef _BITMAP_H_
#define _BITMAP_H_

typedef union header_file_magic {
	int16_t value;
	uint8_t bytes[2];
} __attribute__((transparent_union)) header_file_magic_u;

typedef union color {
	uint32_t uint32;
	int32_t int32;
	char values[4];
	struct {
		char red;
		char green;
		char blue;
		char alpha;
	};
} __attribute__((packed, transparent_union)) color_u;

typedef struct header_file {
	header_file_magic_u magic;
	uint32_t size;
	union {
		int32_t value;
		uint8_t bytes[4];
		int16_t field[2];
	} __attribute__((transparent_union)) reserved;
	uint32_t offset;
} __attribute__((packed)) header_file_t;

typedef struct header_info {
	uint32_t size;
	struct {
		int32_t  width;
		int32_t  height;
		uint16_t planes;
		uint16_t bits;
		uint32_t compression;
		uint32_t size;
	} image;
	union {
		int32_t array[2];
		struct {
			int32_t x;
			int32_t y;
		};
	} __attribute__((transparent_union)) res;
	union {
		uint32_t array[2];
		struct {
			uint32_t count;
			uint32_t important;
		};
	} __attribute__((transparent_union)) colors;
} __attribute__((packed)) header_info_t;

typedef struct header {
	header_file_t file;
	header_info_t info;
} __attribute__((packed)) header_t;

const header_file_magic_u header_file_magic_default = { .bytes = { 'B', 'M', }, };

uint32_t
color_value(color_u color) {
	return (0
		+ (int) color.red
		+ (int) color.green
		+ (int) color.blue
		+ (int) color.alpha
	);
}

header_t
header_create(size_t width, size_t height, short bits) {
	return (header_t) {
		.file = {
			.magic    = header_file_magic_default,
			.size     = sizeof(header_t) + ((width * height * bits) >> 3),
			.reserved = { .value = 0x00, },
			.offset   = sizeof(header_t),
		},
		.info = {
			.size = sizeof(header_info_t),
			.image = {
				.width       = width,
				.height      = height,
				.planes      = 1,
				.bits        = bits,
				.compression = 0,
				.size        = (width * height * bits) >> 3,
			},
			.res = {
				.x = 0,
				.y = 0,
			},
			.colors = {
				.count     = (0 << bits),
				.important = (0 << bits),
			},
		},
	};
}

char*
header_serialize(const header_t *header) {
	const unsigned size = 8192;
	char *buffer = (char*) calloc(size, sizeof(char));
	snprintf(buffer, size,
		"\nfile.magic:             %x"
		"\nfile.size:              %d"
		"\nfile.offset:            %d"
		"\ninfo.size:              %d"
		"\ninfo.image.width:       %d"
		"\ninfo.image.height:      %d"
		"\ninfo.image.planes:      %d"
		"\ninfo.image.bits:        %d"
		"\ninfo.image.compression: %d"
		"\ninfo.image.size:        %d"
		"\ninfo.resolution.x:      %d"
		"\ninfo.resolution.y:      %d"
		"\ninfo.colors.count:      %d"
		"\ninfo.colors.important:  %d"
		"\n",
		header->file.magic.value,
		header->file.size,
		header->file.offset,
		header->info.size,
		header->info.image.width,
		header->info.image.height,
		header->info.image.planes,
		header->info.image.bits,
		header->info.image.compression,
		header->info.image.size,
		header->info.res.x,
		header->info.res.y,
		header->info.colors.count,
		header->info.colors.important,
		NULL
	);
	return buffer;
}

#endif // _BITMAP_H_
