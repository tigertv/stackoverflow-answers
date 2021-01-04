#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>


class Screen {
private:
	int height = 30;
	int width = 100;
	std::vector<uint32_t> chars;

public:
	Screen(int width, int height) : chars(height * width, (int)((1 << 8) | ' ') ) {}
	int getWidth() { return width; }
	int getHeight() { return height; }

	void show() {
		for (int i = 0; i < height; ++i) {
			for(int j = 0; j < width; ++j) {
				// unpack char
				int backColor = (chars[i * width + j] >> 8) & 0xff;
				int foreColor = (chars[i * width + j] >> 16) & 0xff;
				int effect = (chars[i * width + j] >> 24) & 0xff;
				char c = chars[i * width + j] & 0xff;

				std::cout << "\x1b[38:5:" << foreColor << "m";
				std::cout << "\x1b[48:5:"<< backColor << "m";
				std::cout << "\x1b["<< effect << "m" << c;
				std::cout << "\x1b[m" << "\x1b[m" << "\x1b[m";
			}
			std::cout << std::endl;
		}
	}

	void setCharAtXY(int x, int y, char value, uint8_t backColor = 16, uint8_t foreColor = 0, uint8_t effect = 1) {
		chars[x + y * width] = value | (backColor << 8) | (foreColor << 16) | (effect << 24);
	}

	void setCharAtXY(int x, int y, uint32_t packedChar) {
		chars[x + y * width] = packedChar;
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

struct IScreenable {
	virtual void draw(Screen& screen) = 0;
};

class Tree : public IScreenable {
private:
	int x;
	int y;
	int height;

public:
	Tree (int x, int y, int height) : x(x), y(y), height(height) {}

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }

	void draw(Screen& screen) override {
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

class Ground : public IScreenable {
private:
	int height = 5;
public:
	void draw(Screen& screen) override {
		int sHeight = screen.getHeight();
		int sWidth = screen.getWidth();
		for (int i = sHeight - height; i < sHeight; ++i) {
			for(int j = 0; j < sWidth; ++j) {
				screen.setCharAtXY(j, i, ' ', 15);
			}
		}
	}
};

class BigText : public IScreenable {
private:
	int x = 11;
	int y = 3;
	int color = 15;
public:
	void draw(Screen& screen) override {
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

class DedMoroz : public IScreenable {
private:
	int x;
	int y;

public:
	DedMoroz(int x, int y) : x(x), y(y) {}
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	int getX() { return x; }
	int getY() { return y; }
	void goLeft() { --x; }
	void goRight() { ++x; }

	void draw(Screen& screen) override {
		const int width = 12;
		uint32_t points[][width] = {
			{0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x0106df20, 0x0100df4f, 0x0106df20, 0x0100df4f, 0x0106df20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x0106df20, 0x0106df20, 0x0106df22, 0x0106df20, 0x0106df20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x0106df20, 0x0106df20, 0x0106df7e, 0x0106df20, 0x0106df20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x0106a020, 0x0106a020, 0x01060f20, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106df20, 0, 0, 0, 0, 0,}, 
			{0x0106a020, 0x0106a020, 0x01060f20, 0x0106a020, 0x0106a020, 0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, }, 
			{0x0106a020, 0x0106a020, 0x01060f20, 0x0106a020, 0x0106a020, 0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, }, 
			{0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, }, 
			{0x01000020, 0x01000020, 0x00000000, 0x01000020, 0x01000020, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, }, 

		};

		int height = (int)(sizeof(points) / sizeof(points[0]));

		for (int i = 0; i < height ; ++i) {
			for (int j = 0; j < width; ++j) {
				if (points[i][j]) {
					screen.setCharAtXY(x + j, y + i, points[i][j]);
				}

			}
		}
	}
};

int main() {
	Screen screen(100, 30);
	std::vector<IScreenable*> objects;

	Ground ground; objects.push_back(&ground);
	Tree tree(50, 8, 20); objects.push_back(&tree);
	BigText text; objects.push_back(&text);
	DedMoroz santa(0, 18); objects.push_back(&santa);


	bool isPlaying = true;	
	while(isPlaying ) {
		screen.clear();
		santa.goRight();
		if (santa.getX() == 50) isPlaying = false;

		for (IScreenable* const obj : objects) {
			obj->draw(screen);
		}

		screen.show();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
