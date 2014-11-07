#ifdef _WIN32
#include <windows.h>
#endif
#include "Window.h"
#include "Projection.h"
#include "parser.h"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;


int Window::width = 512;
int Window::height = 512;
int Window::limit = width * height * 3 - 2;
bool Window::lightEnable = false;
bool Window::zbufferEnable = false;
bool Window::adjustPointEnable = false;
bool Window::sphericalPointEnable = false;
bool Window::fakeDiffuseEnable = false;
PointLight Window::pl = { Color{ Vector3d(400.0, 400.0, 280.0) }, Vector3d(0, 10, -10) };
int old_x, old_y;
bool pressed;
Matrix4d Window::viewport;
Matrix4d Window::projection;
Camera Window::camera(Vector3d(0,0,20), Vector3d(0,0,0), Vector3d(0,1,0));
Matrix4d Window::model;
Matrix4d Window::rotateY;
Matrix4d Window::scaling;
Matrix4d temp;

vector<double> vertices;
vector<Vector3d> normals;

const int font = (int)GLUT_BITMAP_9_BY_15;

double angle;

float* Window::pixels = new float[width * height * 3];
float* Window::zbuffer = new float[(width + 1) * (height + 1)];

vector<double> bunnyPos;
vector<double> dragonPos;
Matrix4d tran_bunny;
Matrix4d tran_dragon;
Matrix4d scale_bunny;
Matrix4d scale_dragon;
double bunny_xmin, bunny_xmax, bunny_ymin, bunny_ymax, bunny_zmin, bunny_zmax, dragon_xmin, dragon_xmax, dragon_ymin, dragon_ymax, dragon_zmin, dragon_zmax;
vector<Vector3d> bunnyNor;
vector<Vector3d> dragonNor;
bool bunny = true;



void Window::loadFiles(){
	cout << "start parsing bunny..." << endl;
	Parser::parse("bunny.xyz", bunnyPos, bunnyNor, bunny_xmin, bunny_xmax, bunny_ymin, bunny_ymax, bunny_zmin, bunny_zmax);
	tran_bunny.makeTranslate(-(bunny_xmin + bunny_xmax) / 2, -(bunny_ymin + bunny_ymax) / 2, -(bunny_zmin + bunny_zmax) / 2);
	tran_bunny.print("translation matrix for bunny:");
	cout << "bunny is loaded." << endl;
	cout << "start parsing dragon..." << endl;
	Parser::parse("dragon.xyz", dragonPos, dragonNor, dragon_xmin, dragon_xmax, dragon_ymin, dragon_ymax, dragon_zmin, dragon_zmax);
	tran_dragon.makeTranslate(-(dragon_xmin + dragon_xmax) / 2, -(dragon_ymin + dragon_ymax) / 2, -(dragon_zmin + dragon_zmax) / 2);
	tran_dragon.print("translation matrix for dragon:");
	cout << "dragon is loaded." << endl;
}

int Window::getWidth(){
	return width;
}

int Window::getHeight(){
	return height;
}

void Window::clearBuffer(){
	Color clearColor = { Vector3d(0.0, 0.0, 0.0) };   // clear color: black
	for (int i = 0; i < width*height; ++i)
	{
		pixels[i * 3] = clearColor.rgb[0];
		pixels[i * 3 + 1] = clearColor.rgb[1];
		pixels[i * 3 + 2] = clearColor.rgb[2];
	}
	int size = (width + 1) * (height + 1);
	for (int i = 0; i < size; i++){
		zbuffer[i] = 10;
	}
	vertices.clear();
	normals.clear();
}

float Window::getZ(int x, int y){
	return zbuffer[y * width + x];
}

bool Window::setZ(int x, int y, double z){
	if (x < 0 || x > width || y < 0 || y > height)
		return false;
	if (z < getZ(x, y)){
		zbuffer[y * width + x] = z;
		return true;
	}
	else
		return false;
}

void Window::writePoint(double& x, double& y, double& z){
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

void Window::writeNormal(Vector3d& n){
	normals.push_back(n);
}

// Draw a point into the frame buffer
void Window::drawPoint(int x, int y, double z, float r, float g, float b)
{
	if (adjustPointEnable){
		int size = 1;
		
		if (z < 0.96)
			size++;
		if (z < 0.95)
			size++;
		if (z < 0.94)
			size++;
		if (z < 0.93)
			size++;

		int right = size / 2;
		int left = right - (size - 1) % 2;
		int startX = x - left;
		int startY = y - left;
		int* offsets = new int[size * size];
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				int offset = startY * width * 3 + startX * 3;
				if (offset <= limit && offset >= 0){
					if (zbufferEnable){
						if (setZ(startX, startY, z)){
							pixels[offset] = r;
							pixels[offset + 1] = g;
							pixels[offset + 2] = b;
						}
					}
					else{
						pixels[offset] = r;
						pixels[offset + 1] = g;
						pixels[offset + 2] = b;
					}
					startX++;
				}
			}
			startX = x - left;
			startY++;
		}
	}
	else{
		int offset = y*width * 3 + x * 3;
		if (offset <= limit && offset >= 0){
			pixels[offset] = r;
			pixels[offset + 1] = g;
			pixels[offset + 2] = b;
		}
	}
}


void Window::drawPoint(int x, int y, float r, float g, float b){
	int offset = y*width * 3 + x * 3;
	if (offset <= limit && offset >= 0){
		pixels[offset] = r;
		pixels[offset + 1] = g;
		pixels[offset + 2] = b;
	}
}

void Window::drawSphericalPoint(int x0, int y0, double z, float r, float g, float b, Vector3d normalPoint, Vector3d normalLight){
	// mid-point algorithm
	double _r = r;
	double _g = g;
	double _b = b;
	int ri = 1;
	if (z < 0.96)
		ri++;
	if (z < 0.95)
		ri++;
	if (z < 0.94)
		ri++;
	if (z < 0.93)
		ri++;
	
	int x = ri;
	int y = 0;
	int radiusError = 1 - x;
	while (ri > 0){
		while (x >= y){
			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, x + x0, y + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(x + x0, y + y0, z))
					drawPoint(x + x0, y + y0, _r, _g, _b);
			}
			else{
				drawPoint(x + x0, y + y0, _r, _g, _b);
			}

			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, y + x0, x + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(y + x0, x + y0, z))
					drawPoint(y + x0, x + y0, _r, _g, _b);
			}
			else
			{
				drawPoint(y + x0, x + y0, _r, _g, _b);
			}

			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, -x + x0, y + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(-x + x0, y + y0, z))
					drawPoint(-x + x0, y + y0, _r, _g, _b);
			}else
				drawPoint(-x + x0, y + y0, _r, _g, _b);

			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, -y + x0, x + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(-y + x0, x + y0, z)){
					drawPoint(-y + x0, x + y0, _r, _g, _b);
				}
			}else
				drawPoint(-y + x0, x + y0, _r, _g, _b);
			
			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, -x + x0, -y + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(-x + x0, -y + y0, z))
					drawPoint(-x + x0, -y + y0, _r, _g, _b);
			}else
				drawPoint(-x + x0, -y + y0, _r, _g, _b);
			
			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, -y + x0, -x + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(-y + x0, -x + y0, z))
					drawPoint(-y + x0, -x + y0, _r, _g, _b);
			}else
				drawPoint(-y + x0, -x + y0, _r, _g, _b);
			
			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, x + x0, -y + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(x + x0, -y + y0, z))
					drawPoint(x + x0, -y + y0, _r, _g, _b);
			}else
				drawPoint(x + x0, -y + y0, _r, _g, _b);
			
			if (fakeDiffuseEnable)
				calculateFakeDiffuse(x0, y0, ri, y + x0, -x + y0, normalPoint, normalLight, _r, _g, _b);
			if (zbufferEnable){
				if (setZ(y + x0, -x + y0, z))
					drawPoint(y + x0, -x + y0, _r, _g, _b);
			}else
				drawPoint(y + x0, -x + y0, _r, _g, _b);
			
			y++;
			if (radiusError < 0)
			{
				radiusError += 2 * y + 1;
			}
			else
			{
				x--;
				radiusError += 2 * (y - x + 1);
			}
		}
		ri--;
		x = ri;
		y = 0;
		radiusError = 1 - x;
	}
	if (zbufferEnable){
		if (setZ(x0, y0, z))
			drawPoint(x0, y0, r, g, b);
	}else
		drawPoint(x0, y0, r, g, b);
	
}

void Window::calculateFakeDiffuse(int x0, int y0, int radius, int x1, int y1, Vector3d normalPoint, Vector3d normalLight, double& r, double& g, double& b){
	if (r <= 0 && g <= 0 && b <= 0)
		return;
	Vector3d n(x1 - x0, y1 - y0, (sqrt(radius*radius + (y1 - y0) * (y1 - y0))));
	normalPoint.scale(radius);
	n.add(normalPoint);
	n.normalize();
	double factor = n.dot(normalLight);
	r *= factor;
	g *= factor;
	b *= factor;
}

void Window::rasterize()
{
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
	Vector3d rgb(1.0, 1.0, 1.0);
	Matrix4d m(model);
	for (int i = 0; i < normals.size(); i++){
		Vector4d v4(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1);
		v4 = m * v4;
		Vector4d n(normals[i][0], normals[i][1], normals[i][2], 0);
		n = m * n;
		Vector3d v3 = v4.getVector3d();
		Vector3d l = pl.pos - v3;				// light position - position of the point
		if (lightEnable){
			/*
			double ld = 0.1 * l.magnitude();
			double qd = 0.1 * ld * ld;
			rgb.set(0, pl.c.rgb[0] / (1 + ld + qd));
			rgb.set(1, pl.c.rgb[1] / (1 + ld + qd));
			rgb.set(2, pl.c.rgb[2] / (1 + ld + qd));
			l.normalize();
			//double reflect = normals[i].dot(l);
			Vector3d n3 = n.getVector3d();
			n3.normalize();
			double reflect = n3.dot(l);
			reflect = max(0, reflect);
			//cout << reflect << endl;
			rgb.scale(reflect);
			*/
			Vector3d Li = pl.c.rgb;
			double d = l.magnitude();
			Li.scale(1 / (d * d));
			l.normalize();
			Vector3d n3 = n.getVector3d();
			n3.normalize();
			double factor = 1 / M_PI * (n3.dot(l));
			Li.scale(factor);
			rgb = Li;
		}
		//v4 = camera.getMatrix() * v4;
		v4 = projection * v4;
		v4.dehomogenize();
		if (v4[0] < -1 || v4[0] > 1 || v4[1] < -1 || v4[1] > 1 || v4[2] < -1 || v4[2] > 1){
			continue;
		}
		v4 = viewport * v4;
		v4.dehomogenize();
		int x = v4[0] + 0.5;
		int y = v4[1] + 0.5;
		if (sphericalPointEnable)
			drawSphericalPoint(x, y, v4[2], rgb[0], rgb[1], rgb[2], n.getVector3d().normalize(), l);
		else
			drawPoint(x, y, v4[2], rgb[0], rgb[1], rgb[2]);
		/*
		if (zbufferEnable){
			
			if (v4[2] < getZ(x, y)){
				setZ(x, y, v4[2]);
				
			}
		}
		else{
			int x = v4[0] + 0.5;
			int y = v4[1] + 0.5;
			if (sphericalPointEnable)
				drawSphericalPoint(x, y, v4[2], rgb[0], rgb[1], rgb[2], n.getVector3d().normalize(), l);
			else
				drawPoint(x, y, v4[2], rgb[0], rgb[1], rgb[2]);
		}
		*/

	}
	
}

// Called whenever the window size changes
void Window::reshapeCallback(int new_width, int new_height)
{
	width = new_width;
	height = new_height;
	limit = width * height * 3 - 2;
	delete[] pixels;
	delete[] zbuffer;
	pixels = new float[width * height * 3];
	zbuffer = new float[(width + 1) * (height + 1)];
	viewport = Projection::viewport(0, 0, width, height);
	viewport.print("view port");
	projection = Projection::perspective(60, double(width) / double(height), 1.0, 1000.0);
	projection.print("projection");
	//Matrix4d t;
	//t.makeTranslate(0, 0, -20);
	//projection = projection * t;


	// calculate scaling matrix for bunny
	double x = (bunny_xmax - bunny_xmin) / 2;
	double y = (bunny_ymax - bunny_ymin) / 2;
	double z = (bunny_zmax - bunny_zmin) / 2;

	double a = z / y;
	double ymax = 20 / (a + 1 / tan(30 * M_PI / 180));
	double sy = ymax / y;					// scaling factor according to y

	double xmax = ymax * (double)width / (double)height; //scaling factor according to x
	double sx = xmax / x;

	double min = sy < sx ? sy : sx;
	scale_bunny.makeScale(min, min, min);

	// calculate scaling matrix for dragon
	x = (dragon_xmax - dragon_xmin) / 2;
	y = (dragon_ymax - dragon_ymin) / 2;
	z = (dragon_zmax - dragon_zmin) / 2;

	a = z / y;
	ymax = 20 / (a + 1 / tan(30 * M_PI / 180));
	xmax = ymax * (double)width / (double)height;
	sy = ymax / y;
	sx = xmax / x;
	min = sy < sx ? sy : sx;
	scale_dragon.makeScale(min, min, min);
	//scale_dragon.print("scaling matrix for dragon:");

}

void Window::idleCallback()
{
	displayCallback();         // call display routine to show the cube
}

void Window::keyboardCallback(unsigned char key, int, int)
{
	switch (key){
	case '1':
		bunny = true;
		break;
	case '2':
		bunny = false;
		break;
	case 's':
		temp.makeScale(0.9, 0.9, 0.9);
		scaling.multiply(temp);
		break;
	case 'S':
		temp.makeScale(1.1, 1.1, 1.1);
		scaling.multiply(temp);
		break;
	case 27:
		exit(0);
	}
}

void Window::processSpecialKeys(int k, int x, int y){
	switch (k){
	case GLUT_KEY_F1:
		lightSwitch(false);
		break;
	case GLUT_KEY_F2:
		lightSwitch(true);
		break;
	case GLUT_KEY_F3:
		zbufferSwitch(!zbufferEnable);
		break;
	case GLUT_KEY_F4:
		adjustPointSwitch(!adjustPointEnable);
		break;
	case GLUT_KEY_F5:
		sphericalPointSwitch(!sphericalPointEnable);
		break;
	case GLUT_KEY_F6:
		fakeDiffuseSwitch(!fakeDiffuseEnable);
		break;
	}
}

void Window::displayCallback()
{
	clearBuffer();
	if (bunny){
		model = camera.getMatrix() * scaling * rotateY * scale_bunny * tran_bunny;
		for (int i = 0; i < bunnyNor.size(); i++){
			writeNormal(bunnyNor[i]);
			writePoint(bunnyPos[i * 3], bunnyPos[i * 3 + 1], bunnyPos[i * 3 + 2]);
		}
	}
	else{
		model = camera.getMatrix() * scaling * rotateY * scale_dragon * tran_dragon;
		for (int i = 0; i < dragonNor.size(); i++){
			writeNormal(dragonNor[i]);
			writePoint(dragonPos[i * 3], dragonPos[i * 3 + 1], dragonPos[i * 3 + 2]);
		}
	}

	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
	glutSwapBuffers();
}

void Window::mouseMotionProcess(int x, int y){
	int dx = x - old_x;
	int dy = old_y - y;
	old_x = x;
	old_y = y;
	if (pressed){
		angle += dx;
		if (angle < 0)
			angle = 360;
		if (angle > 360)
			angle = 0;
		rotateY.makeRotateY(angle);
	}
}

void Window::mousePassiveMotionProcess(int x, int y){
	old_x = x;
	old_y = y;
}

void Window::mouseProcess(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		pressed = true;
	}
	else{
		pressed = false;
	}
}

void Window::lightSwitch(bool b){
	lightEnable = b;
}
void Window::adjustPointSwitch(bool b){
	adjustPointEnable = b;
}
void Window::setLight(PointLight p){
	pl = p;
}
void Window::zbufferSwitch(bool b){
	zbufferEnable = b;
}
void Window::sphericalPointSwitch(bool b){
	sphericalPointEnable = b;
}
void Window::fakeDiffuseSwitch(bool b){
	fakeDiffuseEnable = b;
}

void Window::renderBitmapString(float x, float y, void *font, const char *string){
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}