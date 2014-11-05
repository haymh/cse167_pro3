#include "Projection.h"
#include <math.h>
#include <iostream>


using namespace std;

Matrix4d Projection::perspective(double fovy, double aspect, double zNear, double zFar){
	/*
	Matrix4d m;
	m.identity();
	m.set(0, 0, 1 / (aspect * tan(fovy / 2)));
	m.set(1, 1, 1 / tan(fovy / 2));
	m.set(2, 2, (zNear + zFar) / (zNear - zFar));
	m.set(3, 3, 0);
	m.set(2, 3, -1);
	m.set(3, 2, 2 * zNear * zFar / (zNear - zFar));
	return m;
	*/
	///*
	double ymax, xmax;
	ymax = zNear * tan(fovy * M_PI / 360.0);
	xmax = ymax * aspect;
	return frustum(-xmax, xmax, -ymax, ymax, zNear, zFar);
	//*/
}

Matrix4d Projection::frustum(double left, double right, double bottom, double top, double nearVal, double farVal){
	Matrix4d m;
	m.identity();
	m.set(0, 0, 2 * nearVal / (right - left));
	m.set(1, 1, 2 * nearVal / (top - bottom));
	m.set(2, 2, -(nearVal + farVal) / (farVal - nearVal));
	m.set(3, 3, 0);
	m.set(2, 0, (right + left) / (right - left));
	m.set(2, 1, (top + bottom) / (top - bottom));
	m.set(2, 3, -1);
	m.set(3, 2, -2 * farVal * nearVal / (farVal - nearVal)); // TODO: does -2 matter??
	//m.set(3, 2, -2 * farVal * nearVal / (farVal - nearVal));
	return m;
}

Matrix4d Projection::viewport(int x, int y, int w, int h){
	
	Matrix4d m;
	m.identity();
	m.set(0, 0, double(w) / 2);
	m.set(1, 1, double(h) / 2);
	m.set(2, 2, 0.5);
	m.set(3, 0, (2 * double(x) + double(w)) / 2);
	m.set(3, 1, (2 * double(y) + double(h)) / 2);
	m.set(3, 2, 0.5);
	//cout << "x: " << x << " y: " << y << " w: " << w << " h: " << h << endl;
	//m.print("viewport");
	return m;
	
	
}