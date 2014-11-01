#ifndef _WINDOW_H_
#define _WINDOW_H_

struct Color{
	float r, g, b;
};

class Window
{
public:
	static int getWidth();
	static int getHeight();
	static void clearBuffer();
	static void drawPoint(int x, int y, float r, float g, float b);
	static void rasterize();
	static void reshapeCallback(int w, int h);
	static void keyboardCallback(unsigned char key, int, int);
	static void displayCallback();

private:
	static int width;
	static int height;
	static float* pixels;

};

#endif
