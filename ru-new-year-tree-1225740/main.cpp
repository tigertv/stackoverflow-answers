#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>


class Screen {
private:
	int height = 30;
	int width = 100;
	std::vector<uint32_t> chars;
	std::vector<uint32_t> backgroundScreen;

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

	uint32_t getCharAtXY(int x, int y) {
		return chars[x + y * width];
	}

	void clear() {

		if (backgroundScreen.size()) {
			chars = backgroundScreen;
		} else {
			for (int i = 0; i < height; ++i) {
				for(int j = 0; j < width; ++j) {
					setCharAtXY(j, i, 0x01006920U);
				}
			}
		}

		std::cout << "\x1b[2J\x1b[H";
	}

	void save() {
		backgroundScreen = chars;
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
			screen.setCharAtXY(xx + k, y + i, 0x05c4026fU);
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
	bool isSmiling = false;

public:
	DedMoroz(int x, int y) : x(x), y(y) {}
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	int getX() { return x; }
	int getY() { return y; }
	void goLeft() { --x; }
	void goRight() { ++x; (x & 1) ? --y : ++y;}
	void smile() {
		isSmiling = true;
	}

	void draw(Screen& screen) override {
		const int width = 13;
		uint32_t points[][width] = {
			{0x00000000, 0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x00000000, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x00000000, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x0106df28, 0x0106df20, 0x0100df4f, 0x0106df20, 0x0100df4f, 0x0106df20, 0x0106df29, 0, 0, 0, 0, 0, 0,}, 
			{0x00000000, 0x0106df20, 0x0106df20, 0x0106df22, 0x0106df20, 0x0106df20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x00000000, 0x0106df20, 0x0106df20, 0x0106df7e, 0x0106df20, 0x0106df20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x00000000, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0, 0, 0, 0, 0, 0, 0,}, 
			{0x00000000, 0x0106a020, 0x0106a020, 0x01060f20, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106df20, 0, 0, 0, 0, 0,}, 
			{0x00000000, 0x0106a020, 0x0106a020, 0x01060f20, 0x0106a020, 0x0106a020, 0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, }, 
			{0x00000000, 0x0106a020, 0x0106a020, 0x01060f20, 0x0106a020, 0x0106a020, 0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, }, 
			{0x00000000, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x01060f20, 0x00000000, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0x0106a020, 0, }, 
			{0x00000000, 0x01000020, 0x01000020, 0x00000000, 0x01000020, 0x01000020, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, }, 

		};

		int height = (int)(sizeof(points) / sizeof(points[0]));

		for (int i = 0; i < height ; ++i) {
			for (int j = 0; j < width; ++j) {
				if (points[i][j]) {
					screen.setCharAtXY(x + j, y + i, points[i][j]);
				}

			}
		}

		if (isSmiling) {
			uint32_t points2[][5] = {
				{0x0, 0x00000000, 0x00000000, 0x00000000, 0x0}, 
				{0x0, 0x0106df5c, 0x0106df5f, 0x0106df2f, 0x0}, 
			};
			int xx = x + 1;
			int yy = y + 4;
			for (int i = 0; i < 2; ++i) {
				for (int j = 0; j < 5; ++j) {
					if (points2[i][j]) {
						screen.setCharAtXY(xx + j, yy + i, points2[i][j]);
					}

				}
			}
		}
	}
};

class SnowFall: public IScreenable {
private:
	int y;
	int x;
	int snowInRow = 3;
	const uint32_t packedSnowFlake = 0x010f002a;
	std::vector<std::vector<int>> points;

	void step() {
		if (++y >= 30) y = 0;
		if (++x >= 100) x = 0;
	}

public:
	SnowFall() {
		y = 0;
		x = 0;
		int height = 14; 
		int sWidth = 100;
		std::srand(std::time(nullptr));
		for(int i = 0; i < height; ++i) {
			std::vector<int> a;
			for(int j = 0; j < snowInRow; ++j) {
				a.push_back(std::rand() % sWidth);	
			}
			points.push_back(a);
		}
	}

	void setY(int y) { this->y = y; }
	int getY() { return y; }

	void draw(Screen& screen) override {
		int height = points.size();

		int sHeight = screen.getHeight();
		int sWidth = screen.getWidth();

		for (int i = 0; i < height ; ++i) {
			for (int j = 0; j < snowInRow; ++j) {
				int yy = y + i * 2;
				int xx = x + points[i][j];
				if (yy >= sHeight) yy -= sHeight;
				if (xx >= sWidth) xx -= sWidth;

				uint32_t c = screen.getCharAtXY(xx, yy);
				c &= 0x0000ff00; // current background
				c |= packedSnowFlake; // change background for a snowflake

				screen.setCharAtXY(xx, yy, c);
			}
		}

		step();
	}
};

class Window: public IScreenable {
private:
	int x = 60;
	int y = 15;
	int width = 35;
	int height = 7;
	int color = 195;
public:
	void draw(Screen& screen) override {

		// vertical borders
		for (int i = 0; i < height ; ++i) {
			screen.setCharAtXY(x, y + i, ' ', color);
			screen.setCharAtXY(x + width - 1, y + i, ' ', color);
		}

		// horizontal borders
		for (int i = 1; i < width - 1; ++i) {
			screen.setCharAtXY(x + i, y, ' ', color);
			screen.setCharAtXY(x + i, y + height - 1, ' ', color);
		}

		const std::string texts[] = {
			"       HO-HO-HO, HO-HO-HO      ",
			"       MERRY CHRISTMAS !!!     ",
			"       HAPPY NEW YEAR !!!      ",
		};

		int xx = x + 2;
		int yy = y + 2;
		int size = sizeof(texts) / sizeof(texts[0]);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < (int)texts[i].size(); ++j) {
				screen.setCharAtXY(xx + j, yy + i, texts[i][j], color);
			}
		}

	}

};

int main() {
	Screen screen(100, 30);
	std::vector<IScreenable*> objects;

	screen.clear();
	// static objects for background can be saved
	Ground ground; ground.draw(screen);
	Tree tree(50, 7, 20); tree.draw(screen);
	screen.save();

	DedMoroz santa(0, 16); objects.push_back(&santa);
	SnowFall snow; objects.push_back(&snow);
	BigText text; 
	Window window; 

	bool isPlaying = true;	

	while(isPlaying) {
		screen.clear();
		santa.goRight(); // go to the tree
		if (santa.getX() == 50) {
			isPlaying = false;
			santa.smile();
			objects.push_back(&text);
			objects.push_back(&window);
		}

		for (IScreenable* const obj : objects) {
			obj->draw(screen);
		}

		screen.show();
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
	}

}
