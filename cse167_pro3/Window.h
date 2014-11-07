#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Matrix4d.h"
#include "Camera.h"
#include <vector>

struct Color{
	Vector3d rgb;
};

struct PointLight{
	Color c;
	Vector3d pos;
};

class Window
{
public:
	static int getWidth();
	static int getHeight();
	static void clearBuffer();
	static void drawPoint(int x, int y, double z, float r, float g, float b);
	static void rasterize();
	static void reshapeCallback(int w, int h);
	static void idleCallback();
	static void keyboardCallback(unsigned char key, int, int);
	static void processSpecialKeys(int k, int x, int y);
	static void displayCallback();
	static void drawCube();
	static void loadCloudPoint();
	static void loadFiles();
	static void mouseMotionProcess(int, int);
	static void mousePassiveMotionProcess(int, int);
	static void mouseProcess(int, int, int, int);
	
	

private:
	static int width;
	static int height;
	static int limit;			// limit of range of pixels
	static float* pixels;
	static float* zbuffer;
	static Matrix4d viewport;
	static Matrix4d projection;
	static Camera camera;
	static Matrix4d model;
	static Matrix4d scaling;
	static Matrix4d rotateY;
	static bool lightEnable;
	static bool zbufferEnable;
	static bool adjustPointEnable;
	static bool sphericalPointEnable;
	static bool fakeDiffuseEnable;
	static PointLight pl;

	static void writePoint(double&, double&, double&);
	static void writeNormal(Vector3d &);
	static void lightSwitch(bool);
	static void zbufferSwitch(bool);
	static void adjustPointSwitch(bool);
	static void sphericalPointSwitch(bool b);
	static void fakeDiffuseSwitch(bool b);
	static void setLight(PointLight);
	static float getZ(int x, int y);
	static void setZ(int x, int y, int z);
	static void drawSphericalPoint(int x, int y, double z, float r, float g, float b, Vector3d normalPoint, Vector3d normalLight);
	static void calculateFakeDiffuse(int x0, int y0, int radius, int x1, int y1, Vector3d normalPoint, Vector3d normalLight, double& r, double& g, double& b);
	static void drawPoint(int x, int y, float r, float g, float b);
	static void renderBitmapString(float x, float y, void *font, const char *string);


	


};

#endif
