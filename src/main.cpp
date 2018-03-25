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

// -----------------------------------------------------------------------
// create tets. 5 tets per cube : http://www.ics.uci.edu/~eppstein/projects/tetra/
void create_tets(uint X, uint Y, uint Z, vector<ivec4> &tets){

    printf(" Creating tets...");

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
}

// create tris. 2 triangles ber quad
void create_tris(uint X, uint Y, vector<ivec3> &triangles){

    printf(" Creating triangles...");

    for(uint row = 1; row < Y; row++){
    for(uint col = 1; col < X; col++){

        // v is the bottom left vertex (origin)
        // vx is the next vertex in x direction
        // vxy is diagonally opp to v in xy plane
        int v    = X*(row-1) + (col-1);

        int vx   = X*(row-1) + (col);
        int vy   = X*(row)   + (col-1);
        int vxy  = X*(row)   + (col);

        /*
        cout<<" col = "<<col<<" row = "<<row<<endl;
        cout<<"\t v - "<<v<<","<<vx<<","<<vy<<","<<vxy<<endl;

        cout<<"\t creating "<<v<<","<<vx<<","<<vxy<<"\n";
        cout<<"\t creating "<<v<<","<<vxy<<","<<vy<<"\n";*/

        triangles.push_back( ivec3(v, vx, vxy) );
        triangles.push_back( ivec3(v, vxy, vy) );
    }
    }
    printf("Done! Created %'d triangles!\n", triangles.size());
}

// -----------------------------------------------------------------------
void usage(int argc, char *argv[]) {

    printf("Usage:\n");
    printf("  %s file.vti\n", argv[0]);
    printf("  %s file1 X Y Z\n", argv[0]);
    printf("  %s file1 file2\n", argv[0]);
    printf("\n where,\n");
    printf("   file.vti is a VTK image data file\n");
    printf("   file1 is a text file where each line is: x y z vx vy vz (coordinates of points and corresponding vectors)\n");
    printf("   X Y Z are the dimensions of the regular grid (program creates tets automatically)\n");
    printf("   file2 is a text file where each line is: i1 i2 i3 i4 (indices of the 4 corners of a tet)\n");
}

// -----------------------------------------------------------------------
// main function

int main (int argc, char *argv[]){

    if(argc != 2 && argc != 3 && argc != 5) {
        usage(argc, argv);
        exit(1);
    }

    // -----------------------------------------------------------
    // read dimensionality from vtk file
    if (argc == 2) {

#ifndef USE_VTK
        printf("VTK not available. Please reinstall with VTK libraries!\n");
        exit(1);
#else
        std::vector<size_t> dims;
        vector<vec> vfield;
        vector<point> points;

        int vdim = RW::read_vti(dims, vfield, points, argv[1]);

        if (vdim == 2) {

            vector<ivec3> tris;
            create_tris ( dims[0], dims[1], tris );

            CPDetector *CPD = new CPDetector(&vfield, &tris);
            CPD->compute();

            const std::vector<uint> &cp = CPD->get_CP();

            /*
            // display critical points
            for(uint i = 0; i < cp.size(); i++){

                uint t = cp[i];
                point p = RW::get_centroid(tris[t], points);
                printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
                //printf(" CP exists in simplex %d\n", t);
            }*/

            std::string outfile(argv[1]);
            outfile = outfile.append(".cp.txt");
            RW::write_cp(outfile, cp, tris, points);

            delete CPD;
        }
        else if (vdim == 3) {

            vector<ivec4> tets;
            create_tets ( dims[0], dims[1], dims[2], tets );

            CPDetector *CPD = new CPDetector(&vfield, &tets);
            CPD->compute();

            const std::vector<uint> &cp = CPD->get_CP();

            /*
            // display critical points
            for(uint i = 0; i < cp.size(); i++){

                uint t = cp[i];
                point p = get_centroid(tets[t], points);
                printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
            }*/
            std::string outfile(argv[1]);
            outfile = outfile.append(".cp.txt");
            RW::write_cp(outfile, cp, tets, points);

            delete CPD;
        }
#endif
    }


    // -----------------------------------------------------------
    else{
        // currently, working only for 3D
        // vdim is the dimensionality of vector field (2 or 3)
        const int vdim = 3;

        // vector field is a vector of vec (vec is a vector of 3 doubles)
        vector<vec> vfield;
        vector<point> points;
        vector<ivec4> tets;

        RW::read_text(points, vfield, argv[1], vdim);

        if(argc == 3) {     RW::read_text(tets, argv[2]);                                       }
        else {              create_tets ( atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), tets );  }

        CPDetector *CPD = new CPDetector(&vfield, &tets);
        CPD->compute();

        const std::vector<uint> &cp = CPD->get_CP();

        /*
        // display critical points
        for(uint i = 0; i < cp.size(); i++){

            uint t = cp[i];
            point p = RW::get_centroid(tets[t], points);
            printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
        }*/

        std::string outfile(argv[1]);
        outfile = outfile.append(".cp.txt");
        RW::write_cp(outfile, cp, tets, points);

        delete CPD;
    }
    return 0;
}
