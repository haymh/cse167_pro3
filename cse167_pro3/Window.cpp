#ifdef _WIN32
#include <windows.h>
#endif
#include "Window.h"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

int Window::width = 512;
int Window::height = 512;

float* Window::pixels = new float[width * height * 3];

int Window::getWidth(){
	return width;
}

int Window::getHeight(){
	return height;
}

void Window::clearBuffer(){
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	for (int i = 0; i<width*height; ++i)
	{
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
	}
}

// Draw a point into the frame buffer
void Window::drawPoint(int x, int y, float r, float g, float b)
{
	int offset = y*width * 3 + x * 3;
	pixels[offset] = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
}

void Window::rasterize()
{
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
}

// Called whenever the window size changes
void Window::reshapeCallback(int new_width, int new_height)
{
	width = new_width;
	height = new_height;
	delete[] pixels;
	pixels = new float[width * height * 3];
	displayCallback();
}

void Window::keyboardCallback(unsigned char key, int, int)
{
	
}

void Window::displayCallback()
{
	clearBuffer();
	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

	glutSwapBuffers();
}