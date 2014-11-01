#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "GL\glew.h"

class Vector3d
{
protected:
	GLdouble v[3];   // matrix elements

public:
	Vector3d();        // constructor
	Vector3d(GLdouble, GLdouble, GLdouble);
	void set(int index, GLdouble value);
	GLdouble get(int index);
	GLdouble operator[](int index);
	Vector3d operator+(Vector3d & another);
	void add(Vector3d & another);
	Vector3d operator-(Vector3d & another);
	void substract(Vector3d & another);
	void negate();
	void scale(GLdouble s);
	GLdouble dot(Vector3d & another);
	static GLdouble dot(Vector3d &, Vector3d &);
	Vector3d cross(Vector3d & another);
	GLdouble magnitude();
	void normalize();
	void print(char*);

};

#endif