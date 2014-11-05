#ifndef _PROJECTION_H_
#define _PROJECTION_H_

#include "Matrix4d.h"

class Projection{

public:
	static Matrix4d perspective(double fovy, double aspect, double zNear, double zFar);
	static Matrix4d frustum(double left, double right, double bottom, double top, double nearVal, double farVal);
	static Matrix4d viewport(int x, int y, int w, int height);
};


#endif