#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void Parser::parse(char* file, vector<double> &position, vector<Vector3d> &normal, double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max){
	ifstream infile;
	infile.open(file, std::ifstream::in);
	if (!infile.is_open()){
		std::cout << "failed to open file" << std::endl;
		return;
	}
	else
		std::cout << "file has been opened" << std::endl;

	string line;
	x_min = y_min = z_min = std::numeric_limits<double>::max();
	x_max = y_max = z_max = std::numeric_limits<double>::min();

	while (getline(infile, line)){
		std::istringstream iss(line);
		double d;
		iss >> d;
		if (d > x_max)
			x_max = d;
		if (d < x_min)
			x_min = d;
		position.push_back(d);

		iss >> d;
		if (d > y_max)
			y_max = d;
		if (d < y_min)
			y_min = d;
		position.push_back(d);

		iss >> d;
		if (d > z_max)
			z_max = d;
		if (d < z_min)
			z_min = d;
		position.push_back(d);

		Vector3d n;
		for (int i = 0; i < 3; i++){
			iss >> d;
			n.set(i, d);
		}
		n.normalize();
		normal.push_back(n);
	}
	std::cout << "x min: " << x_min << "x max: " << x_max << "y min: " << y_min << "y max: " << y_max << "z min: " << z_min << "z max: " << z_max << std::endl;
}