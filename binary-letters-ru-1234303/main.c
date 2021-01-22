#include <iostream>
#include <map>

#define LETTER_WIDTH 5
#define LETTER_HEIGHT 7
#define LETTER_MARGIN 1

int main() {

	char alphabet[][LETTER_HEIGHT] = {
		{0x04,0x0A,0x11,0x11,0x1F,0x11,0x11}, // A
		{0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}, // B
		{0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}, // C
		{0x1C,0x12,0x11,0x11,0x11,0x12,0x1C}, // D
		{0x11,0x11,0x11,0x13,0x15,0x19,0x11}, // И
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // empty
	};
	std::map<int, int> ma = { {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'И', 4}, {' ', 5} };

	const int mytext[] = {'A', 'B', ' ', 'И', ' ',  'C', 'D'};
	int mytextSize = sizeof(mytext) / sizeof(mytext[0]);

	int outa[LETTER_HEIGHT][mytextSize * (LETTER_WIDTH + LETTER_MARGIN)];

	for (int i = 0; i < mytextSize; ++i) {
		int ch = ma[mytext[i]];
		int x = i * (LETTER_WIDTH + LETTER_MARGIN);
		for (int j = 0; j < LETTER_HEIGHT; ++j) {
			char c = alphabet[ch][j];
			for (int k = 0; k < LETTER_WIDTH; ++k) {
				outa[j][x + LETTER_WIDTH - k - 1] = c & 1;
				c >>= 1;
			}
		}
		// right margin
		for (int k = 0; k < LETTER_MARGIN; ++k) {
			for (int j = 0; j < LETTER_HEIGHT; ++j) {
				outa[j][x + LETTER_WIDTH + k] = 0;
			}
		}
	}

	// output
	for (int j = 0; j < LETTER_HEIGHT; ++j) {
		for (int i = 0; i < mytextSize * (LETTER_WIDTH + LETTER_MARGIN); ++i) {
			std::cout << outa[j][i] << " ";
		}
		std::cout << std::endl;
	}

}
