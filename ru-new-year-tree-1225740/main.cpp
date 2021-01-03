#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>


class Screen {
private:
	int height = 30;
	int width = 100;
	std::vector<unsigned int> chars;

public:
	Screen(int width, int height) : chars(height * width, (int)((1 << 8) | ' ') ) {}
	int getWidth() { return width; }
	int getHeight() { return height; }

	void show() {
		for (int i = 0; i < height; ++i) {
			for(int j = 0; j < width; ++j) {
				int backColor = (unsigned int)(chars[i * width + j] >> 8) & 0xff;
				int foreColor = (unsigned int)(chars[i * width + j] >> 16) & 0xff;
				int effect = (unsigned int)(chars[i * width + j] >> 24) & 0xff;
				char c = chars[i * width + j] & 0xff;

				std::cout << "\x1b[38:5:" << foreColor << "m";
				std::cout << "\x1b[48:5:"<< backColor << "m";
				std::cout << "\x1b["<< effect << "m" << c << "\x1b[0m";
				std::cout << "\x1b[m" << "\x1b[m";
			}
			std::cout << std::endl;
		}
	}

	void setCharAtXY(int x, int y, char value, uint8_t backColor = 16, uint8_t foreColor = 0, uint8_t effect = 1) {
		chars[x + y * width] = value | (backColor << 8) | (foreColor << 16) | (effect << 24);
	}

	void clear() {

		for (int i = 0; i < height; ++i) {
			for(int j = 0; j < width; ++j) {
				setCharAtXY(j, i, ' ', 105);
			}
		}

		std::cout << "\x1b[2J\x1b[H";
	}

};

class Tree {
private:
	int x;
	int y;
	int height;

public:
	Tree (int x, int y, int height) : x(x), y(y), height(height) {}

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }

	void draw(Screen& screen) {
		int stemWidth = 5;
		int stemHeight = 4;

		std::srand(std::time(nullptr));

		// needles
		int xx = x;
		int i = 0;
		for ( ; i < height - stemHeight; ++i, --xx) {
			for(int j = 0; j < i * 2 + 1; ++j) {
				screen.setCharAtXY(xx + j, y + i, '*', 2);
			}
			int k = std::rand() % (i * 2 + 1);
			screen.setCharAtXY(xx + k, y + i, 'o', 15, 0xc4, 5);
		}
		// stem
		for (xx = x; i < height; ++i) {
			for(int j = 0; j < stemWidth; ++j) {
				screen.setCharAtXY(xx - stemWidth / 2 + j, y + i, '|', 3);
			}
		}

	}
};

class Ground {
private:
	int height = 5;
public:
	void draw(Screen& screen) {
		int sHeight = screen.getHeight();
		int sWidth = screen.getWidth();
		for (int i = sHeight - height; i < sHeight; ++i) {
			for(int j = 0; j < sWidth; ++j) {
				screen.setCharAtXY(j, i, ' ', 15);
			}
		}
	}
};

class BigText {
private:
	int x = 11;
	int y = 3;
	int color = 15;
public:
	void draw(Screen& screen) {
		const int width = 15;
		int points[][width] = {
			{1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0}, 
			{0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0},
			{1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0},
			{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
			{1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
		};

		int height = (int)(sizeof(points) / sizeof(points[0]));

		for (int i = 0; i < height ; ++i) {
			for (int j = 0; j < width; ++j) {
				if (points[i][j]) {
					screen.setCharAtXY(x + j, y + i, ' ', color);
				}
			}
		}
	}

};

int main() {
	Screen screen(100, 30);
	Tree tree(50, 8, 20);
	Ground ground;
	BigText text;

	screen.clear();
	ground.draw(screen);
	tree.draw(screen);
	text.draw(screen);

	screen.show();
}
