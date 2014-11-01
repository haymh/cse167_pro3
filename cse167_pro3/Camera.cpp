#include "Camera.h"
#include <iostream>

Camera::Camera(Vector3d projection, Vector3d lookat, Vector3d upVector){
	e = projection;
	d = lookat;
	up = upVector;
	computeMatrix();
}
Camera::Camera(double eyeX, double eyeY, double eyeZ,
	double centerX, double centerY, double centerZ,
	double upX, double upY, double upZ){
	setCamera(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void Camera::setCamera(double eyeX, double eyeY, double eyeZ,
	double centerX, double centerY, double centerZ,
	double upX, double upY, double upZ){
	
	double f[3];
	f[0] = centerX - eyeX;
	f[1] = centerY - eyeY;
	f[2] = centerZ - eyeZ;
	double m_f = sqrt(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
	f[0] /= m_f;
	f[1] /= m_f;
	f[2] /= m_f;

	double up[3];
	up[0] = upX;
	up[1] = upY;
	up[2] = upZ;
	double m_u = sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
	up[0] /= m_u;
	up[1] /= m_u;
	up[2] /= m_u;

	double s[3];
	s[0] = f[1] * up[2] - f[2] * up[1];
	s[1] = f[2] * up[0] - f[0] * up[2];
	s[2] = f[0] * up[1] - f[1] * up[0];

	double u[3];
	u[0] = s[1] * f[2] - s[2] * f[1];
	u[1] = s[2] * f[0] - s[0] * f[2];
	u[2] = s[0] * f[1] - s[1] * f[0];

	Matrix4d r;						// inverse rotation
	r.identity();
	r.set(0, 0, s[0]);
	r.set(1, 0, s[1]);
	r.set(2, 0, s[2]);

	r.set(0, 1, u[0]);
	r.set(1, 1, u[1]);
	r.set(2, 1, u[2]);

	r.set(0, 2, -f[0]);
	r.set(1, 2, -f[1]);
	r.set(2, 2, -f[2]);


	Matrix4d t;						// inverse translation
	t.identity();
	t.makeTranslate(-eyeX, -eyeY, -eyeZ);

	m = r * t;						// inverse camera


}

Matrix4d Camera::getMatrix(){
	return m;
}

void Camera::computeMatrix(){
	
	Vector3d z = e - d;
	z.normalize();
	Vector3d x = up.cross(z);
	x.normalize();
	Vector3d y = z.cross(x);
	y.normalize();
	Matrix4d r;
	r.identity();
	r.set(0, 0, x[0]);
	r.set(1, 0, x[1]);
	r.set(2, 0, x[2]);

	r.set(0, 1, y[0]);
	r.set(1, 1, y[1]);
	r.set(2, 1, y[2]);

	r.set(0, 2, z[0]);
	r.set(1, 2, z[1]);
	r.set(2, 2, z[2]);
	Matrix4d t;
	t.identity();
	t.makeTranslate(-e[0], -e[1], -e[2]);
	m = r * t;
	
}