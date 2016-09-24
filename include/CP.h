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

#ifndef _CP_H_
#define _CP_H_

#include <vector>
#include "vec.h"
#include "sos_utils.h"

class CPDetector{

    unsigned int dim;
    const std::vector<vec> *vfield;   // vector field
    const std::vector<ivec4> *tets;   // tets
    const std::vector<ivec3> *tris;   // tets

    std::vector<unsigned int> cp;       // indices of simplices containing cp


    unsigned int SOS_ZERO_IDX = 1;  // index assigned to zero value!
    bool createSoS(bool verbose = false);

public:
    CPDetector(const std::vector<vec> *vfield_, std::vector<ivec4> *tets_) :
        dim(3), vfield(vfield_), tets(tets_), tris(0) {

        createSoS();
    }

    CPDetector(const std::vector<vec> *vfield_, std::vector<ivec3> *tris_) :
        dim(2), vfield(vfield_), tets(0), tris(tris_) {

        createSoS();
    }

    ~CPDetector() {
        sos_shutdown();
    }

    static float sign (const point &p1, const point &p2, const point &p3);
    static bool point_in_triangle(const point &p, const point &a, const point &b, const point &c);
    static bool point_in_tetrahedron(const point &p, const point &a, const point &b, const point &c, const point &d);

    void compute();
    const std::vector<unsigned int>& get_CP() const {   return cp;  }

};
#endif
