#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Matrix4d.h"
#include "Vector3d.h"


class Camera{
protected:
	Vector3d up;			// up-vector
	Vector3d e;				// projection
	Vector3d d;				// look at
	Matrix4d m;

	void computeMatrix();
	void setCamera(double eyeX, double eyeY, double eyeZ,
		double centerX, double centerY, double centerZ,
		double upX, double upY, double upZ);

public:
	Camera(Vector3d e, Vector3d d, Vector3d up);
	Camera(double eyeX, double eyeY, double eyeZ,
		double centerX, double centerY, double centerZ,
		double upX, double upY, double upZ);
	Matrix4d getMatrix();
	


};

#endif