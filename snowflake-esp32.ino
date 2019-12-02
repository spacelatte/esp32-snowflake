
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    16
#define FLAKE_COUNT   12
#define FLAKE_WIDTH    8
#define FLAKE_HEIGHT   8


#define XPOS   0
#define YPOS   1
#define DELTAY 2


int16_t icons[FLAKE_COUNT][3];
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t flakes[][64] = {
	{
		#include "00.h"
	},
	{
		#include "01.h"
	},
	{
		#include "02.h"
	},
	{
		#include "03.h"
	},
	{
		#include "04.h"
	},
	{
		#include "05.h"
	},
	{
		#include "06.h"
	},
	{
		#include "07.h"
	},
	{
		#include "08.h"
	},
	{
		#include "09.h"
	},
	{
		#include "10.h"
	},
	{
		#include "11.h"
	},
	{
		#include "12.h"
	},
};

void setup() {
	Serial.begin(115200);
	Wire.begin(4, 15, 4E5L);
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println("SSD1306 allocation failed");
		while(true) delay(1);
	}
	for(int i=0; i<FLAKE_COUNT; i++) {
		icons[i][YPOS] = display.height() + FLAKE_HEIGHT;
	}
	return;
}

char* display_dump(Adafruit_SSD1306 &d) {
	unsigned position = 0;
	char *buffer = (char*) calloc(1
		+ (d.width() * d.height())
		+ d.height(),
		sizeof(char)
	);
	for(int i=0; i<d.height(); i++) {
		for(int j=0; j<d.width(); j+=8) {
			unsigned char b = 0;
			for(int k=0; k<8; k++) {
				bool pixel = d.getPixel(j+k, i);
				// use branchless: equalivent to `(pixel ? '1' : '0')`
				buffer[position++] = (pixel + '0');
				//b |= pixel << k;
				continue;
			}
			continue;
		}
		buffer[position++] = '\n';
		continue;
	}
	return buffer;
}

void loop() {
	if(Serial.available()) {
		Serial.read();
		char *data = display_dump(display);
		Serial.write(data);
		free(data);
	}

	display.clearDisplay();
	display.setCursor(10, 10);
	display.setTextWrap(false);
	display.setTextColor(SSD1306_WHITE);
	display.printf("%02d:%02d hello world",
		millis()/1000/60%60,
		millis()/1000%60,
		NULL
	);

	for(int i=0; i<FLAKE_COUNT; i++) {
		icons[i][YPOS] += icons[i][DELTAY];
		if (icons[i][YPOS] >= display.height()) {
			icons[i][XPOS]   = random(1 - FLAKE_WIDTH, display.width());
			icons[i][YPOS]   = -FLAKE_HEIGHT;
			icons[i][DELTAY] = random(1, 6);
		}
		display.drawBitmap(
			icons[i][XPOS],
			icons[i][YPOS],
			flakes[i%(sizeof(flakes)/sizeof(*flakes))],
			FLAKE_WIDTH,
			FLAKE_HEIGHT,
			SSD1306_WHITE
		);
	//	Serial.printf("%2d %3d %3d %2d\n", i,
	//		icons[i][XPOS], icons[i][YPOS], icons[i][DELTAY]
	//	);
		continue;
	}
	display.display();
	return;
}
