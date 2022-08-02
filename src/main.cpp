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
void create_tets(size_t X, size_t Y, size_t Z, vector<ivec4> &tets){

    printf(" Creating tets...");

    size_t XY = X*Y;
    for(size_t slice = 1; slice < Z; slice++){
    for(size_t row = 1; row < Y; row++){
    for(size_t col = 1; col < X; col++){

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

// create tris. 2 triangles per quad
void create_tris(size_t X, size_t Y, vector<ivec3> &triangles){

    printf(" Creating triangles...");

    for(size_t row = 1; row < Y; row++){
    for(size_t col = 1; col < X; col++){

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
// actual function that computes the critical points
void compute_cp(const int &vdim, const std::vector<size_t> &dims,
                const vector<vec> &vfield, const vector<point> &points,
                const std::string &outfname) {

    if (2 == vdim) {

        vector<ivec3> tris;
        create_tris ( dims[0], dims[1], tris );

        CPDetector *CPD = new CPDetector(&vfield, &tris);
        CPD->compute();

        const std::vector<size_t> &cp = CPD->get_CP();

        /*
        // display critical points
        for(uint i = 0; i < cp.size(); i++){

            uint t = cp[i];
            point p = RW::get_centroid(tris[t], points);
            printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
            //printf(" CP exists in simplex %d\n", t);
        }*/

        RW::write_cp(outfname, cp, tris, points);
        delete CPD;
    }

    else if (3 == vdim) {

        vector<ivec4> tets;
        create_tets ( dims[0], dims[1], dims[2], tets );

        CPDetector *CPD = new CPDetector(&vfield, &tets);
        CPD->compute();

        const std::vector<size_t> &cp = CPD->get_CP();

        /*
        // display critical points
        for(size_t i = 0; i < cp.size(); i++){

            size_t t = cp[i];
            point p = get_centroid(tets[t], points);
            printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
        }*/

        RW::write_cp(outfname, cp, tets, points);
        delete CPD;
    }

    else {
        std::cerr << " Invalid dimensionality! Can be 2 or 3, but found " << vdim << std::endl;
        exit(1);
    }
}

// -----------------------------------------------------------------------
void usage(int argc, char *argv[]) {

    printf("Usage:\n");
    printf("  %s file.vti\n", argv[0]);
    printf("  %s file1 X Y\n", argv[0]);
    printf("  %s file1 X Y Z\n", argv[0]);
    printf("  %s file1 file2\n", argv[0]);
    printf("\n where,\n");
    printf("   file.vti is a VTK image data file\n");
    printf("   file1 is a text file where each line is: x y z vx vy vz (coordinates of points and corresponding vectors) [[x y vx vy: for the 2D case]]\n");
    printf("   X Y are the dimensions of the regular grid (program creates trianglues automatically)\n");
    printf("   X Y Z are the dimensions of the regular grid (program creates tets automatically)\n");
    printf("   file2 is a text file where each line is: i1 i2 i3 i4 (indices of the 3/4 corners of a tri/tet)\n");
}

// -----------------------------------------------------------------------
// main function

int main (int argc, char *argv[]){

    if(argc < 2 || argc > 5) {
        usage(argc, argv);
        exit(1);
    }


    const std::string infilename (argv[1]);
    const std::string outfilename = std::string(infilename).append(".cp.txt");

    // -----------------------------------------------------------
    // 2 arguments: ./CriticalPointDetection file1.vti
    if (argc == 2) {

#ifndef USE_VTK
        printf("VTK not available. Please reinstall with VTK libraries!\n");
        exit(1);
#else
        std::vector<size_t> dims;
        vector<vec> vfield;
        vector<point> points;

        int vdim = RW::read_vti(dims, vfield, points, infilename);
        compute_cp(vdim, dims, vfield, points, outfilename);
#endif
    }

    // -----------------------------------------------------------
    // 3 arguments: ./CriticalPointDetection file1 file2
    else if (argc == 3) {

        const std::string tri_file(argv[2]);

        // -------------------------------------------------------
        // peek in file to identify dimensionality!

        const int ncols_val = RW::count_columns(infilename);
        const int ncols_tri = RW::count_columns(tri_file);

        // 2D case
        if (ncols_val == 4 && ncols_tri == 3) {

            // vdim is the dimensionality of vector field (2 or 3)
            const int vdim = 2;

            // vector field is a vector of vec (vec is a vector of 3 doubles)
            vector<vec> vfield;
            vector<point> points;
            vector<ivec3> tris;

            RW::read_text(points, vfield, infilename, vdim);
            RW::read_text(tris, tri_file);

            CPDetector *CPD = new CPDetector(&vfield, &tris);
            CPD->compute();

            const std::vector<size_t> &cp = CPD->get_CP();

            /*
            // display critical points
            for(size_t i = 0; i < cp.size(); i++){

                size_t t = cp[i];
                point p = RW::get_centroid(tets[t], points);
                printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
            }*/

            RW::write_cp(outfilename, cp, tris, points);
            delete CPD;
        }

        else if (ncols_val == 6 && ncols_tri == 4) {

            // vdim is the dimensionality of vector field (2 or 3)
            const int vdim = 3;

            // vector field is a vector of vec (vec is a vector of 3 doubles)
            vector<vec> vfield;
            vector<point> points;
            vector<ivec4> tets;

            RW::read_text(points, vfield, infilename, vdim);
            RW::read_text(tets, tri_file);

            CPDetector *CPD = new CPDetector(&vfield, &tets);
            CPD->compute();

            const std::vector<size_t> &cp = CPD->get_CP();

            /*
            // display critical points
            for(size_t i = 0; i < cp.size(); i++){

                size_t t = cp[i];
                point p = RW::get_centroid(tets[t], points);
                printf(" CP exists in simplex %d at [%f, %f, %f]\n", t, p[0], p[1], p[2]);
            }*/

            RW::write_cp(outfilename, cp, tets, points);
            delete CPD;
        }

        else {
            std::cerr << " Invalid files/dimensionality: found " << ncols_val << " and " << ncols_tri << " columns!\n";
            exit(1);
        }
    }

    // -----------------------------------------------------------
    // 4 arguments: ./CriticalPointDetection file1 X Y
    else if (argc == 4){

        const int vdim = 2;
        const std::vector<size_t> dims ({size_t(atoi(argv[2])), size_t(atoi(argv[3]))});

        vector<vec> vfield;
        vector<point> points;

        RW::read_text(points, vfield, infilename, vdim);
        compute_cp(vdim, dims, vfield, points, outfilename);
    }

    // -----------------------------------------------------------
    // 5 arguments: ./CriticalPointDetection file1 X Y Z
    else if (argc == 3){

        const int vdim = 3;
        const std::vector<size_t> dims ({size_t(atoi(argv[2])), size_t(atoi(argv[3])), size_t(atoi(argv[4]))});

        vector<vec> vfield;
        vector<point> points;

        RW::read_text(points, vfield, infilename, vdim);
        compute_cp(vdim, dims, vfield, points, outfilename);
    }

    // -----------------------------------------------------------
    else {
        std::cerr << "Invalid number of arguments!\n";
        exit(1);
    }
    return 0;
}
