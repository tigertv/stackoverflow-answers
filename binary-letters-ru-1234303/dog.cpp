//#include <FastLED.h>
#include <iostream>

#define NUM_LEDS 240

#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define DATA_PIN 4
#define BRIGHTNESS 20

//CRGB leds[NUM_LEDS];
int leds[NUM_LEDS];

#define LETTER_WIDTH 5
#define LETTER_HEIGHT 7
#define LETTER_MARGIN 1
#define LED_SCREEN_WIDTH 30
#define LED_SCREEN_HEIGHT 7

char alphabet[][LETTER_HEIGHT] = {
	{0x04,0x0A,0x11,0x11,0x1F,0x11,0x11},  // A
	{0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E},  // B
	{0x0E,0x11,0x10,0x10,0x10,0x11,0x0E},  // C
	{0x1C,0x12,0x11,0x11,0x11,0x12,0x1C},  // D
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // E
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // F
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // G
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // H
	{0x0E,0x04,0x04,0x04,0x04,0x04,0x0E},  // I
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // J
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // K
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // L
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // M
};

//const char mytext[] = {'a', 'b', 'c'};
const char mytext[] = {'A', 'B', 'I', 'D', 'C', 'B', 'A', 'D'};
const int maskWidth = sizeof(mytext) * (LETTER_WIDTH + LETTER_MARGIN);
int mask[LETTER_HEIGHT][maskWidth];

void prepareMask() {
	for (int i = 0; i < (int)sizeof(mytext); ++i) {
		int ch = mytext[i] - 'A';
		int x = i * (LETTER_WIDTH + LETTER_MARGIN);
		for (int j = 0; j < LETTER_HEIGHT; ++j) {
			char c = alphabet[ch][j];
			for (int k = 0; k < LETTER_WIDTH; ++k) {
				mask[j][x + LETTER_WIDTH - k - 1] = c & 1;
				c >>= 1;
			}
		}
		// right margin
		for (int k = 0; k < LETTER_MARGIN; ++k) {
			for (int j = 0; j < LETTER_HEIGHT; ++j) {
				mask[j][x + LETTER_WIDTH + k] = 0;
			}
		}
	}
}


void setup() {
	//FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering
	//FastLED.setBrightness(BRIGHTNESS);

	prepareMask();
}


void maskToLeds(int shift) {
	/*
	for (int j = 0; j < LED_SCREEN_HEIGHT; j += 2) {
		int jj = j * LED_SCREEN_WIDTH;
		for (int i = 0; i < LED_SCREEN_WIDTH; ++i) {
			int ii = (i + shift) % maskWidth;
			leds[jj + i] = mask[j][ii];
		}
	}
	//*/
	for (int j = 0; j < LED_SCREEN_HEIGHT; j += 2) {
		int jj = j * LED_SCREEN_WIDTH;
		for (int i = 0; i < LED_SCREEN_WIDTH; ++i) {
			int ii = (i + shift) % maskWidth;
			leds[jj + i] = mask[j][ii];// ? '1' : '0';
		}
	}

	for (int j = 1; j < LED_SCREEN_HEIGHT; j += 2) {
		int jj = (j + 1) * LED_SCREEN_WIDTH - 1;
		for (int i = 0; i < LED_SCREEN_WIDTH; ++i) {
			int ii = (i + shift) % maskWidth;
			leds[jj - i] = mask[j][ii];// ? '1' : '0';
		}
	}

}

//*
void showMask() {
	// output
	for (int j = 0; j < LETTER_HEIGHT; ++j) {
		for (int i = 0; i < maskWidth; ++i) {
			std::cout << mask[j][i] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
//*/

void showLeds() {
	// output
	for (int j = 0; j < LED_SCREEN_HEIGHT; ++j) {
		int rowj;
		if (j & 1) {
			rowj = (j + 1) * LED_SCREEN_WIDTH - 1;
		} else {
			rowj = j * LED_SCREEN_WIDTH;
		}
		for (int i = 0; i < LED_SCREEN_WIDTH; ++i) {
			int ii;
			if (j & 1) {
				ii = rowj - i;	
			} else {
				ii = rowj + i;	
			} 

			int backColor = 1;

			if (leds[ii]) {
				backColor = 2;
			}

			//int foreColor = 4;
			//std::cout << "\x1b[38:5:" << foreColor << "m";
			std::cout << "\x1b[48:5:"<< backColor << "m";
			std::cout << leds[ii] << " ";
			//std::cout << "\x1b[m";
			std::cout << "\x1b[m";// << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int shift = 0;

void loop()  {
	maskToLeds(shift);
	showLeds();
	//FastLED.show();
	++shift;
	if (shift == maskWidth) shift = 0;
	//delay(100);
}

int main() {
	setup();
	for (int i = 0; i < 1; ++i) {
		loop();
	}
	//showMask();
}

