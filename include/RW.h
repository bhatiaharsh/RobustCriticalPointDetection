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

    int read_vti(std::vector<size_t> &dims, std::vector<vec> &vfield, std::vector<point> &points, std::string filename);



    point get_centroid(const ivec4 &tet, const std::vector<point> &points);
    point get_centroid(const ivec3 &tri, const std::vector<point> &points);

    template<typename T>
    void write_cp(const std::string &filename, const std::vector<uint> &cp, const std::vector<T> &cells, const std::vector<point> &points) {

        std::ofstream infile(filename.c_str());
        if(!infile.is_open()){
            std::cerr << "Unable to open file "<<filename<<std::endl;
            exit(1);
        }

        printf(" Write critical points to file %s...", filename.c_str());
        fflush(stdout);

        // display critical points
        for(uint i = 0; i < cp.size(); i++){

            uint t = cp[i];
            point p = get_centroid(cells[t], points);

            infile << t << " " << p[0] << " " << p[1] << " " << p[2] << std::endl;
        }
        infile.close();
        printf(" Done! Wrote %'ld critical points\n", cp.size());
    }

}
#endif
