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

#include "vec.h"
#include "RW.h"
#include "CP.h"

point get_centroid(const ivec4 &tet, const vector<point> &points){
    return ( points[tet[0]] + points[tet[1]] + points[tet[2]] + points[tet[3]] ) / 4.0;
}

point get_centroid(const ivec3 &tet, const vector<point> &points){
    return ( points[tet[0]] + points[tet[1]] + points[tet[2]] ) / 3.0;
}

// -----------------------------------------------------------------------
// create tets. 5 tets per cube : http://www.ics.uci.edu/~eppstein/projects/tetra/
vector<ivec4> create_tets(uint X, uint Y, uint Z){

    printf(" Creating tets...");

    vector<ivec4> tets;

    uint XY = X*Y;
    for(uint slice = 1; slice < Z; slice++){
    for(uint row = 1; row < Y; row++){
    for(uint col = 1; col < X; col++){

        // v is the bottom left vertex (origin)
        // vx is the next vertex in x direction
        // vxy is diagonally opp to v in xy plane
        // vxyz is diag opp to v in the cube
        int v    = XY*(slice-1) + X*(row-1) + (col-1);

        int vx   = XY*(slice-1) + X*(row-1) + (col);
        int vy   = XY*(slice-1) + X*(row)   + (col-1);
        int vz   = XY*(slice)   + X*(row-1) + (col-1);

        int vxy  = XY*(slice-1) + X*(row)   + (col);
        int vyz  = XY*(slice)   + X*(row)   + (col-1);
        int vxz  = XY*(slice)   + X*(row-1) + (col);

        int vxyz = XY*(slice)   + X*(row)   + (col);

        /*cout<<" slice = "<<slice<<", col = "<<col<<" row = "<<row<<endl;
        cout<<"\t v - "<<v<<","<<vx<<","<<vy<<","<<vz<<","<<vxy<<","<<vxz<<","<<vyz<<","<<vxyz<<endl;

        cout<<"\t creating "<<v<<","<<vx<<","<<vy<<","<<vz<<"\n";
        cout<<"\t creating "<<vxz<<","<<vxyz<<","<<vz<<","<<vx<<"\n";
        cout<<"\t creating "<<vyz<<","<<vz<<","<<vy<<","<<vxyz<<"\n";
        cout<<"\t creating "<<vxy<<","<<vxyz<<","<<vx<<","<<vy<<"\n";
        cout<<"\t creating "<<vz<<","<<vx<<","<<vxyz<<","<<vy<<"\n";*/

        tets.push_back( ivec4(v, vx, vz, vy) );
        tets.push_back( ivec4(vxz, vxyz, vz, vx) );
        tets.push_back( ivec4(vyz, vz, vy, vxyz) );
        tets.push_back( ivec4(vxy, vxyz, vx, vy) );
        tets.push_back( ivec4(vz, vx, vxyz, vy) );
    }
    }
    }
    printf("Done! Created %'d tets!\n", tets.size());
    return tets;
}


// -----------------------------------------------------------------------
// main function

int main (int argc, char *argv[]){

    // -----------------------------------------------------------
    // vdim is the dimensionality of vector field (2 or 3)
    int vdim = 3;

    // vector field is a vector of vec (vec is a vector of 3 doubles)
    vector<vec> vfield;
    vector<point> points;
    vector<ivec4> tets;

    RW::read_text(points, vfield, "/Users/bhatia4/WORK/data/from_Primoz/random/vfrandom5.txt", vdim);
    RW::read_text(tets, "/Users/bhatia4/WORK/data/from_Primoz/random/tetlistrandom5.txt");

    CPDetector *CPD = new CPDetector(&vfield, &tets);
    CPD->compute();

    const std::vector<uint> &cp = CPD->get_CP();

    // display critical points
    for(uint i = 0; i < cp.size(); i++){

        uint t = cp[i];
        point p = get_centroid(tets[t], points);
        printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
    }

    delete CPD;
    return 0;
}
