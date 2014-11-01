#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include "Vector3d.h"


class Parser	  // OpenGL output window related routines
{
public:
	static void parse(char* file, std::vector<double> &position, std::vector<Vector3d> &normal, double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max);
};

#endif

