#include <math.h>
#include <iostream>

#include "Vector3d.h"

Vector3d::Vector3d(){
	v[0] = v[1] = v[2] = 0.0;
}

Vector3d::Vector3d(GLdouble x, GLdouble y, GLdouble z){
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void Vector3d::set(int index, GLdouble value){
	if (index <= 2 && index >= 0)
		v[index] = value;
}

GLdouble Vector3d::get(int index){
	return v[index];
}

GLdouble Vector3d::operator[](int index){
	return get(index);
}

Vector3d Vector3d::operator+(Vector3d & another){
	return Vector3d(v[0] + another[0], v[1] + another[1], v[2] + another[2]);
}

void Vector3d::add(Vector3d & another){
	for (int i = 0; i < 3; i++)
		v[i] += another[i];
}

Vector3d Vector3d::operator-(Vector3d & another){
	return Vector3d(v[0] - another[0], v[1] - another[1], v[2] - another[2]);
}

void Vector3d::substract(Vector3d & another){
	for (int i = 0; i < 3; i++)
		v[i] -= another.get(i);
}

void Vector3d::negate(){
	for (int i = 0; i < 3; i++)
		v[i] = 0 - v[i];
}

void Vector3d::scale(GLdouble s){
	for (int i = 0; i < 3; i++)
		v[i] *= s;
}

GLdouble Vector3d::dot(Vector3d & another){
	double sum = 0;
	for (int i = 0; i < 3; i++)
		sum += v[i] * another[i];
	return sum;
}

GLdouble Vector3d::dot(Vector3d & one, Vector3d & two){
	return one[0] * two[0] + one[1] * two[1] + one[2] * two[2];
}

Vector3d Vector3d::cross(Vector3d & another){
	GLdouble x = v[1] * another[2] - v[2] * another[1];
	GLdouble y = v[2] * another[0] - v[0] * another[2];
	GLdouble z = v[0] * another[1] - v[1] * another[0];
	return Vector3d(x, y, z);
}

GLdouble Vector3d::magnitude(){
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void Vector3d::normalize(){
	GLdouble m = magnitude();
	for (int i = 0; i < 3; i++)
		v[i] /= m;
}

void Vector3d::print(char* msg){
	std::cout << msg << v[0] << " " << v[1] << " " << v[2] << std::endl;
}