#include <iostream>

#include "Vector4d.h"

Vector4d::Vector4d(){
	for (int i = 0; i < 3; i++)
		v[i] = 0.0;
	v[3] = 1.0;
}
Vector4d::Vector4d(GLdouble x, GLdouble y, GLdouble z){
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = 1.0;
}

Vector4d::Vector4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w){
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

void Vector4d::set(int index, GLdouble value){
	if (index <= 4 && index >= 0)
		v[index] = value;
}

GLdouble Vector4d::get(int index){
	return v[index];
}

GLdouble Vector4d::operator[](int index){
	return v[index];
}

Vector4d Vector4d::operator+(Vector4d & another){
	GLdouble x = v[0] / v[3];
	GLdouble y = v[1] / v[3];
	GLdouble z = v[2] / v[3];
	GLdouble w = v[3];
	x = (x + another[0] / another[3]) * w;
	y = (y + another[1] / another[3]) * w;
	z = (z + another[2] / another[3]) * w;
	return Vector4d(x, y, z, w);
}

Vector4d& Vector4d::operator=(Vector4d & other){
	if (this != &other)
	{
		for (int i = 0; i<4; ++i)
		{
			v[i] = other[i];
		}
	}
	return *this;
}

void Vector4d::add(Vector4d & another){
	GLdouble x = another[0] / another[3];
	GLdouble y = another[1] / another[3];
	GLdouble z = another[2] / another[3];
	v[0] = (v[0] / v[3] + x) * v[3];
	v[1] = (v[1] / v[3] + y) * v[3];
	v[2] = (v[2] / v[3] + z) * v[3];
}

Vector4d Vector4d::operator-(Vector4d & another){
	GLdouble x = v[0] / v[3];
	GLdouble y = v[1] / v[3];
	GLdouble z = v[2] / v[3];
	GLdouble w = v[3];
	x = (x - another[0] / another[3]) * w;
	y = (y - another[1] / another[3]) * w;
	z = (z - another[2] / another[3]) * w;
	return Vector4d(x, y, z, w);
}

void Vector4d::substract(Vector4d & another){
	GLdouble x = another[0] / another[3];
	GLdouble y = another[1] / another[3];
	GLdouble z = another[2] / another[3];
	v[0] = (v[0] / v[3] - x) * v[3];
	v[1] = (v[1] / v[3] - y) * v[3];
	v[2] = (v[2] / v[3] - z) * v[3];
}

void Vector4d::dehomogenize(){
	for (int i = 0; i < 3; i++)
		v[i] /= v[3];
	v[3] = 1.0;
}

void Vector4d::print(){
	std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << std::endl;
}