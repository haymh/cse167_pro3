#ifndef _Matrix4d_H_
#define _Matrix4d_H_

#include <string>
#include "Vector4d.h"
#include "Vector3d.h"

#define M_PI 3.14159265358979323846

class Matrix4d
{
  protected:
    double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)
    
  public:
    Matrix4d();     
    Matrix4d& operator=(const Matrix4d&);
	Matrix4d operator* (const Matrix4d&);
	Vector4d operator* (Vector4d&);
	void multiply(const Matrix4d&);
	void makeRotateY(double angle);
	void makeRotateX(double angle);
	void makeRotateZ(double angle);
	void makeRotate(double angle, Vector3d & axis);
	void makeScale(double sx, double sy, double sz);
	void makeTranslate(double tx, double ty, double tz);
	void set(int, int, double);
    double* getPointer(); 
    void identity(); 
    void transpose();
	void print(std::string comment);
};

#endif