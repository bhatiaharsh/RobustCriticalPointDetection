/**
 Copyright (c) 2016, Lawrence Livermore National Security, LLC.
 Produced at the Lawrence Livermore National Laboratory
 Written by Harsh Bhatia (bhatia4@llnl.gov).
 CODE-701040.
 All rights reserved.

 This file is part of RobustCriticalPointDetection v1.0.

 For details, see https://github.com/bhatiaharsh/RobustCriticalPointDetection.
 For more details on the Licence, please read LICENCE file.
*/

#ifndef _RW_H_
#define _RW_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "vec.h"

namespace RW{

    void read_text(std::vector<point> &points, std::vector<vec> &vfield, std::string filename, int vdim);
    void read_text(std::vector<ivec4> &tets, std::string filename);
    void read_text(std::vector<ivec3> &tris, std::string filename);

    std::vector<vec> read_plot3d_q(std::string filename, int &x, int &y, int &z, int &dim);
    std::vector<point> read_plot3d_x(std::string filename, int &x, int &y, int &z);
}
#endif
