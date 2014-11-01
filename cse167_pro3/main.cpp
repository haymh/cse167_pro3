#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

static int window_width = 512, window_height = 512;
static float* pixels = new float[window_width * window_height * 3];

using namespace std;

struct Color    // generic color class
{
	float r, g, b;  // red, green, blue
};

void loadData()
{
	// point cloud parser goes here
}

// Clear frame buffer
void clearBuffer()
{
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	for (int i = 0; i<window_width*window_height; ++i)
	{
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
	}
}

// Draw a point into the frame buffer
void drawPoint(int x, int y, float r, float g, float b)
{
	int offset = y*window_width * 3 + x * 3;
	pixels[offset] = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
}

void rasterize()
{
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
}



void keyboardCallback(unsigned char key, int, int)
{
	cerr << "Key pressed: " << key << endl;
}

void displayCallback()
{
	clearBuffer();
	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);

	glutSwapBuffers();
}

// Called whenever the window size changes
void reshapeCallback(int new_width, int new_height)
{
	window_width = new_width;
	window_height = new_height;
	delete[] pixels;
	pixels = new float[window_width * window_height * 3];
	displayCallback();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Rasterizer");

	loadData();

	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMainLoop();
}